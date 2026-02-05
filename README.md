# Intruder Alert! - Movement Scanner & Smart Home
Arduino PEMBED Class Project

## Project Description
This project consists of two separate Arduino modules running on different boards:
1.  **Board A (Movement Scanner)**: Detects proximity and alerts via Buzzer/LED.
2.  **Board B (Smart Home Node)**: Controls 3 LEDs (Red/Green/Yellow) and a DC Motor via Web Interface.

Both boards communicate with a central Web Server (PHP/MySQL) over WiFi.

---
## 1. Web & Database Setup (PC Side)
### Database
1.  Open phpMyAdmin.
2.  Import `database.sql` (Creates `movement_scanner` DB and tables).

### Web Interface
1.  Place project folder in `htdocs`.
2.  Update `web/db.php` if you have a MySQL password and database name.
3.  Access via: `http://<YOUR_PC_IP>/PEMBED-Movement-Scanner/web/`

---
## 2. Arduino Configuration
The code is split into two separate folders. You do not need to edit config flags, just open the correct project for each board.
1.  **Scanner Board**: Open `arduino/movement_scanner/movement_scanner.ino` and upload.
2.  **Smart Home Board**: Open `arduino/smart_home/smart_home.ino` and upload.
3.  *Remember to update WiFi SSID/Password and Server IP in BOTH files.*

---
## 3. Wiring Guide - Board A (Movement Scanner)
*Function: Detects distance and logs to server.*

| Component | Pin | Connect To |
| :--- | :--- | :--- |
| **HC-SR04 Sensor** | VCC | 5V |
| | GND | GND |
| | Trig | D9 |
| | Echo | D10 |
| **Buzzer** | (+) | D11 |
| | (-) | GND |
| **Blue LED (Alert)** | Anode | D12 |
| | Cathode | GND (via 220Ω Resistor) |

---
## 4. Wiring Guide - Board B (Smart Home)
*Function: Controls Lights and Fan via Web UI.*

### Power Supply (Critical)
*   **9V Battery**: Required for DC Motor.
    *   **Positive (+)** -> L293D Pin 8 (VC).
    *   **Negative (-)** -> **Breadboard Common Ground**.
*   **Arduino**: Powered via USB.
    *   **GND** -> **Breadboard Common Ground**.

### LED Wiring
| Color | Pin | Connection |
| :--- | :--- | :--- |
| **Red LED** | D6 | Anode -> D6, Cathode -> Resistor -> GND |
| **Green LED** | D7 | Anode -> D7, Cathode -> Resistor -> GND |
| **Yellow LED** | D8 | Anode -> D8, Cathode -> Resistor -> GND |

### Motor & Driver (L293D) Wiring
*L293D Notch = Top*

| L293D Pin | Function | Connect To |
| :--- | :--- | :--- |
| **1 (EN1)** | Enable/Speed | Arduino **D3** (PWM) |
| **2 (IN1)** | Input A | Arduino **D4** |
| **7 (IN2)** | Input B | Arduino **D5** |
| **3 (OUT1)** | Motor + | Motor Wire 1 |
| **6 (OUT2)** | Motor - | Motor Wire 2 |
| **16 (VSS)** | Logic Power | Arduino **5V** |
| **8 (VC)** | Motor Power | **9V Battery (+)** |
| **4, 5, 12, 13** | Ground | **Common Ground** |
