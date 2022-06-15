/* Drafted by Jordan Mellish (jordan.mellish@townsville.qld.gov.au)
 * Refined by Tristan Fivaz (tristan.fivaz@townsville.qld.gov.au)
 * Description: Online and Offline variants for showing temperature, humidity on an OLED and uploading to Thingspeak
 * Only fields that need to be edited include - ssid, pswd, myChannelNumber, myWriteAPIKey and uncomment either "bootOffline()" or "bootOnline()"
 */

/* Import Libraries  */ 
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

/* Definitions */ 
#define DHTPIN D4 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

/* Wi-Fi Credentials */ 
char ssid[] = "TCC-Sensors";                        // Your network SSID (name) 
char pswd[] = "5Birds+Feathers";                    // Your network password
int status = WL_IDLE_STATUS;

WiFiClient  client;

/* ThingSpeak Credentials  */
unsigned long myChannelNumber = 1693948;            // Enter your channel number from ThingSpeak
const char * myWriteAPIKey = "QSX3RE58OBUMLAOV";    // Enter your Write API Key from ThingSpeak

/* Screen Board Defenition */
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ ESP8266);  // This is defining the board type as ESP8266

void setup() {
    Serial.begin(115200); 
    dht.begin(); 
    u8g2.begin();
    u8g2.setFontPosTop();
    }

void loop() {
    //bootOffline();                                 // Uncomment the mode you plan on using, offline/display only or online/display and wifi
    //bootOnline();
    delay(1500);
}



void connectWiFi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid,pswd);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP Address :");
  Serial.println(WiFi.localIP());
  Serial.println();
}


void bootOffline() {
  Serial.println(" ");
  Serial.println("Booting in offline mode");
  delay(2000);
  float h = dht.readHumidity();                         // Reads temperature and humidity, assigns values to a variable
  float t = dht.readTemperature();
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" ");
  Serial.print("Humidity: ");
  Serial.print(h);
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT Sensor");
    return;
    } else { 
        Serial.println(" ");
        u8g2.clearBuffer();                             // Clears all pixel in the memory frame buffer.  
        u8g2.setFont(u8g2_font_maniac_tr);  
        u8g2.setCursor(0, 15);
        u8g2.print("Temperature: ");                    // Start drawing strings at the coordinates of x, y “DFR123”
        u8g2.setCursor(8, 15);
        u8g2.print(t);
        u8g2.setCursor(0, 0);
        u8g2.print("Humidity: ");                      
        u8g2.setCursor(8, 0);
        u8g2.print(h);
        u8g2.sendBuffer();                              // Send the content of the memory frame buffer to the display.
  }
}

void bootOnline() {
  Serial.print("Booting in online mode!");
  ThingSpeak.begin(client);
  if (WiFi.status() != WL_CONNECTED) { connectWiFi();}
  Serial.print("Data Uploaded!");
  delay(2000);
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      Serial.print(t);
      Serial.print(h);
      if (isnan(h) || isnan(t)) {
        Serial.println("Failed to read from DHT Sensor");
        return;
        } else { 
          //ThingSpeak.setField(Field No. , data)
          ThingSpeak.setField(1, t); 
          ThingSpeak.setField(2, h); 
          ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
          delay(2000);
          u8g2.clearBuffer();                           // Clears all pixel in the memory frame buffer.  
          u8g2.setFont(u8g2_font_maniac_tr);
          u8g2.setCursor(0, 15);
          u8g2.print("Temperature: ");                  // Start drawing strings at the coordinates of x, y “DFR123”
          u8g2.setCursor(8, 15);
          u8g2.print(t);
          u8g2.setCursor(0, 0);
          u8g2.print("Humidity: ");                     
          u8g2.setCursor(8, 0);
          u8g2.print(h);
          u8g2.sendBuffer();                            // Send the content of the memory frame buffer to the display.      
          delay(20000);
  }
}
