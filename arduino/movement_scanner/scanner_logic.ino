// Smart Home ONLY - Arduino Code (Uno R4 WiFi)
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

WiFiClient client; // Shared WiFi Client

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect
  }

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

  // Initialize Modules
  setupSmartHome();
}

void loop() {
  runSmartHome();
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}
