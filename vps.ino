#include <ESP8266WiFi.h> //to execute WiFi related commands
#include <ThingSpeak.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050_light.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int switchPin = 13;
int val;                        // variable for reading the pin status
int val2;                       // variable for reading the delayed status
int buttonState;                // variable to hold the button state
int Mode = 0;   

WiFiClient client; //define client
long myChannelNumber =  1888617;
const char myWriteAPIKey[] = "YGFVJSSOYHS8CNWV";
int zaxis=0;
int yaxis=0;
int xaxis=0;
int result=0;

String welcome = "Welcome To";
String text = "Virtual";
String text1 = " Physiotherapist";
String text2 = "System";

MPU6050 mpu(Wire);

unsigned long timer=0;

void setup() {
pinMode(switchPin, INPUT);
buttonState = digitalRead(switchPin);  
  Serial.begin (9600);

  WiFi.begin("Guptas","1962649201");
while(WiFi.status()!=WL_CONNECTED){ // WL_IDLE_STATUS ,
//WL_CONNECT_FAILED
Serial.print('.');
delay(200);
}
Serial.println();
Serial.println("Connected!");
Serial.println(WiFi.localIP());
ThingSpeak.begin(client);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  display.setTextSize(1.5); // Draw 2X-scale text
display.setTextColor(WHITE);
display.setCursor(25, 25);
display.println(welcome);
oledDisplayCenter(welcome);
delay(1000);
display.println(text);
oledDisplayCenter(text);
delay(1000);
display.println(text1);
oledDisplayCenter(text1);
delay(1000);
display.println(text2);
oledDisplayCenter(text2);
delay(1000);
display.display();
delay(1000);
 
  Wire.begin();
  byte status=mpu.begin();
  Serial.print(F("mpu6050 status"));
  Serial.println(status);
  while (status!=0){
   
  }
  Serial.print(F("Calculating offset \n"));
  delay (1000);
  mpu.calcOffsets();
  Serial.println("Done");
  // put your setup code here, to run once:
}


void loop() {
  // put your main code here, to run repeatedly:
  zaxis=int(mpu.getAngleZ());
  yaxis=int(mpu.getAngleY());
  xaxis=int(mpu.getAngleX());

 val = digitalRead(switchPin);      // read input value and store it in val
      delay(10);                         // 10 milliseconds is a good amount of time
      val2 = digitalRead(switchPin);     // read the input again to check for bounces
      if (val == val2) {                 // make sure we got 2 consistant readings!
        if (val != buttonState) {          // the button state has changed!
          if (val == LOW) {                // check if the button is pressed
            if (Mode == 0) {          
              Mode = 1;               
            } else {
                if (Mode == 1) {        
                Mode = 2;           
            } else {
                if (Mode == 2) {      
                Mode = 3;           
            } else {
                if (Mode == 3) { 
                Mode = 4;          
            } else {
              if (Mode ==4) {
                Mode=0;
              }
            }
            }
           }
          }
         }
        }
        buttonState = val;                 // save the new state in our variable
      }
      
  


if (Mode == 0) { // all-off
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Modes:-\n\n1. Shoulder Flexion \n2. Shoulder Extension\n3. Shoulder Abduction\n4. Shoulder Abduction\n   Horizontal");
  display.display();
  delay(100);
}

if (Mode == 1) { 
// Shoulder Flexion
 
 display.clearDisplay();
 display.setTextSize(2);
 display.setTextSize(1.5);
 display.setTextColor(WHITE);
 display.setCursor(0, 0);
 display.println("Shoulder Flexion");
 display.print("Angle : ");
 display.print(zaxis);
 display.display();
 delay(100);
 if(zaxis>=170){
 tone(D6, 496, 500);
 delay(800);
 tone(D6, 496, 500); 
  }
ThingSpeak.writeField(myChannelNumber,1,zaxis,myWriteAPIKey);
delay(50);  
}

if (Mode == 2) { 
 //Shoulder Extension
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Shoulder Extension");
  display.print("Angle : ");
  if(zaxis<0){
  display.print(zaxis*-1);
  }
  else{
  display.print(zaxis);
  }
  display.display();
  delay(100);
    if(zaxis>=30 or zaxis<=-60){
tone(D6, 496, 500);
 delay(800);
 tone(D6, 496, 500);
  }
ThingSpeak.writeField(myChannelNumber,2,zaxis,myWriteAPIKey);
delay(50);  
      }
if (Mode == 3)  { 
  //Shoulder Abduction
 display.clearDisplay();
  display.setTextSize(2);
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Shoulder Abduction");
  display.print("Angle : ");
  if(yaxis>=45){
  display.print(zaxis);
  if(zaxis>=170){
  tone(D6, 496, 500);
  delay(800);
  tone(D6, 496, 500);
}
ThingSpeak.writeField(myChannelNumber,3,zaxis,myWriteAPIKey);
delay(50); 
}
  else{
   display.print(xaxis);
   if(xaxis>=170){
  tone(D6, 496, 500);
  delay(800);
  tone(D6, 496, 500);
  
} 
ThingSpeak.writeField(myChannelNumber,3,xaxis,myWriteAPIKey);
delay(50); 
}
  display.display();
  delay(100);

}

if (Mode == 4)  { 
  //Shoulder Abduction Horizontal
   display.clearDisplay();
  display.setTextSize(2);
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Shoulder Abduction \nHorizontal");
  display.print("Angle : ");
  if(yaxis>=45){
    if(zaxis>=80 and zaxis<=110){
      if(xaxis>=50){
          tone(D6, 496, 500);
          delay(800);
          tone(D6, 496, 500);
      }
    }
  }
ThingSpeak.writeField(myChannelNumber,4,xaxis,myWriteAPIKey);
delay(50); 
  display.print(xaxis);
  display.display();
  delay(100);
}
 

  mpu.update();
  if ((millis()-timer)>50){
    timer=millis();
   Serial.print("Angle : ");
   Serial.print(zaxis);
   Serial.print('\t');
   Serial.print(yaxis);
   Serial.print('\t');
   Serial.print(xaxis);
   Serial.print('\t');
   Serial.print('\n');
   Serial.print("No of Rotations: ");
   Serial.print(result);
   Serial.print('\n');
   delay(5);
   
  }
// ThingSpeak.writeField(myChannelNumber,1,value ,myWriteAPIKey);
//delay(5);
//ThingSpeak.writeField(myChannelNumber,2,result ,myWriteAPIKey);
//delay(5);

}

void oledDisplayCenter(String text) {
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  display.getTextBounds(text, 0, 0, &x1, &y1, &width, &height);

  // display on horizontal and vertical center
  display.clearDisplay(); // clear display
  display.setCursor((SCREEN_WIDTH - width) / 2, (SCREEN_HEIGHT - height) / 2);
  display.println(text); // text to display
 display.display();
}
 
