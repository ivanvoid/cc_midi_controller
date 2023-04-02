#include "MIDIUSB.h"

// Initiali
const int total_address = 16;

// CHANNALS OF MULTEPLEXAR
int address[total_address][4] = {
{1,1,1,1},  //channel 15
{0,1,1,1}, //channel 14
{1,0,1,1}, //channel 13
{0,0,1,1}, //channel 12
{1,1,0,1}, //channel 11
{0,1,0,1}, //channel 10
{1,0,0,1}, //channel 9
{0,0,0,1}, //channel 8
{1,1,1,0}, //channel 7
{0,1,1,0}, //channel 6
{1,0,1,0}, //channel 5
{0,0,1,0}, //channel 4
{1,1,0,0}, //channel 3
{0,1,0,0}, //channel 2
{1,0,0,0}, //channel 1
{0,0,0,0}, //channel 0
};

int addres_out_pins[] = {2,3,4,5};

int signal_read_pin = A3;

float stored_values[total_address];

void noteOn(byte channel, byte control, byte value) {
  midiEventPacket_t noteOn = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(noteOn);
}

void setup() {
  // In pins
  for (int i=0; i < 4; i++){
    pinMode(addres_out_pins[i], OUTPUT);
  }

  // Output pins
  pinMode(signal_read_pin, INPUT);

  // Init serial port
//  Serial.begin(115200);
  Serial.begin(115200);

  // Init storage
  for (int i=0; i < total_address; i++){
    stored_values[i] = 0;
  }
  
}

void loop() {
  // For each address 
  for (int adr_idx=0; adr_idx < total_address; adr_idx++){
    // Print address
//    Serial.print(address[adr_idx][0]);
//    Serial.print(' ');
//    Serial.print(address[adr_idx][1]);Serial.print(' ');
//    Serial.print(address[adr_idx][2]);Serial.print(' ');
//    Serial.print(address[adr_idx][3]);Serial.print(' ');

    // Send address
    for (int j=0; j < 4; j++){
      if (address[adr_idx][j]){
        digitalWrite(addres_out_pins[j], HIGH);} else {
        digitalWrite(addres_out_pins[j], LOW);
      }
    }

//    delay(100);
    
    // Read signal 
    float value = analogRead(signal_read_pin);
    value = value / 8; // conversion 1024 -> 128
    stored_values[adr_idx] = value;
    Serial.print(adr_idx);
    Serial.print("  |  ");
    Serial.println(stored_values[adr_idx]);
  
  
    // Send values by MIDI
    // channel, pitch, velocity
    noteOn(0, adr_idx, stored_values[adr_idx]);
    MidiUSB.flush();

  }
}
