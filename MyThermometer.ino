#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal.h>
#include <MQ135.h>

#define DHTPIN 6
#define DHTTYPE DHT11

DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;
float rzero;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define RZERO 50
//MQ135 mq135_sensor(A0);
MQ135 mq135_sensor(A0, RZERO);

void setup() {
  //pinMode(A0, INPUT); //Air quality sensor... MQ-135

  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;

  lcd.begin(20, 4);
}

void printLcd(float temperature, int humidity, int airQuality)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MyThermometer 1.0");

  lcd.setCursor(0, 1);
  if (temperature==-9999) lcd.print("Temperature error!");
  else lcd.print("Temperature: "+String(temperature, 1)+"C");

  lcd.setCursor(0, 2);
  if (humidity==-9999) lcd.print("Humidity error!");
  else lcd.print("Humidity: "+String(humidity)+"%");

  lcd.setCursor(0, 3);
  lcd.print("Air quality: "+String(airQuality)+" PPM");
}

void loop() {
  // put your main code here, to run repeatedly:
  float temperature;
  int humidity, airQuality;

  delay(delayMS);

  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) temperature=-9999;
  else temperature=event.temperature;

  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) humidity=-9999;
  else humidity=event.relative_humidity;

  airQuality=mq135_sensor.getCorrectedPPM(temperature, humidity);

  printLcd(temperature, humidity, airQuality);
}
