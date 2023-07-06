// change C:\Users\xxxx\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.9\cores\esp32\Server.h
// "virtual void begin(uint16_t port=0) =0;" to "virtual void begin() =0;"

#include <SPI.h>
#include <Ethernet.h>

class httpClient {
  public:
  EthernetClient httpClientRequest;
  int responseStatus;
  String responseHeader;
  String responseBody;
  void get(String host,String path="/",String value="") { send("GET",host,path,value); }
  void post(String host,String path="/",String value="") { send("POST",host,path,value); }
  void send(String type,String host,String path,String value) {
    httpClientRequest.connect(host.c_str(),80);
    httpClientRequest.println(type + " " + path + " HTTP/1.0");
    httpClientRequest.println("Host: " + host);
    httpClientRequest.println("Accept: text/html");
    httpClientRequest.println("Content-Type: application/x-www-form-urlencoded");
    httpClientRequest.println("Content-Length: " + String(value.length()));
    httpClientRequest.println("User-Agent: Mozilla/5.0");
    httpClientRequest.println();
    if (value.length()) { httpClientRequest.println(value); }
    responseHeader=""; responseBody=""; String line=""; bool isHeader=true; responseStatus=-1;
    while (httpClientRequest.connected()) { if (httpClientRequest.available()) {
      char c=httpClientRequest.read(); line+=c; if (c=='\n') {
        if (line.length()<3) { isHeader=false; }
        else { if (isHeader) { responseHeader+=line; } else { responseBody+=line; } } line=""; } } }
    if (responseHeader.startsWith("HTTP/")) {
      int a=responseHeader.indexOf(" ")+1; int b=responseHeader.indexOf(" ",a); responseStatus=responseHeader.substring(a,b).toInt(); }
    httpClientRequest.stop(); } };
