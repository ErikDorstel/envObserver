#include <Preferences.h>

Preferences flash;

struct calibrationStruct { double peak; double rms; double peakDefault=112; double rmsDefault=248; } calibration;

void getCalibration() {
  flash.begin("envObserver",true);
  calibration.peak=flash.getDouble("peak",calibration.peakDefault);
  calibration.rms=flash.getDouble("rms",calibration.rmsDefault);
  flash.end();
  if (debug) { Serial.println("Calibration Peak: " + String(calibration.peak,4) + " RMS: " + String(calibration.rms,4)); } }

void setCalibration(double voltagePeak, double voltageRMS) {
  flash.begin("envObserver",false);
  flash.putDouble("peak",calibration.peak/voltagePeak*325);
  flash.putDouble("rms",calibration.rms/voltageRMS*230);
  flash.end();
  getCalibration(); }

void resetCalibration() {
  flash.begin("envObserver",false);
  flash.putDouble("peak",calibration.peakDefault);
  flash.putDouble("rms",calibration.rmsDefault);
  flash.end();
  getCalibration(); }
