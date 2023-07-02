bool debug=true;

#include "measure.h"
#include "Eth.h"
#include "HTTP.h"

void setup() {
  Serial.begin(115200);
  initMeasure();
  initEth();
  initHTTP(); }

void loop() { measureWorker(); ethWorker(); httpWorker(); }
