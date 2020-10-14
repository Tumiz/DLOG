// Copyright (c) Tumiz.
// Distributed under the terms of the GPL-3.0 License.
var datapool = {}
var color = 1000

class LogChart extends Chart{
    constructor(){
        var ctx = document.getElementById('chart').getContext('2d');
        var cfg = {
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
        }
        super(ctx, cfg)
        this.datasets=cfg.data.datasets
        this.timeFormat="ns"
        this.startTime=0
        this.t0=0
    }
    ns(pair){
        pair.x = pair.xbak - this.startTime
    }
    ms(pair){
        pair.x = (pair.xbak - this.startTime)/1e6 
    }
    us(pair){
        pair.x = (pair.xbak - this.startTime)/1e3
    }
    s(pair){
        pair.x = (pair.xbak - this.startTime)/1e9
    }
    forceUpdate(){
        for(var i = 0, il = this.datasets.length; i < il; i++){
            var dataset = this.datasets[i].data
            for(var j=0, jl = dataset.length; j < jl; j++){
                this[this.timeFormat](dataset[j])
            }
        }
        this.update()
    }
    setTimeFormat(f){
        this.timeFormat = f
        this.forceUpdate()
    }
    startFromZero(enable){
        this.startTime = enable ? this.t0 : 0
        this.forceUpdate()
    }
    add(t, v){
        
    }
}
var chart = new LogChart()
var ws = new WebSocket("ws://127.0.0.1:8000/viewer")
ws.onopen = function (ev) {
    console.log(ev)
}
ws.onmessage = function (ev) {
    var ss = ev.data.split(',')
    var n = ss[0]
    var t = parseFloat(ss[1])
    var v = parseFloat(ss[2])
    if(chart.t0==0){
        chart.t0=t
    }
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
        chart.datasets.push(series)
        color+=10000
    }
    var pair = {xbak:t,x:t,y:v}
    chart[chart.timeFormat](pair)
    series.data.push(pair)
    chart.update()
}