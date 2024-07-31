#include <SoftwareSerial.h>
#include <PN532_SWHSU.h>
#include <PN532.h>
#include <ESP8266WiFi.h>
#include <Adafruit_Fingerprint.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>

#include <Adafruit_SSD1306.h>

#include <Fonts/FreeSerif9pt7b.h>

// Pin assignment

#define SIGNAL_PIN D8
const int trigPin = D6;  //D4

const int echoPin = D7;  //D3

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
SoftwareSerial SWSerial( D5, D0 ); // RX, TX
 
PN532_SWHSU pn532swhsu( SWSerial );
PN532 nfc( pn532swhsu );
String tagId = "None", dispTag = "None";
byte nuidPICC[4];

long duration;

int distance;

LiquidCrystal_I2C lcd(0x3F, 16, 2);
SoftwareSerial mySerial(D3, D4);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);


Servo servoMotor;

const char* ssid     = "Vodafone-1474";   //replace with your own SSID
const char* password = "2d96qhXsGFZGqkNA";    //replace with your own password
const char* host = "api.pushingbox.com";
String member = "";
int flag = 0;
const int MAX_NAMES = 127; // Maximum number of names
const int MAX_NAME_LENGTH = 20; // Maximum length of each name

char names[MAX_NAMES][MAX_NAME_LENGTH];

 
void setup(void)
{ 
  servoMotor.attach(SIGNAL_PIN);
 





  // Set initial position to 0 degrees

  servoMotor.write(0);

  delay(1000);





  // Set initial position to 0 degrees
servoMotor.write(0);

  delay(1000);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output

  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to WiFi...       ");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  
  delay(1500);
  
  Serial.println("Hello Maker!");
  //  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata)
  {
    Serial.print("Didn't Find PN53x Module");
    while (1); // Halt
  }
  // Got valid data, print it out!
  Serial.print("Found chip PN5");
  Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. ");
  Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); 
  Serial.println((versiondata >> 8) & 0xFF, DEC);
  // Configure board to read RFID tags
  nfc.SAMConfig();
  //Serial.println("Waiting for an ISO14443A Card ...");
}
 
 
void loop()
{
  char inChar;
  //int fingerprintID = getFingerprintID();
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("waiting !");
  display.display(); 
  //Serial.println(fingerprintID);
  delay(50);
  readNFC();
}

 
void readNFC()
{
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                       // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  if (success)
  {
    Serial.print("UID Length: ");
    Serial.print(uidLength, DEC);
    Serial.println(" bytes");
    Serial.print("UID Value: ");
    for (uint8_t i = 0; i < uidLength; i++)
    {
      nuidPICC[i] = uid[i];
      Serial.print(" "); Serial.print(uid[i], DEC);
    }
    Serial.println();
    tagId = tagToString(nuidPICC);
    dispTag = tagId;
    Serial.print(F("tagId is : "));
    Serial.println(tagId);
    Serial.println("");
    
    
    delay(1000);  // 1 second halt
    if (tagId==tagId){
    
    flag = 0;
    connecthost("SmritiMandana");
    const char* charValue = "Smriti Mandana";
    Serial.println("Welcome Smriti Mandana");
    //strcpy(names[fingerprintID+1], charValue);
   String starc=  String(charValue) ;
   OLED(starc);
   Doorcontrol();
    }
  }
  else
  {
    // PN532 probably timed out waiting for a card
    Serial.println("Timed out! Waiting for a card...");
  }
}
String tagToString(byte id[4])
{
  String tagId = "";
  for (byte i = 0; i < 4; i++)
  {
    if (i < 3) tagId += String(id[i]) + ".";
    else tagId += String(id[i]);
  }
  return tagId;
}

void connecthost(String data)
{
  if (flag == 0)
  {
    member = data;
    flag = 1;
    Serial.print("connecting to ");
    Serial.println(host);
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) 
    {
      Serial.println("connection failed");
      return;
    }

    // We now create a URL for the request
    String url = "/pushingbox?";
    url += "devid=";
    url += "v88998D347B12D4C";
    url += "&Name=" + String(member);

    Serial.print("Requesting URL: ");
    Serial.println(url);
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
      Serial.print("Data Sent!");
    }

    Serial.println();
    Serial.println("closing connection");
  }
}

void Doorcontrol() {

  delay(3000);
     digitalWrite(trigPin, LOW);

   delayMicroseconds(2);

   Serial.print("hi");

   Serial.print(D8);

// Sets the trigPin on HIGH state for 10 micro seconds

  digitalWrite(trigPin, HIGH);

  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds

  duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor

Serial.print("Distance: ");

Serial.println(distance);
 if (distance< 100){
    servoMotor.write(180);
delay(300);
// Wait for 10 seconds
delay(5000);
servoMotor.write(0);
 delay(100);
    }

  
}
void OLED(String starc){
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("welcome");
  display.println(starc);  
  display.display(); 


}