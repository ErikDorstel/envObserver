bool debug=true;
bool ethDHCP=true;
uint8_t ethIP[]={192,168,100,100};
uint8_t ethGW[]={192,168,100,1};
uint8_t ethDNS[]={192,168,100,1};
uint8_t ethNM[]={255,255,255,0};
String httpHost="office.dorstel.de";
String httpPath="/receiver2.php";

#include "HTTPclient.h"
#include "relay.h"
#include "measure.h"
#include "Eth.h"
#include "HTTPserver.h"

void setup() {
  Serial.begin(115200);
  initRelay();
  initMeasure();
  initEth();
  initHTTPServer(); }

void loop() { measureWorker(); ethWorker(); httpServerWorker(); }
