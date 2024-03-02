#define relayA 32
#define relayB 33

struct relayStruct { bool state[2]; } relay;

void setRelay(int channel, bool state) {
  if (channel==0 && state==false) { digitalWrite(relayA,LOW); relay.state[0]=state; if (debug) { Serial.println("Relay Chan: 0, State: Off"); } }
  if (channel==0 && state==true) { digitalWrite(relayA,HIGH); relay.state[0]=state; if (debug) { Serial.println("Relay Chan: 0, State: On"); } }
  if (channel==1 && state==false) { digitalWrite(relayB,LOW); relay.state[1]=state; if (debug) { Serial.println("Relay Chan: 1, State: Off"); } }
  if (channel==1 && state==true) { digitalWrite(relayB,HIGH); relay.state[1]=state; if (debug) { Serial.println("Relay Chan: 1, State: On"); } } }

void initRelay() {
  pinMode(relayA,OUTPUT); setRelay(0,false);
  pinMode(relayB,OUTPUT); setRelay(1,false); }
