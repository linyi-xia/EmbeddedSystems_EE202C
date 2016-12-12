
var express = require('express');
var router = express.Router();
var path = require('path');
var path_to_view = path.join(__dirname,'../views/');

// middleware that is specific to this router
router.use(function timeLog(req, res, next) {
  console.log('/routes/viewpage.js Time: ', Date.now());
  next();
});

/* GET About page. */
router.get('/', function(req, res) {
  console.log("inside of viewpage.js router");
  res.render('viewdata', { title: 'The Last "Mile"', message: 'About dat team....'});

});

module.exports = router;

