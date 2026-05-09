/**
 * Creates a chart using Highcharts library and fetches data to update the chart periodically.
 */
function CreateChart() {
    const fetchDataAndUpdateChart = () => {
        fetch('/circularBufferData')
            .then(response => response.json())
            .then(data => {
                const timestamps = [];
                const values = data.data;
                const intervalDuration = data.intervalDuration;

                let currentTimestamp = Date.now() - (data.lastTimestamp - data.end * intervalDuration);

                values.forEach((value, index) => {
                    timestamps.push(currentTimestamp + index * intervalDuration);
                });

                const chartOptions = {
                    chart: {
                        type: 'line',
                        backgroundColor: getComputedStyle(document.documentElement).getPropertyValue('--bg-color')
                    },
                    title: {
                        text: 'CO2 Levels Over Time',
                        style: {
                            color: getComputedStyle(document.documentElement).getPropertyValue('--title-color')
                        }
                    },
                    xAxis: {
                        type: 'datetime',
                        labels: {
                            format: '{value:%H:%M}',
                            style: {
                                color: getComputedStyle(document.documentElement).getPropertyValue('--font-color')
                            }
                        }
                    },
                    yAxis: {
                        title: {
                            text: 'CO2 Levels (ppm)',
                            style: {
                                color: getComputedStyle(document.documentElement).getPropertyValue('--font-color')
                            }
                        },
                        labels: {
                            style: {
                                color: getComputedStyle(document.documentElement).getPropertyValue('--font-color')
                            }
                        }
                    },
                    series: [{
                        name: 'CO2 Level',
                        data: values.map((value, index) => [timestamps[index], value]),
                        color: getComputedStyle(document.documentElement).getPropertyValue('--title-color')
                    }],
                    legend: {
                        itemStyle: {
                            color: getComputedStyle(document.documentElement).getPropertyValue('--font-color')
                        }
                    }
                };

                const chart = Highcharts.chart('container', chartOptions);

                setInterval(() => {
                    fetch('/circularBufferData')
                        .then(response => response.json())
                        .then(newData => {
                            const newTimestamps = [];
                            const newValues = newData.data;
                            let newCurrentTimestamp = Date.now() - (newData.lastTimestamp - newData.end * intervalDuration);
                            newValues.forEach((value, index) => {
                                newTimestamps.push(newCurrentTimestamp + index * intervalDuration);
                            });

                            chart.series[0].setData(newValues.map((value, index) => [newTimestamps[index], value]), true);
                        })
                        .catch(error => console.error('Error fetching new data:', error));
                }, 60000); // Fetch new data every 60 seconds

                // Listener para actualizar el gr├ífico al cambiar el tema
                const themeChangeHandler = () => {
                    chart.update({
                        chart: {
                            backgroundColor: getComputedStyle(document.documentElement).getPropertyValue('--bg-color')
                        },
                        title: {
                            style: {
                                color: getComputedStyle(document.documentElement).getPropertyValue('--title-color')
                            }
                        },
                        xAxis: {
                            labels: {
                                style: {
                                    color: getComputedStyle(document.documentElement).getPropertyValue('--font-color')
                                }
                            }
                        },
                        yAxis: {
                            title: {
                                style: {
                                    color: getComputedStyle(document.documentElement).getPropertyValue('--font-color')
                                }
                            },
                            labels: {
                                style: {
                                    color: getComputedStyle(document.documentElement).getPropertyValue('--font-color')
                                }
                            }
                        },
                        series: [{
                            color: getComputedStyle(document.documentElement).getPropertyValue('--title-color')
                        }],
                        legend: {
                            itemStyle: {
                                color: getComputedStyle(document.documentElement).getPropertyValue('--font-color')
                            }
                        }
                    }, true, false, false);
                };

                document.addEventListener('themeChange', themeChangeHandler);
            })
            .catch(error => console.error('Error fetching data:', error));
    };

    fetchDataAndUpdateChart();
}

document.addEventListener("DOMContentLoaded", function () {
    var currentURL = window.location.href;
    if (currentURL.includes("charts.html")) {
        highlightCurrentPage();
        CreateChart();
    }
});
