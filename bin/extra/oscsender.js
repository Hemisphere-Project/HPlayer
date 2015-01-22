var osc = require('node-osc');

var client = new osc.Client('127.0.0.1', 9000);
client.send('/play', 'sintel_720p.mp4');

client = null;
