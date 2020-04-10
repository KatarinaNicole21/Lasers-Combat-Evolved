#include <IRSendRev.h>
#include <ChainableLED.h>

#define BIT_LEN         0
#define BIT_START_H     1
#define BIT_START_L     2
#define BIT_DATA_H      3
#define BIT_DATA_L      4
#define BIT_DATA_LEN    5
#define BIT_DATA        6
#define NUM_LEDS        1

const int ir_freq = 38;             // 38k
const int pinRecv = 2;              // ir receiver connect to D2
const int buttonPin = 4;            //push button connected to D4
ChainableLED leds(7,8, NUM_LEDS);   //LED connected to D8

int buttonState = 0;

unsigned char dtaSend[20];
char PlayerName[100] = "xXPew^3Xx";

void dtaInit() 
{ 
    //constant logic DO NOT CHANGE  
    dtaSend[BIT_LEN]        = 11;      // all data that needs to be sent
    dtaSend[BIT_START_H]    = 179;    // the logic high duration of "Start"
    dtaSend[BIT_START_L]    = 90;     // the logic low duration of "Start"
    dtaSend[BIT_DATA_H]     = 11;     // the logic "long" duration in the communication
    dtaSend[BIT_DATA_L]     = 33;     // the logic "short" duration in the communication

    //total bit length, can be modified
    dtaSend[BIT_DATA_LEN]   = 2;      // Number of data which will sent. If the number is other, you should increase or reduce dtaSend[BIT_DATA+x].

    //bit string data that will be sent
    dtaSend[BIT_DATA + 0]     = 0;    //Team ID
    dtaSend[BIT_DATA + 1]     = 1;    //Player ID
}

void setup()
{
    Serial.begin(115200);
    Serial.println("init start");
    IR.Init(pinRecv);
    pinMode(buttonPin, INPUT); //initialize the pushbutton as an input
    dtaInit();
    leds.init();
    Serial.println("init over");
}

unsigned char dta[20];

void loop() 
{
   leds.setColorRGB(0,0,255,0);
    buttonState = digitalRead(buttonPin); //read state of pushbutton value
    if(buttonState == HIGH) //check if the pushbutton is pressed, if it is the state will be HIGH
    {
      leds.setColorRGB(0, 100, 75, 0);
      IR.Send(dtaSend, 38); //send signal when pushbutton is HIGH
      Serial.println("pew");
      Serial.println("Player: ");
      Serial.println(PlayerName);
      Serial.println("fired! \nPlayer Number: ");
      Serial.println(dta[BIT_DATA + 0]);
      
      delay(500);
      IR.Init(pinRecv);
    }
    
    if(IR.IsDta())                  // get IR data
    {
        leds.setColorRGB(0, 255, 0, 0);
        Serial.println("BANG! you got shot");
        IR.Recv(dta);               // receive data to dta

        Serial.println("+------------------------------------------------------+");
        Serial.print("Player Numer:\n");
        Serial.println(dta[BIT_DATA + 0]);
        Serial.println("Player ID:\n");
        Serial.println("+------------------------------------------------------+\r\n\r\n");
        delay(5000); //napping function
    }
}
