#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <Adafruit_BME280.h>
#include "calibrate.h"

#define inputA 34
#define inputB 35
#define inputC 36
#define inputD 39
#define bme280SDA 17
#define bme280SCL 16
#define ads1115Int 25

Adafruit_ADS1115 ads1115;
TwoWire I2C2=TwoWire(1);
Adafruit_BME280 bme280;

httpClient httpClientRequest;

struct envRangeStruct {
  uint16_t measures;
  double voltagePeakMin;
  double voltagePeakMax;
  double voltageRmsMin;
  double voltageRmsMax;
  double frequencyMin;
  double frequencyMax; } envRange;

void resetEnvRange() {
  envRange.measures=0;
  envRange.voltagePeakMin=1000;
  envRange.voltagePeakMax=0;
  envRange.voltageRmsMin=1000;
  envRange.voltageRmsMax=0;
  envRange.frequencyMin=100;
  envRange.frequencyMax=0; }

struct envStruct {
  double voltagePeak;
  double voltagePeakMin;
  double voltagePeakMax;
  double voltageRms;
  double voltageRmsMin;
  double voltageRmsMax;
  double frequency;
  double frequencyMin;
  double frequencyMax; } env;

void resetEnv() {
  env.voltagePeakMin=1000;
  env.voltagePeakMax=0;
  env.voltageRmsMin=1000;
  env.voltageRmsMax=0;
  env.frequencyMin=100;
  env.frequencyMax=0; }

struct measureStruct {
  hw_timer_t* phaseTimer=NULL;
  uint64_t timer;
  int16_t rawCur;
  int16_t rawMax;
  int64_t rawSum;
  int counter;
  int polarity;
  int phases;
  uint64_t phaseDuration; } measure;

void resetMeasure() {
  measure.timer=millis()+5000;
  measure.rawMax=-32768;
  measure.rawSum=0;
  measure.counter=0;
  measure.polarity=0;
  measure.phases=0; }

volatile bool newData=false;
void IRAM_ATTR newDataISR() { newData=true; }

void initMeasure() {

  // hardware timer 0
  measure.phaseTimer=timerBegin(0,80,true);
  timerStart(measure.phaseTimer);

  // ads1115 voltage adc
  if (!ads1115.begin()) { Serial.println("Failed to initialize ADS1115."); }
  ads1115.setGain(GAIN_TWO); ads1115.setDataRate(RATE_ADS1115_860SPS);
  pinMode(ads1115Int,INPUT); attachInterrupt(ads1115Int,newDataISR,FALLING);
  ads1115.startADCReading(ADS1X15_REG_CONFIG_MUX_SINGLE_0,true);

  // bme280 environment sensor
  I2C2.begin(bme280SDA,bme280SCL);
  if (!bme280.begin(0x76,&I2C2)) { Serial.println("Failed to initialize BME280."); }

  // ltc845 optocoupler digital input
  pinMode(inputA,INPUT); pinMode(inputB,INPUT); pinMode(inputC,INPUT); pinMode(inputD,INPUT);

  getCalibration(); resetEnv(); resetEnvRange(); resetMeasure(); }

void measureWorker() {

  if (newData) {
    newData=false; measure.counter++;
    measure.rawCur=ads1115.getLastConversionResults();
    measure.rawSum+=measure.rawCur;
    if (measure.rawCur>measure.rawMax) { measure.rawMax=measure.rawCur; }
    if (measure.rawCur>=16000 && measure.polarity<=0) { measure.polarity=1;
      measure.phases++; if (measure.phases>2) { measure.phaseDuration=timerRead(measure.phaseTimer); } else { timerWrite(measure.phaseTimer,0x0ULL); } }
    if (measure.rawCur<16000 && measure.polarity>=0) { measure.polarity=-1; } }

  if (millis()>=measure.timer) {
    envRange.measures++;
    double voltagePeak=ads1115.computeVolts(measure.rawMax)+0.6;
    double voltageRms=ads1115.computeVolts(measure.rawSum/measure.counter)+0.6;
    double frequency=0; if (measure.phases>2) { frequency=500000/((double)measure.phaseDuration/(measure.phases-2)); }
    env.voltagePeak=voltagePeak*325/calibration.peak;
    if (env.voltagePeak<env.voltagePeakMin) { env.voltagePeakMin=env.voltagePeak; }
    if (env.voltagePeak>env.voltagePeakMax) { env.voltagePeakMax=env.voltagePeak; }
    if (env.voltagePeak<envRange.voltagePeakMin) { envRange.voltagePeakMin=env.voltagePeak; }
    if (env.voltagePeak>envRange.voltagePeakMax) { envRange.voltagePeakMax=env.voltagePeak; }
    env.voltageRms=voltageRms*230/calibration.rms;
    if (env.voltageRms<env.voltageRmsMin) { env.voltageRmsMin=env.voltageRms; }
    if (env.voltageRms>env.voltageRmsMax) { env.voltageRmsMax=env.voltageRms; }
    if (env.voltageRms<envRange.voltageRmsMin) { envRange.voltageRmsMin=env.voltageRms; }
    if (env.voltageRms>envRange.voltageRmsMax) { envRange.voltageRmsMax=env.voltageRms; }
    env.frequency=frequency;
    if (env.frequency<env.frequencyMin) { env.frequencyMin=env.frequency; }
    if (env.frequency>env.frequencyMax) { env.frequencyMax=env.frequency; }
    if (env.frequency<envRange.frequencyMin) { envRange.frequencyMin=env.frequency; }
    if (env.frequency>envRange.frequencyMax) { envRange.frequencyMax=env.frequency; }

    int inputs=((!digitalRead(inputA))*1)+((!digitalRead(inputB))*2)+((!digitalRead(inputC))*4)+((!digitalRead(inputD))*8);
    String update="update=" + String(env.voltagePeak) + "," + String(env.voltageRms) + "," + String(env.frequency);
    update+="," + String(bme280.readTemperature()) + "," + String(bme280.readPressure()/100) + "," + String(bme280.readHumidity()) + "," + String(inputs);
    if (envRange.measures>=120) {
      update+="," + String(envRange.voltagePeakMin) + "," + String(envRange.voltagePeakMax);
      update+="," + String(envRange.voltageRmsMin) + "," + String(envRange.voltageRmsMax);
      update+="," + String(envRange.frequencyMin) + "," + String(envRange.frequencyMax); }
    httpClientRequest.post(httpHost,httpPath,update);
    if (envRange.measures>=120 && httpClientRequest.responseStatus==200) { resetEnvRange(); }

    if (debug) {
      Serial.print(env.voltagePeak); Serial.print(" - ");
      Serial.print(env.voltageRms); Serial.print(" - ");
      Serial.print(env.frequency); Serial.println(); }

    resetMeasure(); } }
