
// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID       "TMPLmBlXAPe6"
#define BLYNK_DEVICE_NAME       "SENSOR"
#define BLYNK_AUTH_TOKEN        "-FMFqU5hLLVztt0hniu58SVjv21a4rNL"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Librerias para el uso de OLED
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>


char auth[] = BLYNK_AUTH_TOKEN;

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


int sensor = A0;
float valor= 0.0;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "LookingPlace";
char pass[] = "contrasenna123";

BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  // Update state
  Blynk.virtualWrite(V1, value);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

void setup()
{
  pinMode(sensor, INPUT);
  pinMode(D7, OUTPUT);
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  

  // Inicializacion de display OLED
  delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
  display.setContrast (255); // dim display

  // Clear the buffer.
  display.clearDisplay();
  display.display();
}

void loop()
{
  digitalWrite(D7, LOW);
  
  Blynk.run();
  timer.run();
  //valor = analogRead(sensor);
  valor = analogRead(sensor);
  Serial.println(valor);
  if(valor > 100){
    // Manda a llamar al evento programado en Blynk
    Blynk.logEvent("hppm", String("PELIGRO, niveles de CO2 altos: ")+ valor + String(" PPM."));
    digitalWrite(D7, HIGH);
  }
  Blynk.virtualWrite(V0,valor);
  delay(2000);
  

  // Limpiar buffer
  display.clearDisplay();
 
  // Tamaño del texto
  display.setTextSize(1);
  // Color del texto
  display.setTextColor(SH110X_WHITE);
  // Posición del texto
  display.setCursor(10, 10);
  // Escribir texto
  display.println("Calidad de Aire: ");

  // Tamaño del texto
  display.setTextSize(2);
  // Color del texto
  display.setTextColor(SH110X_WHITE);
  // Posición del texto
  display.setCursor(10, 32);
  // Escribir texto
  display.println(valor);

  // Tamaño del texto
  display.setTextSize(1);
  // Color del texto
  display.setTextColor(SH110X_WHITE);
  // Posición del texto
  display.setCursor(110, 40);
  // Escribir texto
  display.println("ppm");
 
  // Enviar a pantalla
  display.display();
  
}


