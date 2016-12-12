var bleno = require('./index');

console.log('bleno - iBeacon');
bleno.on('advertisingStart', function() {
  console.log('on -> advertisingStart');
});

bleno.on('stateChange', function(state) {
  console.log('on -> stateChange: ' + state);

 

    setInterval(function(){
	if (state === 'poweredOn'){
    		bleno.startAdvertisingIBeacon('202C1db5dffb48d2b060d0f5a71096e0', 0, 1, -59);
	}
	else{
		bleno.stopAdvertising();
		//break;
	}

	},2000);
  

if (state !== 'poweredOn'){
    bleno.stopAdvertising();
  }
});

bleno.on('advertisingStart', function() {
  console.log('on -> advertisingStart');
});

bleno.on('advertisingStop', function() {
  console.log('on -> advertisingStop');
});
