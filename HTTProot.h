String httpServerRequest(String request) {
  String response="";

  if (request.indexOf("/getStatus")>=0) {
    response+=String(ESP.getEfuseMac(),HEX) + ",";
    response+=String(env.voltagePeakMin,2) + ",";
    response+=String(env.voltagePeakMax,2) + ",";
    response+=String(env.voltageRmsMin,2) + ",";
    response+=String(env.voltageRmsMax,2) + ",";
    response+=String(env.frequencyMin,2) + ",";
    response+=String(env.frequencyMax,2) + ",";
    response+=String(bme280.readTemperature(),2) + ",";
    response+=String(bme280.readPressure()/100,2) + ",";
    response+=String(bme280.readHumidity(),2) + ",";
    response+=String(!digitalRead(inputA),BIN) + ",";
    response+=String(!digitalRead(inputB),BIN) + ",";
    response+=String(!digitalRead(inputC),BIN) + ",";
    response+=String(!digitalRead(inputD),BIN) + ",";
    response+=String(relay.state[0]) + ",";
    response+=String(relay.state[1]) + ",";
    response+=String(relay.state[2]) + ",";
    response+=String(relay.state[3]);
    resetEnv(); }

  else if (request.indexOf("/getId")>=0) {
    response+=String(ESP.getEfuseMac(),HEX); }

  else if (request.indexOf("/getVoltageRange")>=0) {
    response+=String(env.voltagePeakMin,2) + ",";
    response+=String(env.voltagePeakMax,2) + ",";
    response+=String(env.voltageRmsMin,2) + ",";
    response+=String(env.voltageRmsMax,2) + ",";
    response+=String(env.frequencyMin,2) + ",";
    response+=String(env.frequencyMax,2);
    resetEnv(); }

  else if (request.indexOf("/getVoltage")>=0) {
    response+=String(env.voltagePeak,2) + ",";
    response+=String(env.voltageRms,2) + ",";
    response+=String(env.frequency,2); }

  else if (request.indexOf("/getTemperature")>=0) {
    response+=String(bme280.readTemperature(),2); }

  else if (request.indexOf("/getPressure")>=0) {
    response+=String(bme280.readPressure()/100,2); }

  else if (request.indexOf("/getHumidity")>=0) {
    response+=String(bme280.readHumidity(),2); }

  else if (request.indexOf("/getDigitalIn")>=0) {
    response+=String(!digitalRead(inputA),BIN) + ",";
    response+=String(!digitalRead(inputB),BIN) + ",";
    response+=String(!digitalRead(inputC),BIN) + ",";
    response+=String(!digitalRead(inputD),BIN); }

  else if (request.indexOf("/getCalibration")>=0) {
    response+=String(calibration.peak,4) + ",";
    response+=String(calibration.rms,4); }

  else if (request.indexOf("/setCalibration")>=0) {
    setCalibration(env.voltagePeak,env.voltageRms);
    response+=String(calibration.peak,4) + ",";
    response+=String(calibration.rms,4); }

  else if (request.indexOf("/resetCalibration")>=0) {
    resetCalibration();
    response+=String(calibration.peak,4) + ",";
    response+=String(calibration.rms,4); }

  else if (request.indexOf("/setRelay")>=0) {
    int a=request.indexOf(",")+1; int b=request.indexOf(",",a)+1;
    if (a>0 && b>0) {
      int channel=request.substring(a,b-1).toInt(); int state=request.substring(b).toInt();
      setRelay(channel,state); }
    response+=String(relay.state[0]) + ",";
    response+=String(relay.state[1]) + ",";
    response+=String(relay.state[2]) + ",";
    response+=String(relay.state[3]); }

  else if (request.indexOf("/getRelay")>=0) {
    response+=String(relay.state[0]) + ",";
    response+=String(relay.state[1]) + ",";
    response+=String(relay.state[2]) + ",";
    response+=String(relay.state[3]); }

  else if (request.indexOf("/favicon.ico")>=0) { }

  else {
    response+="<a href='/getStatus'>getStatus</a><br>";
    response+="<a href='/getId'>getId</a><br>";
    response+="<a href='/getVoltage'>getVoltage</a><br>";
    response+="<a href='/getVoltageRange'>getVoltageRange</a><br>";
    response+="<a href='/getTemperature'>getTemperature</a><br>";
    response+="<a href='/getPressure'>getPressure</a><br>";
    response+="<a href='/getHumidity'>getHumidity</a><br>";
    response+="<a href='/getDigitalIn'>getDigitalIn</a><br>";
    response+="<a href='/getCalibration'>getCalibration</a><br>";
    response+="<a href='/setCalibration'>setCalibration</a><br>";
    response+="<a href='/resetCalibration'>resetCalibration</a><br>";
    response+="<a href='/setRelay,0,0'>setRelay,0,0</a><br>";
    response+="<a href='/setRelay,0,1'>setRelay,0,1</a><br>";
    response+="<a href='/setRelay,1,0'>setRelay,1,0</a><br>";
    response+="<a href='/setRelay,1,1'>setRelay,1,1</a><br>";
    response+="<a href='/setRelay,2,0'>setRelay,2,0</a><br>";
    response+="<a href='/setRelay,2,1'>setRelay,2,1</a><br>";
    response+="<a href='/setRelay,3,0'>setRelay,3,0</a><br>";
    response+="<a href='/setRelay,3,1'>setRelay,3,1</a><br>";
    response+="<a href='/getRelay'>getRelay</a><br>"; }

  resetMeasure();
  return response; }
