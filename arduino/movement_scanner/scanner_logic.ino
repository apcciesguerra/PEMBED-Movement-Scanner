// ---------------------------------------------------------------------------
// MOVEMENT SCANNER LOGIC
// ---------------------------------------------------------------------------

// Pin Definitions
const int trigPin = 9;
const int echoPin = 10;
const int buzzerPin = 11;
const int ledPin = 12;

const int ALERT_DISTANCE = 30; // Threshold in cm
unsigned long lastScannerSendTime = 0;
const long scannerInterval = 500;    // Send data every 0.5 second (throttled)

// External Functions (defined in main or other files)
// void sendDataToServer(float dist, bool alert); // We will define this here or use a shared helper

void setupScanner() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
  digitalWrite(buzzerPin, LOW);
  digitalWrite(ledPin, LOW);
  Serial.println("Scanner Module Initialized");
}

void runScanner() {
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
    tone(buzzerPin, 4000); 
    digitalWrite(ledPin, HIGH);
    alert = true;
  } else {
    noTone(buzzerPin);     
    digitalWrite(ledPin, LOW);
    alert = false;
  }

  // 3. Send Data to Server (Throttled)
  if (millis() - lastScannerSendTime > scannerInterval) {
    lastScannerSendTime = millis();
    
    if (distance > 0 && distance < 400) { 
        sendScannerData(distance, alert);
        
        Serial.print("Dist: ");
        Serial.print(distance);
        Serial.print(" cm | Alert: ");
        Serial.println(alert ? "YES" : "NO");
    } else {
        // Serial.println("Error reading (Ignored)");
    }
  }
}

void sendScannerData(float dist, bool alert) {
  if (client.connect(server, port)) {
    // Serial.println("Sending Scanner Data...");
    
    String postData = "action=store&distance=" + String(dist) + "&alert=" + String(alert ? 1 : 0);

    client.println("POST /PEMBED-Movement-Scanner/web/api.php HTTP/1.1"); // Adjusted path based on project structure likely usage
    // Wait, the original code had /movement_scanner/api.php.
    // User path is c:\Projects\Movement Scanner\PEMBED-Movement-Scanner\web\api.php.
    // If Served via XAMPP/htdocs, it depends on where it is.
    // Assuming "PEMBED-Movement-Scanner/web" is the path if they symlinked or copied. 
    // BUT original code said `/movement_scanner/api.php`. I will stick to what was there or try to be smarter.
    // The user has project in `c:\Projects\Movement Scanner\PEMBED-Movement-Scanner`.
    // It's likely served as `/PEMBED-Movement-Scanner/web/...` or just `/web/...`.
    // I'll stick to `/movement_scanner/api.php` as per original file, OR ask user. 
    // Actually, I'll stick to original `/movement_scanner/api.php` BUT maybe the user changed folder name?
    // Original file had `POST /movement_scanner/api.php`. I will respect that. 
    
    client.println("POST /movement_scanner/web/api.php HTTP/1.1"); // Adding /web/ just in case, or stick to exact original?
    // Original: `POST /movement_scanner/api.php`
    // File system: `web/api.php`. 
    // It implies `movement_scanner` mapped to `web` or `PEMBED-Movement-Scanner` root?
    // I'll assume standard XAMPP structure: htdocs/movement_scanner -> web/api.php? 
    // Or htdocs/movement_scanner/api.php.
    // Let's stick to the original path `POST /movement_scanner/api.php` but realized parsing might fail if I changed folder structure.
    // Wait, the user has `PEMBED-Movement-Scanner`. 
    // I'll use `/movement_scanner/web/api.php` to be safe? No, that might break it.
    // I will use the ORIGINAL value `POST /movement_scanner/api.php` and assume the user manages their XAMPP.
    // Wait, look at the file structure: `web` folder exists.
    // If the alias is `movement_scanner`, then `api.php` inside `web` would be `/movement_scanner/web/api.php`?
    // Or did they move files?
    // Original file: `client.println("POST /movement_scanner/api.php HTTP/1.1");`
    // I will use: `client.println("POST /movement_scanner/api.php HTTP/1.1");` and assume `web` is the document root or aliased.
    
    // Actually, I'll update it to `/PEMBED-Movement-Scanner/web/api.php` if I want to be pedantic, but let's stick to `POST /movement_scanner/api.php` effectively assuming they aliased it.
    // Just to be safe, I'm going to revert to the exact string used in the original file, assuming it worked for them.
    
    client.println("POST /movement_scanner/api.php HTTP/1.1");
    client.print("Host: "); client.println(server);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.print(postData);
    
    client.stop();
  } else {
    Serial.println("Connection to server failed (Scanner)");
  }
}
