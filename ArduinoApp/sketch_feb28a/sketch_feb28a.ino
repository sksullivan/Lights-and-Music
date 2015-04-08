#include <elapsedMillis.h>

//Serial RGB led controller
//Author: Trevor Shannon
//see http://trevorshp.com/creations/android_led.htm

//maximum duty cycle to be used on each led for color balancing.
//if  white  (R=255, G=255, B=255) doesn t look white, reduce the red, green, or blue max value.
#define MAX_RED 255
#define MAX_GREEN 255
#define MAX_BLUE 255

#define interval 1

// Color pin definitions
#define RED_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11

// TODO: fix right hur
// Power pin definitions
#define PPIN_A 2
#define PPIN_B 4

int r,g,b;
elapsedMillis timer;
int stepLatency = 500;
int bytesRead;
char* message;
int readColor[3];
int LColor[3];
int RColor[3];
int readState;


void setup()
{
        // Set all three of our led pins to output
        pinMode(RED_PIN, OUTPUT);
        pinMode(GREEN_PIN, OUTPUT);
        pinMode(BLUE_PIN, OUTPUT);
        
        pinMode(PPIN_A, OUTPUT);
        pinMode(PPIN_B, OUTPUT);

        // Start the Serial connection
        Serial.begin(115200);
        r = 0;
        g = 0;
        b = 0;
        timer = 0;
        readState = 0;
        
        message = (char*)malloc(4*sizeof(char));
        bytesRead = 0;
        
        LColor[0] = 255;
        LColor[1] = 0;
        LColor[2] = 255;
        
        RColor[0] = 255;
        RColor[1] = 0;
        RColor[2] = 255;
}

void write_color(int power_pin, int red, int green, int blue)
{
        digitalWrite(power_pin, HIGH);

        analogWrite(RED_PIN, red);
        analogWrite(GREEN_PIN, green);
        analogWrite(BLUE_PIN, blue);
}

void loop()
{
  if (Serial.available() > 0) {
    // Reads the incoming byte
    int incomingByte = Serial.read();
    message [bytesRead] = incomingByte;
    bytesRead++;
    if (bytesRead == 3) {
      message[3] = '\n';
      bytesRead = 0;
      unsigned long total = atoi(message);
      if (readState == 0) {
        LColor[0] = total;
      } else if (readState == 1) {
        LColor[1] = total;
      } else if (readState == 2) {
        LColor[2] = total;
      } else if (readState == 3) {
        RColor[0] = total;
      } else if (readState == 4) {
        RColor[1] = total;
      } else {
        RColor[2] = total;
        readState = -1;
      }
      readState++;
    }
  }
  
  r = RColor[0];
  g = RColor[1];
  b = RColor[2];
  
  digitalWrite(PPIN_A, LOW);
  write_color(PPIN_B, r, g, b);
  delayMicroseconds(stepLatency);
  
  r = LColor[0];
  g = LColor[1];
  b = LColor[2];
  
  digitalWrite(PPIN_B, LOW);
  write_color(PPIN_A, r, g, b);
  delayMicroseconds(stepLatency);
}

