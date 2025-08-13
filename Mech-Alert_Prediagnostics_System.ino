#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Wire.h>  // Needed for I2C communication

// OLED display parameters
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// DHT11 sensor setup
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Pin definitions
#define SOUND_SENSOR_PIN A0
#define VIBRATION_SENSOR_PIN A1
#define LED_PIN 6
#define BUZZER_PIN 7

// Thresholds for sensors
int soundThreshold = 500;       // Example threshold for sound sensor
int vibrationThreshold = 300;   // Example threshold for vibration sensor
int temperatureThreshold = 30;  // Threshold in Celsius for temperature

void setup() {
  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  
  // Initialize DHT11 sensor
  dht.begin();

  // Setup pins
  pinMode(SOUND_SENSOR_PIN, INPUT);
  pinMode(VIBRATION_SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Start Serial communication for debugging
  Serial.begin(9600);

  // Display welcome message on OLED
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("MechAlert System");
  display.display();
  delay(2000); // Show welcome message for 2 seconds
}

void loop() {
  // Read sensor data
  float h = dht.readHumidity();            // Read humidity
  float t = dht.readTemperature();         // Read temperature
  int soundLevel = analogRead(SOUND_SENSOR_PIN);    // Read sound sensor (0-1023)
  int vibrationLevel = analogRead(VIBRATION_SENSOR_PIN); // Read vibration sensor (0-1023)

  // Display sensor data on OLED
  display.clearDisplay();  // Clear the display before writing new data
  display.setTextSize(1);  // Set text size
  display.setCursor(0, 0); // Start at the top-left corner
  display.print("Temp: ");
  display.print(t);
  display.println(" C");

  display.setCursor(0, 10); // Move down by 10 pixels
  display.print("Humidity: ");
  display.print(h);
  display.println(" %");

  display.setCursor(0, 20);
  display.print("Sound: ");
  display.println(soundLevel);

  display.setCursor(0, 30);
  display.print("Vibration: ");
  display.println(vibrationLevel);

  // Update OLED display
  display.display();

  // Check thresholds and control LED and buzzer
  int exceededCount = 0;  // Count the number of sensors that exceed the threshold

  if (t > temperatureThreshold) exceededCount++;
  if (soundLevel > soundThreshold) exceededCount++;
  if (vibrationLevel > vibrationThreshold) exceededCount++;

  // Control LED and buzzer based on exceeded thresholds
  if (exceededCount >= 1) {
    digitalWrite(LED_PIN, HIGH);  // Turn on LED if at least one sensor exceeds its limit
  } else {
    digitalWrite(LED_PIN, LOW);   // Turn off LED otherwise
  }

  if (exceededCount >= 2) {
    digitalWrite(BUZZER_PIN, HIGH);  // Turn on buzzer if two or more sensors exceed limits
  } else {
    digitalWrite(BUZZER_PIN, LOW);   // Turn off buzzer otherwise
  }

  // Debugging output via Serial Monitor
  Serial.print("Temp: ");
  Serial.print(t);
  Serial.print(" C, Humidity: ");
  Serial.print(h);
  Serial.print(" %, Sound: ");
  Serial.print(soundLevel);
  Serial.print(", Vibration: ");
  Serial.println(vibrationLevel);

  // Wait for 5 seconds before the next reading
  delay(5000); // 5-second delay for next sensor reading and OLED update
}