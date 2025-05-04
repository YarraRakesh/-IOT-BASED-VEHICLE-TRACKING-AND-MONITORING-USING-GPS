#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#define RX_PIN 4   // GPS RX to Arduino D4
#define TX_PIN 3   // GPS TX to Arduino D3
#define GSM_RX 7   // GSM Module RX
#define GSM_TX 8   // GSM Module TX

SoftwareSerial gpsSerial(RX_PIN, TX_PIN);
SoftwareSerial gsmSerial(GSM_RX, GSM_TX);

TinyGPSPlus gps;

// Enter your phone number here (with quotes to make it a valid string)
String phoneNumber = "+919391548969";  

void setup() {
    Serial.begin(9600);
    gpsSerial.begin(9600);
    gsmSerial.begin(9600);
    
    Serial.println("Initializing...");
    delay(3000);
    
    gsmSerial.println("AT");
    delay(1000);
    gsmSerial.println("AT+CMGF=1");  // Set SMS mode to text
    delay(1000);
    gsmSerial.println("AT+CNMI=1,2,0,0,0");  // New SMS indication
    delay(1000);
    Serial.println("GSM and GPS Modules Initialized");
}

void loop() {
    while (gpsSerial.available() > 0) {
        gps.encode(gpsSerial.read());
    }
    
    if (gps.location.isUpdated()) {
        float latitude = gps.location.lat();
        float longitude = gps.location.lng();
        
        Serial.print("Latitude: ");
        Serial.println(latitude, 6);
        Serial.print("Longitude: ");
        Serial.println(longitude, 6);
        
        String message = "Vehicle Location:\nLatitude: " + String(latitude, 6) +
                         "\nLongitude: " + String(longitude, 6) +
                         "\nGoogle Maps Link: https://www.google.com/maps?q=" +
                         String(latitude, 6) + "," + String(longitude, 6);
                         
        sendSMS(message);
        delay(10000);  // Send update every 10 seconds
    }
}

void sendSMS(String message) {
    gsmSerial.println("AT+CMGS=\"" + phoneNumber + "\"");
    delay(1000);
    gsmSerial.println(message);
    delay(1000);
    gsmSerial.write(26);  // End of SMS (Ctrl+Z)
    delay(3000);
    Serial.println("SMS Sent");
}
