#include <Preferences.h>

Preferences flash;

struct calibrationStruct { double peak; double rms; double peakDefault=2.04; double rmsDefault=1.39; } calibration;

void getCalibration() {
  flash.begin("envObserver",true);
  calibration.peak=flash.getDouble("peak",calibration.peakDefault);
  calibration.rms=flash.getDouble("rms",calibration.rmsDefault);
  flash.end();
  if (debug) { Serial.println("Calibration Peak: " + String(calibration.peak,4) + " RMS: " + String(calibration.rms,4)); } }

void setCalibration(double peak, double rms) {
  flash.begin("envObserver",false);
  flash.putDouble("peak",peak*calibration.peak/325);
  flash.putDouble("rms",rms*calibration.rms/230);
  flash.end();
  getCalibration(); }

void resetCalibration() {
  flash.begin("envObserver",false);
  flash.putDouble("peak",calibration.peakDefault);
  flash.putDouble("rms",calibration.rmsDefault);
  flash.end();
  getCalibration(); }
