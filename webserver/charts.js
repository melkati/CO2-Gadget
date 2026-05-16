/* =========================================================
   CO2 Gadget – Charts page
   ========================================================= */

/**
 * Convierte un timestamp (ms) a string 'yyyy-MM-ddTHH:mm' en hora local del navegador.
 * Se usa para rellenar los inputs datetime-local cuando el usuario activa el filtro.
 */
function tsToDatetimeLocal(ts) {
    const d = new Date(ts);
    const pad = n => String(n).padStart(2, '0');
    return d.getFullYear() + '-' + pad(d.getMonth() + 1) + '-' + pad(d.getDate()) +
        'T' + pad(d.getHours()) + ':' + pad(d.getMinutes());
}

/** Descarga un fichero de texto en el navegador */
function downloadText(filename, content) {
    const a = document.createElement('a');
    a.href = URL.createObjectURL(new Blob([content], { type: 'text/plain' }));
    a.download = filename;
    a.click();
    URL.revokeObjectURL(a.href);
}

/**
 * Construye los puntos (timestamp, value) a partir de la respuesta de la API.
 *
 * El ESP32 usa millis() (uptime desde boot), NO hora de pared (epoch).
 * Por tanto data.lastTimestamp es tiempo de uptime del primer punto, no
 * convertible directamente a wall-clock.
 *
 * Estrategia correcta:
 *   - El punto con índice `data.end` (el más reciente) fue añadido al buffer
 *     aproximadamente en el momento en que se recibió esta respuesta HTTP.
 *   - Asumimos: wall_time(data.end) ≈ Date.now()
 *   - Cada punto anterior está un intervalDuration antes:
 *     wall_time(i) = Date.now() + (i - data.end) * intervalDuration
 *
 * Si el usuario ha configurado una zona horaria distinta a la del navegador en
 * las preferencias, aplicamos el desplazamiento extra (tzExtraMs) para que
 * Highcharts (useUTC:false) muestre la hora de la zona guardada.
 *
 * El buffer almacena promedios de movingAverageInterval segundos (por defecto
 * 60 s). Cada punto es ya un "bucket" de 1 minuto de media. Con 1440 puntos
 * (capacidad por defecto = 24 h) Highcharts renderiza sin problemas y no es
 * necesario hacer ningún downsampling adicional en el navegador.
 */
function buildPoints(data) {
    const values = data.data;
    const interval = data.intervalDuration;  // ms entre puntos consecutivos
    const end = data.end;                     // índice del punto más reciente

    // El punto más reciente fue añadido aproximadamente cuando se recibió la respuesta.
    // Highcharts con useUTC:false ya aplica la zona horaria del navegador al mostrar.
    const newestTs = Date.now();

    return values.map((v, i) => [newestTs + (i - end) * interval, v]);
}

/** Filtra puntos por rango de timestamps (ambos extremos inclusivos) */
function filterPoints(points, fromTs, toTs) {
    return points.filter(([ts]) => ts >= fromTs && ts <= toTs);
}

