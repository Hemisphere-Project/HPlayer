var osc = require('node-osc');

var oscServer = new osc.Server(9001, '0.0.0.0');
oscServer.on("message", function (msg, rinfo) {
      console.log("TUIO message:");
      console.log(msg);
});

var client = new osc.Client('127.0.0.1', 9000);

setTimeout( function() {
	client.send('/getStatus', '');
}, 10000);
