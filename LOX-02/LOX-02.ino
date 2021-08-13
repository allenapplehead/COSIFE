// McMaster Engineering Physics Research Co-op
// COSIFE: Cost-effective Oxygen Sensors In Flexible Environments

//** IMPORTANT CONSTANTS **//
bool ALARM_ON = true;                     // If false, alarm will never be sounded / turned off

bool SMART_ALARM = true;                  // If true, alarm is only enabled if oxygen level dips below or onto the threshold first. 
                                          // If false (and ALARM_ON is true), the alarm will sound whenever %O2 is greater than the oxygen level threshold
                            
const float OXYGEN_LVL_THRESHOLD = 20.00; // To 2 decimal places maximum. Once this threshold is passed, depending on alarm settings above alarm may be sounded

// OLED Screen Initialization
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Passive Buzzer Initialization
#include "pitches.h"
int melody[] = {
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6}; // multiple notes if we want multiple audio signals
int duration = 500;  // 500 miliseconds
const int buzzerPin = 12;

// LED Pin Definition
const int ledPin = 13;

// Serial Monitor for LOX-02 Initialization
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // Arduino Port 10 (RX) goes into LOX-02 Port 3. Arduino Port 11 (TX) goes into LOX-02 Port 4.

void setup() {
  // LED init
  pinMode(ledPin, OUTPUT); 
  
  // OLED Initialization
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE); // Note that the IZOKEE OLEDs have top 1/3 yellow and bottom 2/3 cyan hard wired in (not changeable)
  display.setCursor(0, 0);
  // Display static text
  display.println("Hello, world!");
  display.display(); 

  // Set up serial monitors to get oxygen sensor readings
  Serial.begin(9600);
  mySerial.begin(9600);
  //mySerial.write("M 0\r\n"); // Put LOX-02 into stream mode

  if (SMART_ALARM) ALARM_ON = false;
}

String cur_reading = "";
void loop() {
  if (mySerial.available())  {
    char data = mySerial.read();
    if (data != '\r' && data != '\n') {
      cur_reading += data;      
    }
    Serial.write(data);
  }
  // check if the reading is of the right length. if it is, we can process it for information
  if (cur_reading.length() == 39) {
    // Note: memory is very limited / finicky with OLED running
    float ppO2 = cur_reading.substring(2, 8).toDouble();
    float temp = cur_reading.substring(11, 16).toDouble();
    float pres = cur_reading.substring(19, 23).toDouble();
    float prO2 = cur_reading.substring(26, 32).toDouble();

    // If smart alarm is enabled, we set ALARM_ON to true only when %O2 dips below or onto the threshold
    if (SMART_ALARM && prO2 <= OXYGEN_LVL_THRESHOLD) {
      ALARM_ON = true;
    }
    
    // Sound the passive buzzer and illuminate the red LED if a certain % oxygen threshold is passed
    if (ALARM_ON && prO2 > OXYGEN_LVL_THRESHOLD) {
      digitalWrite(ledPin, HIGH);
      tone(buzzerPin, melody[2], duration);
    } else {
      digitalWrite(ledPin, LOW);
    }

    // Print these readings onto the OLED screen
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("LOX-02 Readings");
    display.println("ppO2 temp pres %O2");
    display.println(cur_reading);
    
    // reset the string for the next stream of sensor readings
    cur_reading = "";

    display.display();
  }
}