/** Inyecta el panel de controles encima del contenedor del gráfico */
function injectControls() {
    const container = document.getElementById('container');
    if (!container || document.getElementById('charts-controls')) return;

    const panel = document.createElement('div');
    panel.id = 'charts-controls';
    panel.innerHTML = `
        <div class="charts-filter-group">
            <label class="charts-filter-toggle" title="Activar filtro de rango de fechas">
                <input type="checkbox" id="charts-filter-enabled">
                <span>Rango de fechas:</span>
            </label>
            <div id="charts-range-inputs" class="charts-range-inputs charts-range-disabled">
                <input type="datetime-local" id="charts-from" title="Desde">
                <span class="charts-range-sep">→</span>
                <input type="datetime-local" id="charts-to" title="Hasta">
                <button id="charts-apply" class="btn-charts">Aplicar</button>
            </div>
        </div>
        <div class="charts-export-btns">
            <span class="charts-export-label">Exportar:</span>
            <button id="charts-csv"  class="btn-charts" title="Descargar datos en CSV">CSV</button>
            <button id="charts-json" class="btn-charts" title="Descargar datos en JSON">JSON</button>
            <button id="charts-png"  class="btn-charts" title="Descargar gráfico como imagen PNG">PNG</button>
            <button id="charts-svg"  class="btn-charts" title="Descargar gráfico como vector SVG">SVG</button>
        </div>
    `;

    const style = document.createElement('style');
    style.textContent = `
        #charts-controls {
            display: flex;
            flex-wrap: wrap;
            align-items: center;
            gap: 8px 16px;
            margin-bottom: 14px;
            padding: 10px 16px;
            border-radius: var(--r-md, 10px);
            background: var(--surface-2, #f5f5f7);
            border: 1px solid var(--border, #d1d1d6);
            font-size: .85rem;
        }
        /* Filter group: checkbox + date pickers always visually together */
        .charts-filter-group {
            display: flex;
            align-items: center;
            flex-wrap: wrap;
            gap: 8px;
            flex: 1;
            min-width: 0;
        }
        .charts-filter-toggle {
            display: flex;
            align-items: center;
            gap: 7px;
            cursor: pointer;
            user-select: none;
            white-space: nowrap;
        }
        .charts-filter-toggle input[type=checkbox] {
            accent-color: var(--accent, #007aff);
            width: 15px; height: 15px;
            cursor: pointer;
        }
        .charts-range-inputs {
            display: flex;
            flex-wrap: wrap;
            align-items: center;
            gap: 6px;
            transition: opacity .2s;
        }
        .charts-range-inputs input[type=datetime-local] {
            font-size: .82rem;
            padding: 3px 6px;
            border: 1px solid var(--border, #d1d1d6);
            border-radius: var(--r-sm, 6px);
            background: var(--surface, #fff);
            color: var(--text-1, #333);
        }
        .charts-range-sep {
            color: var(--text-3, #aaa);
            user-select: none;
        }
        .charts-range-disabled {
            opacity: 0.38;
            pointer-events: none;
        }
        /* Export buttons group */
        .charts-export-btns {
            display: flex;
            align-items: center;
            gap: 6px;
            flex-shrink: 0;
            border-left: 1px solid var(--border, #d1d1d6);
            padding-left: 16px;
        }
        .charts-export-label {
            font-size: .78rem;
            color: var(--text-3, #aaa);
            white-space: nowrap;
            user-select: none;
        }
        .btn-charts {
            padding: 4px 11px;
            cursor: pointer;
            border: 1px solid var(--border, #d1d1d6);
            border-radius: var(--r-sm, 6px);
            background: var(--surface, #fff);
            color: var(--text-1, #333);
            font-size: .82rem;
            transition: background .15s;
        }
        .btn-charts:hover { background: var(--surface-3, #ebebed); }
        @media (max-width: 600px) {
            .charts-export-btns {
                border-left: none;
                padding-left: 0;
                border-top: 1px solid var(--border, #d1d1d6);
                padding-top: 8px;
                width: 100%;
            }
        }
    `;
    document.head.appendChild(style);
    container.parentNode.insertBefore(panel, container);
}

/**
 * Crea/actualiza el gráfico Highcharts y registra toda la lógica de controles.
 */
