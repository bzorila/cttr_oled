#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <DHT.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHT_PIN 2 // The Arduino Nano pin connected to DHT22 sensor
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

String tempString, rhString;  // Empty string to keep values of temp and rh

void setup()
{
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) 
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(1000); // Pause for 1 seconds

  // Clear the buffer
  display.clearDisplay();

  display.setTextSize(2);      // text size
  display.setTextColor(WHITE); // text color - in fact, in test version the display color is BLUE :)

  dht.begin();              // initialize DHT22 the temperature and humidity sensor

  tempString.reserve(10); // to avoid fragmenting memory when using String
  rhString.reserve(10);
}

void loop() 
{
  float humi  = dht.readHumidity();    // read humidity
  float temperature_C = dht.readTemperature(); // read temperature

  // check if any reads failed
  if (isnan(humi) || isnan(temperature_C)) {
    tempString = "Failed";
    rhString = "Failed";
  } else {
    tempString  = "T: ";
    tempString += String(temperature_C, 1); // one decimal places
    tempString += " C";
    rhString = "H: ";
    rhString += String(humi, 1); // one decimal places
    rhString += " %";
  }

  display.setCursor(10, 0);
  display.println(tempString); // text to display
  display.setCursor(10, 17);
  display.println(rhString);
  display.display();
  display.clearDisplay();
}