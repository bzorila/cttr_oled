#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#include <Adafruit_BME280.h>
// #include <Fonts/FreeSans12pt7b.h>

#include <DHT.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHT_PIN 2 // The Arduino Nano pin connected to DHT22 sensor
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

String tempString, rhString;

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
  delay(1000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  display.setTextSize(2);      // text size
  // display.setFont(&FreeSans12pt7b);
  display.setTextColor(WHITE); // text color
  // display.setCursor(0, 10);    // position to display

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
  // Serial.println(displayString);    // print the temperature in Celsius to Serial Monitor
  // oled_display_center(displayString); // display temperature and humidity on OLED
  display.println(tempString); // text to display
  display.setCursor(10, 17);
  display.println(rhString);
  display.display();
  display.clearDisplay();
}

// void oled_display_center(String text) {
//   int16_t x1;
//   int16_t y1;
//   uint16_t width;
//   uint16_t height;

//   display.getTextBounds(text, 0, 0, &x1, &y1, &width, &height);

//   // center the display both horizontally and vertically
//   display.clearDisplay(); // clear display
//   display.setCursor((SCREEN_WIDTH - width) / 2, (SCREEN_HEIGHT - height) / 2);
//   display.println(text); // text to display
//   display.display();
// }