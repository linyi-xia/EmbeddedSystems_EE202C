
var querystring = require('querystring');
var http = require('http');

var data = querystring.stringify({
	Car_ID: 1003,
	Beacon_ID: 2003,
	Status:4
    });

var options = {
	host: '169.232.190.193',
	port: 8080,
	path: '/update',
	method: 'POST',
	headers: {
		'Content-Type': 'application/x-www-form-urlencoded',
		'Content-Length': Buffer.byteLength(data)
	}
};

var req = http.request(options, function(res) {
	res.setEncoding('utf8');
	res.on('data', function (chunk) {
		console.log("body: " + chunk);
	});
});

req.write(data);
req.end();
