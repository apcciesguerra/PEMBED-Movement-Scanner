<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Movement Scanner // PROXIMITY_V1</title>
    <link rel="stylesheet" href="assets/style.css">
    <link
        href="https://fonts.googleapis.com/css2?family=Orbitron:wght@400;700;900&family=Rajdhani:wght@300;500;700&display=swap"
        rel="stylesheet">
</head>

<body>
    <div class="container">
        <header>
            <div class="logo">Movement Scanner</div>
            <div class="status-badge">
                <div class="status-dot"></div>
                System Online
            </div>
        </header>

        <nav class="main-nav">
            <button class="nav-btn active" onclick="switchTab('scanner')">Scanner</button>
            <button class="nav-btn" onclick="switchTab('smarthome')">Smart Home</button>
        </nav>