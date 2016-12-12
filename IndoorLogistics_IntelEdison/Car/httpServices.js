var http = require('http');
var ip = '131.179.52.64';
var adddatabase = '/database';
var add_queryID = '?ID=';
var port=8080;
const thisCarID = 1001;
var url = 'http://'+ip+":"+port+adddatabase+add_queryID+thisCarID;
console.log("URL: "+url);
var path = adddatabase+add_queryID+thisCarID;
var XMLHttpRequest = require("xmlhttprequest").XMLHttpRequest;
//var $=require('jquery');
var $ = require('jquery/dist/jquery')(window);

var options = {
	hostname: ip,
	port: '8080',
	path: '/database?ID=1001',
	method: 'GET',
};

var html='';

http.get(options, function(res) {
    res.on('data', function(data) {
        // collect the data chunks to the variable named "html"
        html += data;
    }).on('end', function() {
        // the whole of webpage data has been collected. parsing time!
        var title = $(html).find('title').text();
        console.log(title);
     });
});
// http.get(options, function (err, res) {
// 	if (err) {
// 		console.error(err);
// 		return;
// 	}
	
// 	console.log(res.code, res.headers, res.buffer.toString());
// });

// var req = http.request(url, function(res) {
//   console.log('STATUS: ' + res.statusCode);
//   console.log('HEADERS: ' + JSON.stringify(res.headers));
//   res.setEncoding('utf8');
//   res.on('data', function (chunk) {
//     console.log('BODY: ' + chunk);
//   });
//   res.on('end', function() {
//     console.log('No more data in response.')
//   })
// });

// req.on('error', function(e) {
//   console.log('problem with request: ' + e.message);
// });

// write data to request body
//req.write(postData);
//req.end();

