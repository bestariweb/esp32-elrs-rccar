#include <AlfredoCRSF.h>
#include <HardwareSerial.h>

#define PIN_RX 16
#define PIN_TX 17

bool MAJU = false;
bool MUNDUR = false;
bool KANAN = false;
bool KIRI = false;

// Set up a new Serial object
HardwareSerial crsfSerial(1);
AlfredoCRSF crsf;

void setup()
{
  Serial.begin(115200);
  Serial.println("COM Serial initialized");
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(14,OUTPUT);
  pinMode(27,OUTPUT);
  crsfSerial.begin(CRSF_BAUDRATE, SERIAL_8N1, PIN_RX, PIN_TX);
  if (!crsfSerial) while (1) Serial.println("Invalid crsfSerial configuration");

  crsf.begin(crsfSerial);
  Berhenti();
}

void loop()
{
    // Must call crsf.update() in loop() to process data
    crsf.update();
    printChannels();
    
    if ((KANAN) || (KIRI)|| (MAJU) || (MUNDUR)) 
    {
     if (MAJU){
        KiriMaju();
        KananMaju();
      } else if (MUNDUR) {
        KiriMundur();
        KananMundur();
      } else if (KANAN)
      {
        PutarCCW();  
      } else if (KIRI){
        PutarCW();
      }
    } else 
    {
      Berhenti();
    } 
}

void Berhenti()
{
  digitalWrite(13,HIGH);
  digitalWrite(12,HIGH);
  digitalWrite(14,HIGH);
  digitalWrite(27,HIGH);
  
}
void KiriMaju()
{
  digitalWrite(13,HIGH);
  digitalWrite(12,LOW);
}

void KiriMundur()
{
  digitalWrite(13,LOW);
  digitalWrite(12,HIGH);
}

void KananMaju()
{
  digitalWrite(14,LOW);
  digitalWrite(27,HIGH);
}

void KananMundur()
{
  digitalWrite(14,HIGH);
  digitalWrite(27,LOW);
}

void PutarCW()
{
  KiriMaju();
  KananMundur();
}

void PutarCCW()
{
  KiriMundur();
  KananMaju();
}

//Use crsf.getChannel(x) to get us channel values (1-16).
void printChannels()
{
  int channel1= crsf.getChannel(1);
  int channel2= crsf.getChannel(2);
  
MAJU = false;
MUNDUR = false;
KANAN = false;
KIRI = false;
//Serial.print(channel1);
//Serial.print("   ");
//Serial.print(channel2);
if ((channel1 != 0) && (channel2 !=0))
{
      if (channel1 >1700){
        KANAN = true;
        KIRI = false;
        //Serial.println("CCW");
      } else if (channel1 < 1300) {
        KANAN = false;
        KIRI = true;
        //Serial.println("CW");
      } else {
        KANAN = false;
        KIRI = false;
        if (channel2 >1700){
          MAJU = true;
          MUNDUR = false;
          //Serial.println("MAJU");
          } else if (channel2 < 1300) {
          MUNDUR = true;
          MAJU = false;
          //Serial.println("MUNDUR");
          } else {
          MUNDUR = false;
          MAJU = false;
        }
      }
  }  

}
