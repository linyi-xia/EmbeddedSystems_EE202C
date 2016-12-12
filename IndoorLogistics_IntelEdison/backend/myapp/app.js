
var express = require('express');
var path = require('path');
var favicon = require('serve-favicon');
var logger = require('morgan');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');
var sqlite3 = require('sqlite3').verbose();
var db = new sqlite3.Database("locationsNroutes.db",'OPEN_READWRITE');

var routes = require('./routes/index');
var users = require('./routes/users');
var about = require('./routes/about');
var viewdata = require('./routes/viewdata');
var http = require('http');

var path_to_view = path.join(__dirname,'/views/');

var app = express();
var router = express.Router();
var server = require('http').createServer(app);
var io = require('socket.io')(server);
var port = process.env.PORT || 8080;
var server = app.listen(port);
var query_1001 = "SELECT * FROM car_locations where Car_ID = 1001";
var result = -1;

// create application/x-www-form-urlencoded parser
var urlencodedParser = bodyParser.urlencoded({ extended: true })
var update_ID = -1;
var update_status = -1;
var update_time = -1;
var update_location = -1;

console.log('Server started! At http://localhost: '+port);
//var io = require('socket.io').listen(server);
//added

server.listen(3001);

db.serialize(function(){
//******* Example of how to print the entire table
//	db.each("SELECT * FROM car_locations", function(err,row){
//		if (err)
//			console.log(err);
//		else
//			console.log(row);
//	
//	});
// end of example 

	db.each(query_1001, function(err, row){
		console.log("query_1001: "+query_1001);
		console.log("Sample return console log row.status: " + row.Status);
		console.log("Sample return console log row: "+row.Beacon_ID);
	});

});

router.use(function (req,res,next) {
  console.log("/" + req.method);
  next();
});

//io.on('connection',function(socket){
//	socket.emit('news',{hello: 'world'});
//	console.log("connected");
//	socket.on('my other event',function(data){
//		console.log(data);
//	});
//});
//end added

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'jade');
app.get('/sample',function(req, res){
	res.send('this is a sample');
});


// HTTP GET API interfacing database
app.get('/database',function(req,res){
	console.log("database query");
	var query_id = req.query.Car_ID;
	var stmt = "SELECT * FROM car_locations ";
	var add_id = "WHERE Car_ID = " + query_id;
	stmt = stmt+add_id;
	db.each(stmt, function(err,row){
		if (err)
			console.log(err);
		else{
			console.log("Query Status: "+ row.Status);
			result = row;
			
		}
	}, function(){
		res.json(result);
//		res.send('request database with ID = '+ query_id + " Status is " + result);
	});
});

// HTTP POST API interfacing database
app.post('/update', urlencodedParser, function(req,res){
	if (!req.body) return res.sendStatus(400)
	console.log("In POST request");
	console.log("POST req body: "+JSON.stringify(req.body));
	update_ID = req.body.Car_ID;
	update_status = req.body.Status;
	update_location = req.body.Beacon_ID;
	update_time = Math.floor( (new Date).getTime()/1000);
	
	//prepare for updating
	var stmt = "UPDATE car_locations SET Beacon_ID="+update_location+ ", Status = "+update_status + " WHERE Car_ID = "+update_ID;	
	console.log("update statment: " + stmt);

	//update after completing job
	db.each(stmt, function(err,row){
                if (err)
                        console.log("encountered error: "+err);
                else{
                        console.log("UPDATED Status: "+row.Status);
                }
	}, function(){
//		db.run("COMMIT",function(err,row){
//                                if (err)
//                                        console.log("encountered error in commit: "+err);
//                                else{
//                                        console.log("Commit Succeeded");
//                                }
//                        });
                console.log("database updated on ID = " + update_ID);
		res.send("200");
        });
}, function(err){
	console.log("HTTP POST error: "+err);
});


// uncomment after placing your favicon in /public
//app.use(favicon(path.join(__dirname, 'public', 'favicon.ico')));
app.use(logger('dev'));

// parse application/x-www-form-urlencoded
app.use(bodyParser.urlencoded({ extended: true }))

// parse application/json
app.use(bodyParser.json())

app.use(function (req, res, next) {
  console.log('Router Time:', Date.now());
  //res.setHeader('Content-Type', 'text/plain')
  //res.write('you posted:\n')
  //res.end(JSON.stringify(req.body, null, 2))
  next();	
});

app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use('/', routes);
app.use('/about', about);
app.use('/users', users);
app.use('/viewdata',viewdata);

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  var err = new Error('Not Found');
  err.status = 404;
  next(err);
});

// error handlers

// development error handler
// will print stacktrace


if (app.get('env') === 'development') {
  app.use(function(err, req, res, next) {
    res.status(err.status || 500);
    res.render('error', {
      message: err.message,
      error: err
    });
  });
}

// production error handler
// no stacktraces leaked to user
app.use(function(err, req, res, next) {
  res.status(err.status || 500);
  res.render('error', {
    message: err.message,
    error: {}
  });
});


module.exports = app;

//****** destructor of object do not use till exit
//db.close();
