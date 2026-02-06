<?php
header('Content-Type: application/json');
require 'db.php';

$action = $_REQUEST['action'] ?? '';

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    if ($action === 'store') {
        $distance = $_POST['distance'] ?? 0;
        $alert = $_POST['alert'] ?? 0;

        $stmt = $pdo->prepare("INSERT INTO readings (distance, alert_triggered) VALUES (?, ?)");
        $stmt->execute([$distance, $alert]);

        echo json_encode(['status' => 'success', 'message' => 'Data stored']);
        exit;
    }
} elseif ($_SERVER['REQUEST_METHOD'] === 'GET') {
    if ($action === 'latest') {
        $stmt = $pdo->query("SELECT * FROM readings ORDER BY timestamp DESC LIMIT 1");
        $data = $stmt->fetch();
        echo json_encode($data ?: []);
        exit;
    }

    if ($action === 'history') {
        $stmt = $pdo->query("SELECT * FROM readings ORDER BY timestamp DESC LIMIT 20");
        $data = $stmt->fetchAll();
        echo json_encode($data);
        exit;
    }

    if ($action === 'get_states') {
        $stmt = $pdo->query("SELECT * FROM smart_home_state WHERE id = 1");
        $data = $stmt->fetch();
        echo json_encode($data ?: [
            'red_state' => 0,
            'green_state' => 0,
            'yellow_state' => 0,
            'red_brightness' => 255,
            'green_brightness' => 255,
            'yellow_brightness' => 255,
            'motor_speed' => 0
        ]);
        exit;
    }
}

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    if ($action === 'set_state') {
        $red = isset($_POST['red']) ? (int) $_POST['red'] : null;
        $green = isset($_POST['green']) ? (int) $_POST['green'] : null;
        $yellow = isset($_POST['yellow']) ? (int) $_POST['yellow'] : null;
        $redBrightness = isset($_POST['red_brightness']) ? (int) $_POST['red_brightness'] : null;
        $greenBrightness = isset($_POST['green_brightness']) ? (int) $_POST['green_brightness'] : null;
        $yellowBrightness = isset($_POST['yellow_brightness']) ? (int) $_POST['yellow_brightness'] : null;
        $motor = isset($_POST['motor']) ? (int) $_POST['motor'] : null;

        if ($red !== null) {
            $stmt = $pdo->prepare("UPDATE smart_home_state SET red_state = ? WHERE id = 1");
            $stmt->execute([$red]);
        }
        if ($green !== null) {
            $stmt = $pdo->prepare("UPDATE smart_home_state SET green_state = ? WHERE id = 1");
            $stmt->execute([$green]);
        }
        if ($yellow !== null) {
            $stmt = $pdo->prepare("UPDATE smart_home_state SET yellow_state = ? WHERE id = 1");
            $stmt->execute([$yellow]);
        }
        if ($redBrightness !== null) {
            $stmt = $pdo->prepare("UPDATE smart_home_state SET red_brightness = ? WHERE id = 1");
            $stmt->execute([$redBrightness]);
        }
        if ($greenBrightness !== null) {
            $stmt = $pdo->prepare("UPDATE smart_home_state SET green_brightness = ? WHERE id = 1");
            $stmt->execute([$greenBrightness]);
        }
        if ($yellowBrightness !== null) {
            $stmt = $pdo->prepare("UPDATE smart_home_state SET yellow_brightness = ? WHERE id = 1");
            $stmt->execute([$yellowBrightness]);
        }

        if ($motor !== null) {
            $stmt = $pdo->prepare("UPDATE smart_home_state SET motor_speed = ? WHERE id = 1");
            $stmt->execute([$motor]);
        }

        echo json_encode(['status' => 'success']);
        exit;
    }
}

echo json_encode(['status' => 'error', 'message' => 'Invalid request']);
?>
