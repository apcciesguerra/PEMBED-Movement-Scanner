// ---------------------------------------------------------------------------
// SMART HOME LOGIC
// ---------------------------------------------------------------------------

// Pin Definitions
const int redPin = 6;       // Red LED
const int greenPin = 7;     // Green LED
const int yellowPin = 8;    // Yellow LED

const int motorEnablePin = 3; 
const int motorIn1 = 4;
const int motorIn2 = 5;

unsigned long lastSmartHomeCheckTime = 0;
const long smartHomeInterval = 2000; // Check state every 2 seconds

void setupSmartHome() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);

  pinMode(motorEnablePin, OUTPUT);
  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);

  // Initialize
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(yellowPin, LOW);
  
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, LOW);
  analogWrite(motorEnablePin, 0);
  
  Serial.println("Smart Home Module Initialized");
}

void runSmartHome() {
  if (millis() - lastSmartHomeCheckTime > smartHomeInterval) {
    lastSmartHomeCheckTime = millis();
    fetchSmartHomeState();
  }
}

void fetchSmartHomeState() {
  if (client.connect(server, port)) {
    // Serial.println("Checking Smart Home State...");
    
    client.println("GET /movement_scanner/api.php?action=get_states HTTP/1.1");
    client.print("Host: "); client.println(server);
    client.println("Connection: close");
    client.println();

    // Read Response
    bool headersEnded = false;
    String responseBody = "";
    
    while(client.connected() || client.available()) {
      if(client.available()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
          headersEnded = true;
          continue;
        }
        if (headersEnded) {
          responseBody += line;
        }
      }
    }
    client.stop();
    
    // Parse JSON
    parseStateAndApply(responseBody);
    
  } else {
    Serial.println("Connection to server failed (SmartHome)");
  }
}

void parseStateAndApply(String json) {
  // Parsing "red_state":1, "green_state":0, etc.
  int redIndex = json.indexOf("red_state");
  int greenIndex = json.indexOf("green_state");
  int yellowIndex = json.indexOf("yellow_state");
  int motorIndex = json.indexOf("motor_speed");
  
  if(redIndex != -1 && greenIndex != -1 && yellowIndex != -1 && motorIndex != -1) {
    int redVal = findFirstDigit(json, redIndex);
    int greenVal = findFirstDigit(json, greenIndex);
    int yellowVal = findFirstDigit(json, yellowIndex);
    int motorVal = findFirstNumber(json, motorIndex);

    // Apply
    digitalWrite(redPin, redVal == 1 ? HIGH : LOW);
    digitalWrite(greenPin, greenVal == 1 ? HIGH : LOW);
    digitalWrite(yellowPin, yellowVal == 1 ? HIGH : LOW);
    
    // Motor
    if(motorVal > 0) {
        digitalWrite(motorIn1, HIGH);
        digitalWrite(motorIn2, LOW);
        analogWrite(motorEnablePin, motorVal);
    } else {
        digitalWrite(motorIn1, LOW);
        digitalWrite(motorIn2, LOW);
        analogWrite(motorEnablePin, 0);
    }
    
    Serial.print("SmartHome -> R:");
    Serial.print(redVal);
    Serial.print(" G:");
    Serial.print(greenVal);
    Serial.print(" Y:");
    Serial.print(yellowVal);
    Serial.print(" Motor:");
    Serial.println(motorVal);
  }
}

int findFirstDigit(String str, int startIndex) {
    for (int i = startIndex; i < str.length(); i++) {
        if (isDigit(str.charAt(i))) {
            return String(str.charAt(i)).toInt();
        }
    }
    return 0;
}

int findFirstNumber(String str, int startIndex) {
    String numStr = "";
    bool foundDigit = false;
    for (int i = startIndex; i < str.length(); i++) {
        char c = str.charAt(i);
        if (isDigit(c)) {
            numStr += c;
            foundDigit = true;
        } else if (foundDigit) {
            // End of number
            break;
        }
    }
    return numStr.toInt();
}
