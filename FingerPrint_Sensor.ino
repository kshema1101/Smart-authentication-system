
#include <Adafruit_Fingerprint.h>
#include <ESP8266WiFi.h>
//#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif9pt7b.h>

// Pin assignment

#define SIGNAL_PIN D8
#define LED D0
const int trigPin = D6;  //D4

const int echoPin = D7;  //D3
int frequency=1000; //Specified in Hz
int buzzPin=D5; 
int timeOn=1000; //specified in milliseconds
int timeOff=3000;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);





// defines variables

long duration;

int distance;

//LiquidCrystal_I2C lcd(0x3F, 16, 2);
SoftwareSerial mySerial(D3, D4);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);


Servo servoMotor;



const char* ssid     = "WAVLINK_626E";   //replace with your own SSID
const char* password = "germany!23";    //replace with your own password
const char* host = "api.pushingbox.com";

String member = "";
int flag = 0;
uint8_t id;
const int MAX_NAMES = 127; // Maximum number of names
const int MAX_NAME_LENGTH = 20; // Maximum length of each name

char names[MAX_NAMES][MAX_NAME_LENGTH];


void setup()
{ servoMotor.attach(SIGNAL_PIN);





  // Set initial position to 0 degrees

  servoMotor.write(0);

  delay(1000);
  pinMode(LED, OUTPUT);

  //lcd.begin(16, 2);
  //lcd.init();
  //lcd.backlight();
  //lcd.setCursor(4, 0);
  //lcd.print("WELCOME");
  delay(2000);

 // lcd.clear();
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output

  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
 // lcd.setCursor(0, 0);
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
 // lcd.setCursor(0, 0);
 // lcd.print("WiFi connected          ");
 // lcd.setCursor(0, 1);
 // lcd.print("IP:");
 // lcd.setCursor(4, 1);
 // lcd.print(WiFi.localIP());
  delay(1500);

  while (!Serial);
  delay(100);
  Serial.println("\n\n Waiting for Fingerprint Sensor");
 // lcd.setCursor(0, 0);
 // lcd.print("    Waiting for           ");
 // lcd.setCursor(0, 1);
 // lcd.print("      Sensor              ");
  delay(1500);
  finger.begin(57600);

  if (finger.verifyPassword())
  {
    Serial.println("Found Successfully");
  //  lcd.setCursor(0, 0);
  //  lcd.print("Sensor found!         ");
  //  lcd.setCursor(0, 1);
  //  lcd.print("                      ");
    delay(1500);
  } else
  {
    Serial.println("Fingerprint sensor not found!!!");
   // lcd.setCursor(0, 0);
   // lcd.print("Sensor Not found!         ");
   // lcd.setCursor(0, 1);
   // lcd.print("                      ");
    while (1)
    {
      delay(1);
    }
  }
}

uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}


String pass() {


  while(Serial.available() == 0){
    }

  String input = Serial.readString();

  
  Serial.println(input);
  return input;

}
void loop()
{ char inChar;
  int fingerprintID = getFingerprintID();
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
  if (fingerprintID == 2)
  {
  Serial.println("Welcome Kshema");
    
    connecthost("Kshema");
    flag = 0;
    const char* charValue = "Kshema";

    strcpy(names[fingerprintID+1], charValue);
   String starc=  String(names[fingerprintID+1]) ;

   OLED(starc);
   Doorcontrol();
   spreadsheet_comm();
   
    
 
  }
  else if (fingerprintID == 100)
  {
    Serial.println("Welcome Smriti Mandana");
    
    connecthost("SmritiMandana");
    flag = 0;
    const char* charValue = "Smriti Mandana";

    strcpy(names[fingerprintID+1], charValue);
   String starc=  String(names[fingerprintID+1]) ;
   OLED(starc);
   Doorcontrol();
 }
  else if (fingerprintID == 29)
  { String val = String(names[fingerprintID+1]);
    Serial.println("Welcome" + val );
    
    connecthost("muthu");
    flag = 0;
    
   OLED(val);
   Doorcontrol();
  }
  

  else if (fingerprintID == -4)
  {
    Serial.println("Did not find a match");
    
    Serial.println("Ready to enroll a fingerprint!");
    Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
    id = readnumber();
    while (Serial.available()) {
      Serial.read();}

    
    Serial.println("Enter your name ");
    String value=pass();

    const char* charValue = value.c_str();

    strcpy(names[id+1], charValue);
    
    String starc=  String(names[id+1]) ;
    Serial.print("Enrolling ID #");
    Serial.println(id);
     while (!  getFingerprintEnroll() );
  //  lcd.setCursor(0, 0);
  //  lcd.print("Welcome            ");
  //  lcd.setCursor(0, 1);
  //  lcd.print("                       ");
    Serial.println("Welcome, you have successfully been registered");
    flag = 0;
  }
  else
  {
    Serial.println("Waiting for valid finger!!!");
    
    
  //  lcd.setCursor(0, 0);
  //  lcd.print(" Place a Valid       ");
  //  lcd.setCursor(0, 1);
  //  lcd.print("     Finger           ");
  }
}

int getFingerprintID()
{
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -4;

  return finger.fingerID;
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
uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  return true;
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
  digitalWrite(LED, HIGH);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  tone(buzzPin, frequency);
   delay(timeOn);
   noTone(buzzPin);
   
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("welcome");
  display.println(starc);  
  display.display(); 
  digitalWrite(LED, LOW);


}

void StreakOLED(String starc){
  
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
  display.println("your streak is");
  display.println(starc);  
  display.display(); 
  


}

void spreadsheet_comm(void) {
  String GOOGLE_SCRIPT_ID = "AKfycbw2-Y4oG4reOdRCoom43lZ-4O10dEjKRNrPnRg_7mupjvexMZAg9c5b7mrumdyOpPV8";
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
   client->setInsecure();
   HTTPClient https;
   String url="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?read";
//   Serial.print(url);
  Serial.print("Making a request");
  //https.begin(url.c_str()); //Specify the URL and certificate
  https.begin(*client, url.c_str());
  https.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  int httpCode = https.GET();
  String payload;
    if (httpCode > 0) { //Check for the returning code
        payload = https.getString();
        
        Serial.println(httpCode);
        Serial.println(payload);
        String pay=  String(payload) ;

        StreakOLED(pay);
      }
    else {
      Serial.println("Error on HTTP request");
    }
  https.end();
}



