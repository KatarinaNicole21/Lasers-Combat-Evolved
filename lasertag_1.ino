#include <IRSendRev.h>

#define BIT_LEN         0
#define BIT_START_H     1
#define BIT_START_L     2
#define BIT_DATA_H      3
#define BIT_DATA_L      4
#define BIT_DATA_LEN    5
#define BIT_DATA        6

const int ir_freq = 38;             // 38k
const int pinRecv = 2;              // ir receiver connect to D2
const int buttonPin = 4;            //push button connected to D4

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
    Serial.println("init over");
}

unsigned char dta[20];

void loop() 
{
    buttonState = digitalRead(buttonPin); //read state of pushbutton value
    if(buttonState == HIGH) //check if the pushbutton is pressed, if it is the state will be HIGH
    {
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
        Serial.println("BANG! you got shot");
        //add LGB LED turns red here
        IR.Recv(dta);               // receive data to dta

        Serial.println("+------------------------------------------------------+");
        Serial.print("DATA: ");
        for(int i=0; i<dta[BIT_DATA_LEN]; i++)
        {
            Serial.print(dta[i+BIT_DATA], DEC);
            Serial.print("\t");
        }
        Serial.println();
        Serial.println("+------------------------------------------------------+\r\n\r\n");
        
        delay(4000); //nap after being shot, RGB LED will remain red until nap is over
        //RGB LED is now green again
    }
}
