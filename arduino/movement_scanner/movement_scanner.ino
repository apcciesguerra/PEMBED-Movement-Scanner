// Movement Scanner Arduino Code (Uno R4 WiFi)
#include <WiFiS3.h> // Use WiFiS3 for Uno R4 WiFi

// ---------------------------------------------------------------------------
// 1. WIFI CONFIGURATION
// ---------------------------------------------------------------------------
const char ssid[] = "ComLab314";        // Change your network SSID
const char pass[] = "#Ramswifi";    // Change your network password

// 2. SERVER CONFIGURATION
// IMPORTANT: Use your computer's local IP address (e.g., 192.168.1.5), NOT "localhost"
IPAddress server(192, 168, 35, 246); 
const int port = 80;

WiFiClient client;

// ---------------------------------------------------------------------------
// 3. HARDWARE PIN CONFIGURATION
// ---------------------------------------------------------------------------
const int trigPin = 9;
const int echoPin = 10;
const int buzzerPin = 11;
const int ledPin = 12;

const int ALERT_DISTANCE = 30; // Threshold in cm
unsigned long lastSendTime = 0;
const long interval = 500;    // Send data every 1 second

// ---------------------------------------------------------------------------
// SETUP
// ---------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect
  }

  // Pin Modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  // Initial state
  digitalWrite(buzzerPin, LOW);
  digitalWrite(ledPin, LOW);

  // WiFi Connection
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, pass);
    Serial.print(".");
    delay(5000);
  }
  
  Serial.println("\nConnected to WiFi");
  printWifiStatus();
}

// ---------------------------------------------------------------------------
// LOOP
// ---------------------------------------------------------------------------
void loop() {
  long duration;
  float distance;
  bool alert = false;

  // 1. Read Sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // 2. Handle Alert
  if (distance > 0 && distance < ALERT_DISTANCE) {
    // Gamit tayo ng tone() para sure na tutunog kahit Passive Buzzer
    tone(buzzerPin, 4000); 
    digitalWrite(ledPin, HIGH);
    alert = true;
  } else {
    // Patayin ang tone
    noTone(buzzerPin);     
    digitalWrite(ledPin, LOW);
    alert = false;
  }

// 3. Send Data to Server (Throttled)
  if (millis() - lastSendTime > interval) {
    lastSendTime = millis();
    
    // ETO YUNG FIX: Check muna kung valid ang distance
    // Kapag 0 o kaya lampas 400 (depende sa sensor, minsan 999 ang error), WAG i-send.
    if (distance > 0 && distance < 400) { 
        sendDataToServer(distance, alert);
        
        Serial.print("Dist: ");
        Serial.print(distance);
        Serial.print(" cm | Alert: ");
        Serial.println(alert ? "YES" : "NO");
    } else {
        Serial.println("Error reading (Ignored)");
    }
  }
}

// ---------------------------------------------------------------------------
// HELPER FUNCTIONS
// ---------------------------------------------------------------------------
void sendDataToServer(float dist, bool alert) {
  if (client.connect(server, port)) {
    Serial.println("Connecting to server...");
    
    // Prepare Post Data
    String postData = "action=store&distance=" + String(dist) + "&alert=" + String(alert ? 1 : 0);

    // Send HTTP POST Request
    client.println("POST /movement_scanner/api.php HTTP/1.1");
    client.print("Host: "); client.println(server); // Sometimes needed
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.print(postData);
    /*
    while(client.connected() || client.available()) {
      if(client.available()) { 
        String line = client.readStringUntil('\n'); 
        Serial.println(line); 
      } 
    }
    */
    client.stop();
    Serial.println("Data sent");
  } else {
    Serial.println("Connection to server failed");
  }
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}
