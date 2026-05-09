/* =========================================================
   CO2 Gadget – Charts page
   ========================================================= */

/** Convierte un timestamp Unix (ms) a string local yyyy-MM-ddTHH:mm */
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

/** Construye los puntos (timestamp, value) a partir de la respuesta de la API */
function buildPoints(data) {
    const values = data.data;
    const intervalDuration = data.intervalDuration;
    const baseTs = Date.now() - (data.lastTimestamp - data.end * intervalDuration);
    return values.map((v, i) => [baseTs + i * intervalDuration, v]);
}

/** Aplica el filtro de rango a los puntos */
function filterPoints(points, fromTs, toTs) {
    return points.filter(([ts]) => ts >= fromTs && ts <= toTs);
}

/** Inyecta el panel de controles encima del contenedor del gráfico */
function injectControls() {
    const container = document.getElementById('container');
    if (!container || document.getElementById('charts-controls')) return;

    const panel = document.createElement('div');
    panel.id = 'charts-controls';
    panel.style.cssText = 'display:flex;flex-wrap:wrap;gap:8px;align-items:center;margin-bottom:12px;';
    panel.innerHTML = `
        <label style="font-size:.85rem;">Desde:
            <input type="datetime-local" id="charts-from" style="margin-left:4px;">
        </label>
        <label style="font-size:.85rem;">Hasta:
            <input type="datetime-local" id="charts-to" style="margin-left:4px;">
        </label>
        <button id="charts-apply" class="btn-charts">Aplicar</button>
        <button id="charts-reset" class="btn-charts">Todos</button>
        <button id="charts-csv"   class="btn-charts">↓ CSV</button>
        <button id="charts-json"  class="btn-charts">↓ JSON</button>
    `;

    const style = document.createElement('style');
    style.textContent = '.btn-charts{padding:4px 10px;cursor:pointer;border:1px solid var(--font-color,#666);border-radius:4px;background:var(--bg-color,#fff);color:var(--font-color,#333);font-size:.82rem;}';
    document.head.appendChild(style);

    container.parentNode.insertBefore(panel, container);
}

/**
 * Crea/actualiza el gráfico Highcharts y registra toda la lógica de controles.
 */
function CreateChart() {
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
                text: 'CO2 Levels Over Time',
                style: { color: cs('--title-color') }
            },
            xAxis: {
                type: 'datetime',
                labels: { format: '{value:%d/%m %H:%M}', style: { color: cs('--font-color') } }
            },
            yAxis: {
                title: { text: 'CO2 (ppm)', style: { color: cs('--font-color') } },
                labels: { style: { color: cs('--font-color') } }
            },
            series: [{
                name: 'CO2',
                data: points,
                color: cs('--title-color'),
                marker: { enabled: points.length < 120 }
            }],
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

    function getRange() {
        const fromInput = document.getElementById('charts-from');
        const toInput   = document.getElementById('charts-to');
        const fromTs = fromInput && fromInput.value ? new Date(fromInput.value).getTime() : -Infinity;
        const toTs   = toInput   && toInput.value   ? new Date(toInput.value).getTime()   : Infinity;
        return { fromTs, toTs };
    }

    function applyFilter() {
        const { fromTs, toTs } = getRange();
        const pts = filterPoints(allPoints, fromTs, toTs);
        renderChart(pts.length ? pts : allPoints);
    }

    function resetFilter() {
        if (!allPoints.length) return;
        const fromInput = document.getElementById('charts-from');
        const toInput   = document.getElementById('charts-to');
        if (fromInput) fromInput.value = tsToDatetimeLocal(allPoints[0][0]);
        if (toInput)   toInput.value   = tsToDatetimeLocal(allPoints[allPoints.length - 1][0]);
        renderChart(allPoints);
    }

    function exportCSV() {
        const { fromTs, toTs } = getRange();
        const pts = filterPoints(allPoints, fromTs, toTs);
        const rows = [['Timestamp', 'CO2_ppm']].concat(pts.map(([ts, v]) => [new Date(ts).toISOString(), v]));
        downloadText('co2_data.csv', rows.map(r => r.join(',')).join('\n'));
    }

    function exportJSON() {
        const { fromTs, toTs } = getRange();
        const pts = filterPoints(allPoints, fromTs, toTs);
        const obj = pts.map(([ts, v]) => ({ timestamp: new Date(ts).toISOString(), co2_ppm: v }));
        downloadText('co2_data.json', JSON.stringify(obj, null, 2));
    }

    // Eventos de los botones
    document.getElementById('charts-apply').addEventListener('click', applyFilter);
    document.getElementById('charts-reset').addEventListener('click', resetFilter);
    document.getElementById('charts-csv').addEventListener('click', exportCSV);
    document.getElementById('charts-json').addEventListener('click', exportJSON);

    // Actualización automática del gráfico al cambiar el tema
    document.addEventListener('themeChange', () => {
        if (!chart) return;
        chart.update({
            chart: { backgroundColor: cs('--bg-color') },
            title: { style: { color: cs('--title-color') } },
            xAxis: { labels: { style: { color: cs('--font-color') } } },
            yAxis: { title: { style: { color: cs('--font-color') } }, labels: { style: { color: cs('--font-color') } } },
            series: [{ color: cs('--title-color') }],
            legend: { itemStyle: { color: cs('--font-color') } }
        }, true, false, false);
    });

    // Redimensionar el gráfico con la ventana
    window.addEventListener('resize', () => { if (chart) chart.reflow(); });

    // Carga inicial y refresco cada 60 s
    function fetchAndRender() {
        fetch('/circularBufferData')
            .then(r => r.json())
            .then(data => {
                allPoints = buildPoints(data);
                // Inicializar inputs la primera vez
                const fromInput = document.getElementById('charts-from');
                const toInput   = document.getElementById('charts-to');
                if (fromInput && !fromInput.value && allPoints.length) {
                    fromInput.value = tsToDatetimeLocal(allPoints[0][0]);
                }
                if (toInput && !toInput.value && allPoints.length) {
                    toInput.value = tsToDatetimeLocal(allPoints[allPoints.length - 1][0]);
                }
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
