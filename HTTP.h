// change C:\Users\xxxx\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.9\cores\esp32\Server.h
// "virtual void begin(uint16_t port=0) =0;" to "virtual void begin() =0;"

EthernetServer tcpServer(80);

#include "httpget.h"

void initHTTP() { tcpServer.begin(); }

void httpWorker() {
  EthernetClient httpClient=tcpServer.available(); String header="";
  if (httpClient) { String currentLine="";
    while (httpClient.connected()) {
      if (httpClient.available()) { char c=httpClient.read(); header+=c;
        if (c=='\n') {
          if (currentLine.length()==0) {
            httpClient.println("HTTP/1.1 200 OK");
            httpClient.println("Content-Type: text/html; charset=utf-8");
            httpClient.println("Connection: keep-alive");
            httpClient.println("Keep-Alive: timeout=5, max=1000");
            httpClient.println("Server: ESP32 wlanSkeleton Erik Dorstel");
            httpClient.println();
            int a=header.indexOf("GET "); int b=header.indexOf(" ",a+4);
            if (a>=0) { httpClient.println(httpget(header.substring(a+4,b)));
              if (debug) { Serial.println("HTTP Request " + header.substring(a+4,b) + " from " + httpClient.remoteIP().toString() + " received."); } }
            break; }
          else { currentLine=""; } }
        else if (c!='\r') { currentLine+=c; } } }
    header = ""; httpClient.stop(); } }
