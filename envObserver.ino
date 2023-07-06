bool debug=true;

#include "HTTPclient.h"
#include "measure.h"
#include "Eth.h"
#include "HTTPserver.h"

void setup() {
  Serial.begin(115200);
  initMeasure();
  initEth();
  initHTTPServer(); }

void loop() { measureWorker(); ethWorker(); httpServerWorker(); }
