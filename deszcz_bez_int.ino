#include <Sim800L.h>
#include <SoftwareSerial.h>
#include <Servo.h>
//Wykorzystane: C:\Users\damian\AppData\Local\Arduino15\packages\esp8266\hardware\esp8266\2.3.0\libraries\Servo
//Niewykorzystane: D:\Programy\Arduino\libraries\Servo

#define DeszczD D7 // pin cyfrowy daje odpoweidzieÄ‡ deszcz albo brak deszczu zgodnie z ustaionym potencjometrem na czujniku
#define CzasMartwyDeszczu 3 //czas martwy deszczu w sekundach
#define ServoPin D6 //pin z podłaczonym serwem

Servo servo;  


//SoftwareSerial modem(D3,D4);
int CurrentServoPos=0;
unsigned long czasp=0;
int OstatniaWartosc = HIGH;

#define RX  D3  //10
#define TX  D4  //11

Sim800L GSM(RX, TX);
char* numer="000000000"; //telephone number 


void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);     // otwarcie portu szeregowego
  pinMode(ServoPin,OUTPUT);
  servo.attach(ServoPin);
  Serial.println("servo");
  czasp=0;
  CurrentServoPos=0;
  ServoMove(0);
  ServoMove(180);
  ServoMove(0);
  
  
    delay(1000);
    GSM.begin(115200);
    Serial.println(GSM.getFunctionalityMode());
    Serial.println(GSM.getOperator());
   // Serial.println(GSM.calculateLocation());
   // Serial.println(GSM.getLongitude());
   // Serial.println(GSM.getLatitude());
    Serial.println(GSM.signalQuality());
    Serial.println(GSM.getOperator());
     
  pinMode(DeszczD,INPUT);
//  attachInterrupt(digitalPinToInterrupt(DeszczD), deszcz, FALLING);
  
// startujemy modem GSM
//  modem.begin(115200);//38400 / 9600 / 19200 / 57600 / 115200 
//  modem.println("AT");
}

int ServoMove(int deg)
 {
  int ink=1;
  int pos;
  if(CurrentServoPos>deg)
  {
    for(pos = CurrentServoPos; pos >= deg; pos -= ink) // goes from 0 degrees to 180 degrees 
     {                                  // in steps of 1 degree 
      servo.write(pos);              // tell servo to go to position in variable 'pos' 
      delay(15);                       // waits 15ms for the servo to reach the position 
     } 
    }
    else
    {
     for(pos = CurrentServoPos; pos <= deg; pos += ink) // goes from 0 degrees to 180 degrees 
     {                                  // in steps of 1 degree 
      servo.write(pos);              // tell servo to go to position in variable 'pos' 
      delay(15);                       // waits 15ms for the servo to reach the position 
     } 
    }
    CurrentServoPos=pos;
   return(CurrentServoPos);
  }


//unsigned long czas=0;

void deszcz(){ 
   // if(micros()-czasp>CzasMartwyDeszczu*1000000)
  //  {
   //  czasp=micros();
     
     Serial.println("Deszcz"); 
     Serial.println(millis()/1000);
  delay(500);
    ServoMove(160);

       delay(1000);
    
    ServoMove(0);
    Serial.println("Stan sygnalu"); 
    Serial.println(GSM.signalQuality());
    Serial.println("Dzwonie"); 
    delay(1000);
    Serial.println("Dzwonie"); 
    GSM.callNumber(numer);
    Serial.println("Dzwonie"); 
    delay(15000);
    Serial.println("Rozlaczam"); 
    GSM.hangoffCall();
    Serial.println("Rozlonczone"); 
    
   // czasp=millis();
    
 
   // }
  
    
  }

/*void zadzown(char* numer)
 {
  modem.write(F("ATD")); 
  modem.write(number);
  modem.write(F(";\r\n"));

  sleep(10000);

  modem.write(F("ATH\r\n"));
 }
 */
  
void loop() {

  
int odczyt = digitalRead(DeszczD);

if(odczyt<OstatniaWartosc)
 {

  if(millis()-czasp>CzasMartwyDeszczu*1000)
  {
    czasp=millis();
    OstatniaWartosc=odczyt;
    deszcz();
  }
 }

if(odczyt>OstatniaWartosc) // to spowoduje że bedzie się chciał zamykać bardziej niż powinien tylko dla testów
 {

   if(millis()-czasp>CzasMartwyDeszczu*1000)
   {
     OstatniaWartosc=odczyt;
   }
  }



/*
while(modem.available()>0){
  Serial.write(modem.read());
  }
  
  while(Serial.available()>0){
  modem.write(Serial.read());
  }*/
}

