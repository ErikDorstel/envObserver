String httpServerRequest(String request) {
  String response="";

  if (request.indexOf("/getId")>=0) {
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
    response+=String(!digitalRead(34),BIN) + ",";
    response+=String(!digitalRead(35),BIN) + ",";
    response+=String(!digitalRead(36),BIN) + ",";
    response+=String(!digitalRead(39),BIN); }

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

  else {
    response+="<a href='/getId'>getId</a><br>";
    response+="<a href='/getVoltage'>getVoltage</a><br>";
    response+="<a href='/getVoltageRange'>getVoltageRange</a><br>";
    response+="<a href='/getTemperature'>getTemperature</a><br>";
    response+="<a href='/getPressure'>getPressure</a><br>";
    response+="<a href='/getHumidity'>getHumidity</a><br>";
    response+="<a href='/getDigitalIn'>getDigitalIn</a><br>";
    response+="<a href='/getCalibration'>getCalibration</a><br>";
    response+="<a href='/setCalibration'>setCalibration</a><br>";
    response+="<a href='/resetCalibration'>resetCalibration</a><br>"; }

  resetMeasure();
  return response; }
