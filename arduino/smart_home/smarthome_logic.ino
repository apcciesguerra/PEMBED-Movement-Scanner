// ---------------------------------------------------------------------------
// SMART HOME LOGIC
// ---------------------------------------------------------------------------

// Pin Definitions
const int redPin = 6;       // Red LED (PWM)
const int greenPin = 9;     // Green LED (PWM)
const int yellowPin = 10;   // Yellow LED (PWM)

const int motorEnablePin = 3; 
const int motorIn1 = 4;
const int motorIn2 = 5;

unsigned long lastSmartHomeCheckTime = 0;
const long smartHomeInterval = 500; // Check state every 2 seconds

void setupSmartHome() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);

  pinMode(motorEnablePin, OUTPUT);
  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);

  // Initialize
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(yellowPin, 0);
  
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
  Serial.println("\n--- [DEBUG] START FETCH ---");
  Serial.print("[DEBUG] Target IP: ");
  Serial.println(server);

  if (client.connect(server, port)) {
    Serial.println("[DEBUG] Connected to server! Sending GET request...");

    // IMPORTANT: Check mo kung tama itong path. 
    // Kung nasa htdocs/PEMBED-Movement-Scanner/web ka, ito dapat:
    String url = "/PEMBED-Movement-Scanner/web/api.php?action=get_states";
    
    // Kung binago mo folder name, palitan mo yung string sa taas ^
    
    client.print("GET ");
    client.print(url);
    client.println(" HTTP/1.1");
    client.print("Host: "); client.println(server);
    client.println("Connection: close");
    client.println();

    // Read Response
    bool headersEnded = false;
    String responseBody = "";
    bool receivedData = false;
    
    // Timeout counter para di mag-hang
    unsigned long timeout = millis();
    
    while(client.connected() || client.available()) {
      if (millis() - timeout > 5000) {
        Serial.println("[DEBUG] ERROR: Server timed out!");
        client.stop();
        return;
      }

      if(client.available()) {
        String line = client.readStringUntil('\n');
        
        // Print mo yung pinaka-unang line ng response (Dapat HTTP/1.1 200 OK)
        if (!receivedData) {
            Serial.print("[DEBUG] Server Status: ");
            Serial.println(line); 
            receivedData = true;
        }

        // Detect end of headers
        if (line == "\r") {
          headersEnded = true;
          continue;
        }
        
        // Save the body (JSON)
        if (headersEnded) {
          responseBody += line;
        }
      }
    }
    client.stop();
    Serial.println("[DEBUG] Connection closed.");

    // Dito mo makikita kung ano talaga nare-receive niya
    Serial.println("[DEBUG] RAW BODY RECEIVED:");
    Serial.println("------------------------------------------------");
    Serial.println(responseBody);
    Serial.println("------------------------------------------------");

    if (responseBody.length() > 0) {
        parseStateAndApply(responseBody);
    } else {
        Serial.println("[DEBUG] ERROR: Body is empty! Baka mali ang URL o walang nireturn ang PHP.");
    }
    
  } else {
    Serial.println("[DEBUG] CONNECTION FAILED! Check IP Address or Firewall.");
  }
}

void parseStateAndApply(String json) {
  Serial.println("[DEBUG] Parsing Data...");
  
  // Parsing "red_state":1, "green_state":0, etc.
  int redIndex = json.indexOf("red_state");
  int greenIndex = json.indexOf("green_state");
  int yellowIndex = json.indexOf("yellow_state");
  int motorIndex = json.indexOf("motor_speed");
  int redBrightnessIndex = json.indexOf("red_brightness");
  int greenBrightnessIndex = json.indexOf("green_brightness");
  int yellowBrightnessIndex = json.indexOf("yellow_brightness");
  
  // Debug kung nahanap ba ang keys
  if (redIndex == -1) Serial.println("[DEBUG] Error: 'red_state' not found");
  if (greenIndex == -1) Serial.println("[DEBUG] Error: 'green_state' not found");
  
  if(redIndex != -1 && greenIndex != -1 && yellowIndex != -1 && motorIndex != -1) {
    int redVal = findFirstDigit(json, redIndex);
    int greenVal = findFirstDigit(json, greenIndex);
    int yellowVal = findFirstDigit(json, yellowIndex);
    int motorVal = findFirstNumber(json, motorIndex);
    int redBrightness = (redBrightnessIndex != -1) ? findFirstNumber(json, redBrightnessIndex) : 255;
    int greenBrightness = (greenBrightnessIndex != -1) ? findFirstNumber(json, greenBrightnessIndex) : 255;
    int yellowBrightness = (yellowBrightnessIndex != -1) ? findFirstNumber(json, yellowBrightnessIndex) : 255;

    redBrightness = constrain(redBrightness, 0, 255);
    greenBrightness = constrain(greenBrightness, 0, 255);
    yellowBrightness = constrain(yellowBrightness, 0, 255);

    // Apply
    analogWrite(redPin, redVal == 1 ? redBrightness : 0);
    analogWrite(greenPin, greenVal == 1 ? greenBrightness : 0);
    analogWrite(yellowPin, yellowVal == 1 ? yellowBrightness : 0);
    
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
    
    Serial.print("[DEBUG] APPLIED STATE -> R:");
    Serial.print(redVal);
    Serial.print(" G:");
    Serial.print(greenVal);
    Serial.print(" Y:");
    Serial.print(yellowVal);
    Serial.print(" Motor:");
    Serial.print(motorVal);
    Serial.print(" | Brightness R:");
    Serial.print(redBrightness);
    Serial.print(" G:");
    Serial.print(greenBrightness);
    Serial.print(" Y:");
    Serial.println(yellowBrightness);
  } else {
    Serial.println("[DEBUG] ERROR: JSON Parsing failed (incomplete keys).");
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

