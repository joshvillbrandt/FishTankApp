function dataStats(idBase, data) {
    var min = 200, max = -200, sum = 0, avg = 0;
    
    for(var i = 0; i < data.length; i++) {
        min = Math.min(min, data[i][1]);
        max = Math.max(max, data[i][1]);
        sum += data[i][1];
    }
    
    // find average
    // this average is total BS if the sampling rate isn't consistent!
    if(data.length > 0)
        var avg = sum / data.length;
    
    $('#'+idBase+'-min').html(min.toFixed(2));
    $('#'+idBase+'-avg').html(avg.toFixed(2));
    $('#'+idBase+'-max').html(max.toFixed(2));
}

$(document).ready(function() {
    
    // get data
    var args = {days: 1};
    $.getJSON('/json/getData/?callback=?', args, function(data, textStatus, jqXHR) {
        // highcharts assumes date are given in local timezone...stupid
        // could maybe also use the plotOptions.line.pointState feature
        var localOffset = (new Date().getTimezoneOffset()) * 60000;
        
        // build chart data arrays
        tTank = [];
        tAmbient = [];
        lTank = [];
        for(var i = 0; i < data.data.length; i++) {
            var d = new Date(data.data[i]['date_logged']).getTime() - localOffset;
            tTank.push([d, data.data[i]['temp_tank']]);
            tAmbient.push([d, data.data[i]['temp_ambient']]);
            lTank.push([d, data.data[i]['light_ambient']]);
        }

        // datalog chart
        $('#chart').highcharts({
            chart: {
                spacingTop: 40
            },
            title: {
                text: null
            },
            xAxis: {
                type: 'datetime'
            },
            yAxis: [{
                title: {
                    text: 'Temperature (°F)'
                },
                plotLines: [{
                    value: 0,
                    width: 1,
                    color: '#808080'
                }]
            }, {
                title: {
                    text: 'Light (counts)',
                    style: {
                        color: '#8bbc21'
                    }
                },
                labels: {
                    enabled: false
                },
                opposite: true
            }],
            legend: {
                floating: true,
                align: 'left',
                verticalAlign: 'top',
                borderWidth: 0,
                y: -40
            },
            plotOptions: {
                line: {
                    marker: {
                        enabled: false
                    }
                }
            },
            series: [{
                name: 'Tank Temp',
                data: tTank,
                tooltip: {
                    valueSuffix: '°F'
                }
            }, {
                name: 'Ambient Temp',
                data: tAmbient,
                tooltip: {
                    valueSuffix: '°F'
                }
            }, {
                name: 'Tank Light',
                yAxis: 1,
                data: lTank,
                tooltip: {
                    valueSuffix: ' counts'
                }
            }],
            credits: {
                enabled: false
            }
        });
        
        // data stats
        dataStats('ttank', tTank);
        dataStats('tambient', tAmbient);
        
    }); // end getJSON
    
}); // end document ready
