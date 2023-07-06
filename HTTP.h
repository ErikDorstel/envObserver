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

class httpClient {
  public:
  EthernetClient httpRequest;
  int responseStatus;
  String responseHeader;
  String responseBody;
  void get(String host,String path="/",String value="") { send("GET",host,path,value); }
  void post(String host,String path="/",String value="") { send("POST",host,path,value); }
  void send(String type,String host,String path,String value) {
    httpRequest.connect(host.c_str(),80);
    httpRequest.println(type + " " + path + " HTTP/1.0");
    httpRequest.println("Host: " + host);
    httpRequest.println("Accept: text/html");
    httpRequest.println("User-Agent: Mozilla/5.0");
    httpRequest.println();
    if (value.length()) { httpRequest.println(value); }
    responseHeader=""; responseBody=""; String line=""; bool isHeader=true; responseStatus=-1;
    while (httpRequest.connected()) { if (httpRequest.available()) {
      char c=httpRequest.read(); line+=c; if (c=='\n') {
        if (line.length()<3) { isHeader=false; }
        else { if (isHeader) { responseHeader+=line; } else { responseBody+=line; } } line=""; } } }
    if (responseHeader.startsWith("HTTP/")) {
      int a=responseHeader.indexOf(" ")+1; int b=responseHeader.indexOf(" ",a); responseStatus=responseHeader.substring(a,b).toInt(); }
    httpRequest.stop(); } };
