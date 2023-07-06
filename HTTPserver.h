// change C:\Users\xxxx\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.9\cores\esp32\Server.h
// "virtual void begin(uint16_t port=0) =0;" to "virtual void begin() =0;"

EthernetServer tcpServer(80);

#include "HTTProot.h"

void initHTTPServer() { tcpServer.begin(); }

void httpServerWorker() {
  EthernetClient httpServerClient=tcpServer.available(); String header="";
  if (httpServerClient) { String currentLine="";
    while (httpServerClient.connected()) {
      if (httpServerClient.available()) { char c=httpServerClient.read(); header+=c;
        if (c=='\n') {
          if (currentLine.length()==0) {
            httpServerClient.println("HTTP/1.1 200 OK");
            httpServerClient.println("Content-Type: text/html; charset=utf-8");
            httpServerClient.println("Connection: keep-alive");
            httpServerClient.println("Keep-Alive: timeout=5, max=1000");
            httpServerClient.println("Server: ESP32 wlanSkeleton Erik Dorstel");
            httpServerClient.println();
            int a=header.indexOf("GET "); int b=header.indexOf(" ",a+4);
            if (a>=0) { httpServerClient.println(httpServerRequest(header.substring(a+4,b)));
              if (debug) { Serial.println("HTTP Request " + header.substring(a+4,b) + " from " + httpServerClient.remoteIP().toString() + " received."); } }
            break; }
          else { currentLine=""; } }
        else if (c!='\r') { currentLine+=c; } } }
    header = ""; httpServerClient.stop(); } }
