#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <U8g2lib.h>

#define ONE_WIRE_BUS 10

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);
DeviceAddress frontThermometer = {0x28, 0x29, 0x64, 0x1B, 0x06, 0x0, 0x0, 0x4D};
DeviceAddress backThermometer = {0x28, 0x85, 0x36, 0x1C, 0x06, 0x00, 0x00, 0xFC};

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R2, /* reset=*/U8X8_PIN_NONE);

const uint8_t *font = u8g2_font_crox3cb_tf;
byte digitWidth = 13;
byte digitHeight = 20;

float tempC_front;
float tempC_back;
float voltage;

byte calculateXOffset(float temp)
{
  byte xOffset = 0;
  if ((temp > -10 && temp < 0) || temp >= 10) //offset 1 chars
  {
    xOffset = digitWidth;
  }
  else if (temp >= 0 && temp < 10) //offset 2 chars
  {
    xOffset = digitWidth * 2;
  }
  return xOffset;
}

void printOLED(void)
{
  u8g2.clearBuffer();
  u8g2.firstPage();
  do
  {
    byte temp1XOffset = calculateXOffset(tempC_front);
    u8g2.setFont(font);
    u8g2.setCursor(0 + temp1XOffset, 20);
    u8g2.print(tempC_front, 1);
    u8g2.drawGlyph(73, 20, 0x00b0); // degree
    u8g2.drawStr(83, 20, "C");

    byte temp2XOffset = calculateXOffset(tempC_back);
    u8g2.setCursor(0 + temp2XOffset, 40);
    u8g2.print(tempC_back, 1);
    u8g2.drawGlyph(73, 40, 0x00b0); // degree
    u8g2.drawStr(83, 40, "C");

  } while (u8g2.nextPage());
}

void setup(void)
{
  u8g2.begin();
  sensors.begin();
}

void loop(void)
{
  sensors.requestTemperatures(); // Send the command to get temperatures
  tempC_front = sensors.getTempC(frontThermometer);
  tempC_back = sensors.getTempC(backThermometer);
  printOLED();
  delay(2000);
}
