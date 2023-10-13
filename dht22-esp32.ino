// Daniell Pan - October 1, 2023
// DHT22 Temperature/Humidity Sensor with ESP32 microcontroller and I2C OLED Display

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  dht.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
}

void loop() {
  display.clearDisplay();
  
  // read temperature
  float t = dht.readTemperature();
  // read humidity
  float h = dht.readHumidity();
  
  // check if failure to read
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // declare string length
  const char* temp = "TEMP";
  const char* celcius = "00.00C";
  
  // calculate text width based on the number of characters
  int textWidth = strlen(temp) * 12; 
  // calculate center on x axis
  int x0 = (SCREEN_WIDTH - textWidth) / 2 + 1;
  
  textWidth = strlen(celcius) * 18;
  int x1 = (SCREEN_WIDTH - textWidth) / 2 + 1;

  // display temp word
  display.setTextSize(2);
  display.setCursor(x0, 0);
  display.println(temp);
  // display temp value
  display.setTextSize(3);
  display.setCursor(x1, 18);
  display.print(t);
  display.println("C");

  // bar
  display.drawRect(0, 44, 128, 3, WHITE);

  // display humidity word
  display.setCursor(16, 52);
  display.setTextSize(1);
  display.print("Humidity: ");
  // display humidity value
  display.print(h);
  display.println("%");
  display.display(); 

  Serial.println("Temperature:");
  Serial.println(t);

  // 4 second delay to prevent overheating of module causing inaccurate readings
  delay(4000);
  
  
}
