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
}

echo json_encode(['status' => 'error', 'message' => 'Invalid request']);
?>