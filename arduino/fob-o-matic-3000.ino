#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

int  val = 0; 
char code[10]; 
int  bytesread = 0; 

void setup()   {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("  Fob-o-Matic 3000");
  display.println("");
  display.println(" Scan the first fob");
  display.display();
  
  Serial.begin(2400);       // RFID reader SOUT pin connected to Serial RX pin at 2400bps 
  pinMode(2,OUTPUT);        // Set digital pin 2 as OUTPUT to connect it to the RFID /ENABLE pin 
  digitalWrite(2, LOW);     // Activate the RFID reader

}


void loop() {
 if(Serial.available() > 0) {           // if data available from reader 
    if((val = Serial.read()) == 10) {   // check for header 
      bytesread = 0; 
      while(bytesread<10) {              // read 10 digit code 
        if( Serial.available() > 0) { 
          val = Serial.read(); 
          if((val == 10)||(val == 13)) { // if header or stop bytes before the 10 digit reading 
            break;                       // stop reading 
          } 
          code[bytesread] = val;         // add the digit           
          bytesread++;                   // ready to read next digit  
        } 
      } 
      if(bytesread == 10) {              // if 10 digit read is complete 
        digitalWrite(2, HIGH);                 // deactivate the RFID reader for a moment so it will not flood
        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(0,10);
        display.println(code);
        display.display();

      } 
      bytesread = 0; 
      delay(5000);                            // wait for a bit 
      digitalWrite(2, LOW);                  // Activate the RFID reader
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0,0);
      display.println("Ready for next fob...");
      display.display();
    } 
  }
}