function CreateChart() {
    if (typeof Highcharts !== 'undefined') {
        Highcharts.setOptions({ time: { useUTC: false } });
    }

    injectControls();

    let allPoints = [];
    let chart = null;

    const cs = prop => getComputedStyle(document.documentElement).getPropertyValue(prop).trim();

    function buildOptions(points) {
        return {
            chart: {
                type: 'line',
                backgroundColor: cs('--bg-color'),
                reflow: true,
                animation: false
            },
            title: {
                text: 'CO\u2082 \u2013 mediciones recientes',
                style: { color: cs('--title-color') }
            },
            xAxis: {
                type: 'datetime',
                labels: { format: '{value:%d/%m %H:%M}', style: { color: cs('--font-color') } }
            },
            yAxis: {
                title: { text: 'CO\u2082 (ppm)', style: { color: cs('--font-color') } },
                labels: { style: { color: cs('--font-color') } }
            },
            series: [{
                name: 'CO\u2082',
                data: points,
                color: cs('--title-color'),
                marker: { enabled: points.length < 120 }
            }],
            tooltip: {
                xDateFormat: '%d/%m/%Y %H:%M:%S'
            },
            exporting: { enabled: false },
            legend: { itemStyle: { color: cs('--font-color') } },
            credits: { enabled: false },
            responsive: {
                rules: [{
                    condition: { maxWidth: 500 },
                    chartOptions: {
                        xAxis: { labels: { format: '{value:%H:%M}' } }
                    }
                }]
            }
        };
    }

    function renderChart(points) {
        if (chart) {
            chart.series[0].setData(points, true, false, false);
        } else {
            chart = Highcharts.chart('container', buildOptions(points));
        }
    }

    function isFilterEnabled() {
        const cb = document.getElementById('charts-filter-enabled');
        return cb && cb.checked;
    }

    function getRange() {
        const fromInput = document.getElementById('charts-from');
        const toInput   = document.getElementById('charts-to');
        const fromTs = fromInput && fromInput.value ? new Date(fromInput.value).getTime() : -Infinity;
        const toTs   = toInput   && toInput.value   ? new Date(toInput.value).getTime()   :  Infinity;
        return { fromTs, toTs };
    }

    function applyFilter() {
        if (!allPoints.length) return;
        if (!isFilterEnabled()) {
            renderChart(allPoints);
            return;
        }
        const { fromTs, toTs } = getRange();
        const pts = filterPoints(allPoints, fromTs, toTs);
        renderChart(pts.length ? pts : allPoints);
    }

    function populateRangeInputs() {
        if (!allPoints.length) return;
        const fromInput = document.getElementById('charts-from');
        const toInput   = document.getElementById('charts-to');
        if (fromInput) fromInput.value = tsToDatetimeLocal(allPoints[0][0]);
        if (toInput)   toInput.value   = tsToDatetimeLocal(allPoints[allPoints.length - 1][0]);
    }

    // Checkbox: activa/desactiva el filtro de rango
    const filterCb = document.getElementById('charts-filter-enabled');
    const rangeDiv  = document.getElementById('charts-range-inputs');
    if (filterCb) {
        filterCb.addEventListener('change', () => {
            if (filterCb.checked) {
                rangeDiv.classList.remove('charts-range-disabled');
                populateRangeInputs();
            } else {
                rangeDiv.classList.add('charts-range-disabled');
                renderChart(allPoints);
            }
        });
    }

    document.getElementById('charts-apply').addEventListener('click', applyFilter);

    function exportCSV() {
        const { fromTs, toTs } = getRange();
        const pts = isFilterEnabled() ? filterPoints(allPoints, fromTs, toTs) : allPoints;
        const rows = [['Timestamp', 'CO2_ppm']].concat(
            pts.map(([ts, v]) => [new Date(ts).toISOString(), v])
        );
        downloadText('co2_data.csv', rows.map(r => r.join(',')).join('\n'));
    }

    function exportJSON() {
        const { fromTs, toTs } = getRange();
        const pts = isFilterEnabled() ? filterPoints(allPoints, fromTs, toTs) : allPoints;
        downloadText('co2_data.json', JSON.stringify(
            pts.map(([ts, v]) => ({ timestamp: new Date(ts).toISOString(), co2_ppm: v })),
            null, 2
        ));
    }

    document.getElementById('charts-csv').addEventListener('click', exportCSV);
    document.getElementById('charts-json').addEventListener('click', exportJSON);
    document.getElementById('charts-png').addEventListener('click', () => {
        if (chart) chart.exportChart({ type: 'image/png', filename: 'co2_data' });
    });
    document.getElementById('charts-svg').addEventListener('click', () => {
        if (chart) chart.exportChart({ type: 'image/svg+xml', filename: 'co2_data' });
    });

    // Actualizar colores al cambiar de tema
    document.addEventListener('themeChange', () => {
        if (!chart) return;
        chart.update({
            chart: { backgroundColor: cs('--bg-color') },
            title: { style: { color: cs('--title-color') } },
            xAxis: { labels: { style: { color: cs('--font-color') } } },
            yAxis: {
                title: { style: { color: cs('--font-color') } },
                labels: { style: { color: cs('--font-color') } }
            },
            series: [{ color: cs('--title-color') }],
            legend: { itemStyle: { color: cs('--font-color') } }
        }, true, false, false);
    });

    window.addEventListener('resize', () => { if (chart) chart.reflow(); });

    // Carga inicial y refresco automático cada 60 s.
    // Si el filtro está desactivado (por defecto) siempre se muestran todos los puntos.
    // Si el filtro está activo, los inputs ya están fijados por el usuario → no se sobreescriben.
    function fetchAndRender() {
        fetch('/circularBufferData')
            .then(r => r.json())
            .then(data => {
                allPoints = buildPoints(data);
                applyFilter();
            })
            .catch(err => console.error('Error fetching chart data:', err));
    }

    fetchAndRender();
    setInterval(fetchAndRender, 60000);
}

document.addEventListener('DOMContentLoaded', function () {
    if (window.location.href.includes('charts.html') || window.location.pathname === '/charts.html') {
        if (typeof highlightCurrentPage === 'function') highlightCurrentPage();
        CreateChart();
    }
});
