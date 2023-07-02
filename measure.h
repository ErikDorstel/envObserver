#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_BME280.h>
#include "calibrate.h"

Adafruit_ADS1115 ads1115;
TwoWire I2C2=TwoWire(1);
Adafruit_BME280 bme280;

struct envStruct {
  double voltagePeak;
  double voltagePeakMin;
  double voltagePeakMax;
  double voltageRms;
  double voltageRmsMin;
  double voltageRmsMax;
  double freq;
  double freqMin;
  double freqMax; } env;

void resetEnv() {
  env.voltagePeakMin=1000;
  env.voltagePeakMax=0;
  env.voltageRmsMin=1000;
  env.voltageRmsMax=0;
  env.freqMin=100;
  env.freqMax=0; }

struct measureStruct {
  uint32_t timer;
  int16_t rawCur;
  int16_t rawMax;
  int64_t rawSum;
  int counter;
  int polarity;
  int phases;
  uint64_t phaseStart;
  uint64_t phaseEnd; } measure;

void resetMeasure() {
  measure.timer=millis()+5000;
  measure.rawMax=-32768;
  measure.rawSum=0;
  measure.counter=0;
  measure.polarity=0;
  measure.phases=0;
  measure.phaseStart=0;
  measure.phaseEnd=0; }

volatile bool newData=false;
void IRAM_ATTR newDataISR() { newData=true; }

void initMeasure() {

  // ads1115 voltage adc
  if (!ads1115.begin()) { Serial.println("Failed to initialize ADS1115."); }
  ads1115.setGain(GAIN_TWO); ads1115.setDataRate(RATE_ADS1115_860SPS);
  pinMode(25,INPUT); attachInterrupt(25,newDataISR,FALLING);
  ads1115.startADCReading(ADS1X15_REG_CONFIG_MUX_SINGLE_0,true);

  // bme280 environment sensor
  I2C2.begin(17,16);
  if (!bme280.begin(0x76,&I2C2)) { Serial.println("Failed to initialize BME280."); }

  // ltc845 optocoupler digital input
  pinMode(34,INPUT); pinMode(35,INPUT); pinMode(36,INPUT); pinMode(39,INPUT);

  getCalibration(); resetEnv(); resetMeasure(); }

void measureWorker() {

  if (newData) {
    newData=false; measure.counter++;
    measure.rawCur=ads1115.getLastConversionResults();
    measure.rawSum+=measure.rawCur;
    if (measure.rawCur>measure.rawMax) { measure.rawMax=measure.rawCur; }
    if (measure.rawCur>=10000 && measure.polarity<=0) { measure.polarity=1;
      measure.phases++; if (measure.phases>2) { measure.phaseEnd=micros(); } else { measure.phaseStart=micros(); } }
    if (measure.rawCur<10000 && measure.polarity>=0) { measure.polarity=-1; } }

  if (millis()>=measure.timer) {
    double voltsPeak=ads1115.computeVolts(measure.rawMax)+0.6;
    double voltsRms=ads1115.computeVolts(measure.rawSum/measure.counter)+0.6;
    double freq=0; if (measure.phases>2) { freq=500000/((double)(measure.phaseEnd-measure.phaseStart)/(measure.phases-2)); }
    env.voltagePeak=voltsPeak*325/calibration.peak;
    if (env.voltagePeak<env.voltagePeakMin) { env.voltagePeakMin=env.voltagePeak; }
    if (env.voltagePeak>env.voltagePeakMax) { env.voltagePeakMax=env.voltagePeak; }
    env.voltageRms=voltsRms*230/calibration.rms;
    if (env.voltageRms<env.voltageRmsMin) { env.voltageRmsMin=env.voltageRms; }
    if (env.voltageRms>env.voltageRmsMax) { env.voltageRmsMax=env.voltageRms; }
    env.freq=freq;
    if (freq<env.freqMin) { env.freqMin=freq; }
    if (freq>env.freqMax) { env.freqMax=freq; }
    if (debug) {
      Serial.print(env.voltagePeak); Serial.print(" - ");
      Serial.print(env.voltageRms); Serial.print(" - ");
      Serial.print(env.freq); Serial.println(); }
    resetMeasure(); } }
