#include <DHTesp.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define TEMPERATURE_HUMIDITY_PIN D1
#define CO_PIN A0
#define DISPLAY_SCL D2
#define DISPLAY_SDA D3

const int num_samples = 10;
DHTesp dht;
LiquidCrystal_I2C display(0x27, 16, 2);


float temperature = 0;
float humidity = 0;
float co = 0;


void setup()
{
  pinMode(CO_PIN, INPUT);
  dht.setup(TEMPERATURE_HUMIDITY_PIN, DHTesp::DHT22);

  delay(100);

  Wire.begin(DISPLAY_SDA, DISPLAY_SCL);
  delay(100);

  display.init();
  display.backlight();

  display.setCursor(0, 0);
  display.print("T     H     CO");
}

bool read_temp_and_humidity()
{
  TempAndHumidity data = dht.getTempAndHumidity();

  if (dht.getStatus() != DHTesp::ERROR_NONE)
  {
    return false;
  }
  else
  {
    temperature = data.temperature;
    humidity = data.humidity;
  }
  return true;
}


void read_co()
{
  long sum = 0;

  for (int i = 0; i < num_samples; i++)
  {
    sum += analogRead(CO_PIN);
    delay(10);
  }

  float adcValue = sum / (float)num_samples;

  // Convert ADC value to voltage (5V reference)
  co = adcValue * (5.0 / 1023.0);
}

void print_values()
{
  display.setCursor(0, 1);
  display.print(temperature, 1);

  display.setCursor(6, 1);
  display.print(humidity, 1);

  display.setCursor(12, 1);
  display.print(co, 2);
}

void loop()
{
  bool dht_success = read_temp_and_humidity();
  if (!dht_success)
  {
    display.print("DHT Error");
  }
  read_co();
  print_values();
  delay(2000);  // DHT22 needs at least 2 seconds
}
