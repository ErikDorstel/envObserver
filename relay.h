#define relayA 4
#define relayB 13
#define relayC 32
#define relayD 33

struct relayStruct { bool state[4]; } relay;

void setRelay(int channel, bool state) {
  if (channel==0 && state==false) { digitalWrite(relayA,LOW); relay.state[0]=state; if (debug) { Serial.println("Relay Chan: 0, State: Off"); } }
  if (channel==0 && state==true) { digitalWrite(relayA,HIGH); relay.state[0]=state; if (debug) { Serial.println("Relay Chan: 0, State: On"); } }
  if (channel==1 && state==false) { digitalWrite(relayB,LOW); relay.state[1]=state; if (debug) { Serial.println("Relay Chan: 1, State: Off"); } }
  if (channel==1 && state==true) { digitalWrite(relayB,HIGH); relay.state[1]=state; if (debug) { Serial.println("Relay Chan: 1, State: On"); } }
  if (channel==2 && state==false) { digitalWrite(relayC,LOW); relay.state[2]=state; if (debug) { Serial.println("Relay Chan: 2, State: Off"); } }
  if (channel==2 && state==true) { digitalWrite(relayC,HIGH); relay.state[2]=state; if (debug) { Serial.println("Relay Chan: 2, State: On"); } }
  if (channel==3 && state==false) { digitalWrite(relayD,LOW); relay.state[3]=state; if (debug) { Serial.println("Relay Chan: 3, State: Off"); } }
  if (channel==3 && state==true) { digitalWrite(relayD,HIGH); relay.state[3]=state; if (debug) { Serial.println("Relay Chan: 3, State: On"); } } }

void initRelay() {
  pinMode(relayA,OUTPUT); setRelay(0,false);
  pinMode(relayB,OUTPUT); setRelay(1,false);
  pinMode(relayC,OUTPUT); setRelay(2,false);
  pinMode(relayD,OUTPUT); setRelay(3,false); }
