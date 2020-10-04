// Copyright (c) Tumiz.
// Distributed under the terms of the GPL-3.0 License.
var datapool = {}
var color = 1000
var ctx = document.getElementById('chart').getContext('2d');
var chart = new Chart(ctx, {
    type: 'line',
    data: {
        datasets: []
    },
    options: {
        animation: {
            duration: 0 // general animation time
        },
        scales: {
            xAxes: [{
                type:'linear'
            }],
            yAxes: [{
                ticks: {
                    beginAtZero: true,
                }
            }]
        }
    }
});
var ws = new WebSocket("ws://127.0.0.1:8000/viewer")
ws.onopen = function (ev) {
    console.log(ev)
}
ws.onmessage = function (ev) {
    console.log(ev.data)
    var ss = ev.data.split(',')
    var n = ss[0]
    var t = parseFloat(ss[1])
    var v = parseFloat(ss[2])
    var series = datapool[n]
    if(!series){
        series={
            fill: false,
            label: n,
            lineTension: 0,
            data: [],
            borderColor: "#"+color.toString(16)
        }
        datapool[n]=series
        chart.config.data.datasets.push(series)
        color+=10000
    }    
    series.data.push({x:t,y:v})
    chart.update()
}