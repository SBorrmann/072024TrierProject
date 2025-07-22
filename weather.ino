#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 10
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define BUTTON_PIN 3
bool showFahrenheit = false; // Start in Celsius

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  static bool prevBtn = HIGH; // test comment
  bool currBtn = digitalRead(BUTTON_PIN);

  if (prevBtn == HIGH && currBtn == LOW) {
    showFahrenheit = !showFahrenheit;
    Serial.print("Button pressed! Now showing ");
    Serial.println(showFahrenheit ? "Fahrenheit" : "Celsius");
    delay(200); // Debounce
  }
  prevBtn = currBtn;

  float tC = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(tC) || isnan(h)) {
    lcd.setCursor(0, 0);
    lcd.print("Sensor error     ");
    return;
  }

  float dispT = showFahrenheit ? (tC * 9.0 / 5.0 + 32.0) : tC;
  char unit = showFahrenheit ? 'F' : 'C';

  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(dispT, 1);
  lcd.print((char)223);
  lcd.print(unit);
  lcd.print("   ");

  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
  lcd.print(h, 1);
  lcd.print("%    ");

  delay(1000);
}