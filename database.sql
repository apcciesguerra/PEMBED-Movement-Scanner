CREATE DATABASE IF NOT EXISTS movement_scanner;
USE movement_scanner;

CREATE TABLE IF NOT EXISTS readings (
    id INT AUTO_INCREMENT PRIMARY KEY,
    distance FLOAT NOT NULL,
    alert_triggered BOOLEAN NOT NULL DEFAULT 0,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS smart_home_state (
    id INT PRIMARY KEY DEFAULT 1,
    red_state BOOLEAN DEFAULT 0,
    green_state BOOLEAN DEFAULT 0,
    yellow_state BOOLEAN DEFAULT 0,
    red_brightness INT DEFAULT 255,
    green_brightness INT DEFAULT 255,
    yellow_brightness INT DEFAULT 255,
    motor_speed INT DEFAULT 0
);

INSERT IGNORE INTO smart_home_state (id, red_state, green_state, yellow_state, red_brightness, green_brightness, yellow_brightness, motor_speed)
VALUES (1, 0, 0, 0, 255, 255, 255, 0);
