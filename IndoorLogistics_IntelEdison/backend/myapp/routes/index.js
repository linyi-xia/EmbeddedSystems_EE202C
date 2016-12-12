var express = require('express');
var router = express.Router();
var path = require('path');
var path_to_view = path.join(__dirname,'../views/');

// middleware that is specific to this router
router.use(function timeLog(req, res, next) {
  console.log('/routes/index.js Time: ', Date.now());
  next();
});

/* GET home page. */
router.get('/', function(req, res, next) {
  console.log("inside of index.js router");
  res.render('index', { title: 'The Last "Mile"', message: 'Hello there!'});

});

module.exports = router;
