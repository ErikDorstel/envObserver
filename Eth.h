#include <SPI.h>
#include <Ethernet.h>

uint8_t ethMAC[6];

void initEth() {
  Ethernet.init(5);
  uint64_t espid=ESP.getEfuseMac();
  if (debug) { Serial.println("ESP ID: " + String(espid,HEX)); }
  // WiFiSTA: espid+0, WiFiAP: espid+1, Bluetooth: espid+2, Ethernet: espid+3
  espid+=3;
  if (debug) { Serial.println("Ethernet MAC: " + String(espid,HEX)); }
  ethMAC[0]=espid>>40&255; ethMAC[1]=espid>>32&255; ethMAC[2]=espid>>24&255;
  ethMAC[3]=espid>>16&255; ethMAC[4]=espid>>8&255; ethMAC[5]=espid>>0&255; }

void ethWorker() {
  static uint64_t ethTimer=millis()+200; static bool ethStatus=false;
  if (millis()>=ethTimer) { ethTimer=millis()+10000;
    if (Ethernet.linkStatus()==LinkON && ethStatus==false) { ethStatus=true; if (debug) { Serial.println("Ethernet Link: On"); } }
    if (Ethernet.linkStatus()==LinkOFF && ethStatus==true) { ethStatus=false; if (debug) { Serial.println("Ethernet Link: Off"); } }
    if (Ethernet.linkStatus()==LinkON && Ethernet.localIP()==IPAddress(0,0,0,0)) {
      if (ethDHCP) { Ethernet.begin(ethMAC); if (debug) { Serial.println("Ethernet DHCP IP: " + Ethernet.localIP().toString()); } }
      else { Ethernet.begin(ethMAC,ethIP,ethDNS,ethGW,ethNM); if (debug) { Serial.println("Ethernet Static IP: " + Ethernet.localIP().toString()); } }
      resetMeasure(); }
    if (ethDHCP) { Ethernet.maintain(); } } }
