//SoftwareSerial Libray for new Rx, Tx
#include <SoftwareSerial.h>
SoftwareSerial NodeSerial(14, 12);
// PMS Sensor Library
#include "PMS.h"
PMS pms(NodeSerial);
PMS::DATA data;
// AM2320 Library (Humidity and temperature)
#include <AM2320.h>
AM2320 th(&Wire);
// OLED Library
#include "SSD1306.h" 
#define OLED_SDA 5
#define OLED_SCL 4
SSD1306 display(0x3c, OLED_SDA, OLED_SCL);


void displayPM25(String PM25, String Humid, String Temp){
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "PM 2.5: ");
  display.drawString(60, 0, PM25);

  //Humidity
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 20, "Humid: ");
  display.drawString(60, 20, Humid);
  display.drawString(110, 20, "%");

  //Temp
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 40, "Temp: ");
  display.drawString(60, 40, Temp);
  display.drawString(110, 40, "C");

  display.display();
  
  delay(1000);
  display.clear();
}


void setup()
{
  // Serial Start
  Serial.begin(9600);
  Serial.println("Begin");

  // OLED Start
  display.init();
}


void loop()
{

  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 40, "Temp: ");
//  display.drawString(60, 40, Temp);
  display.drawString(110, 40, "C");
  display.display();

  delay(1000);
  display.clear();
  
  if (pms.read(data))
  {
    //PM 1.0, 2.5 and 10.0
    Serial.print("PM 1.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_1_0);
    Serial.print("PM 2.5 (ug/m3): ");
    Serial.println(data.PM_AE_UG_2_5);
    Serial.print("PM 10.0 (ug/m3): ");
    Serial.println(data.PM_AE_UG_10_0);
    Serial.println();

    //Humidity & Temp
    switch(th.Read()) {
      case 2:
        Serial.println(F("  CRC failed"));
        break;
      case 1:
        Serial.println(F("  Sensor offline"));
        break;
      case 0:
        Serial.print(F("  Humidity = "));
        Serial.print(th.Humidity);
        Serial.println(F("%"));
        Serial.print(F("  Temperature = "));
        Serial.print(th.cTemp);
        Serial.println(F("*C"));
        Serial.println();

        //Display all values
        displayPM25(String(data.PM_AE_UG_2_5), String(th.Humidity), String(th.cTemp));
        break;
    }
  }

}
