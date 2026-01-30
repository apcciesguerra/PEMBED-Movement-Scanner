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

        <main>
            <div class="dashboard-grid">
                <!-- Real-time Gauge -->
                <div class="card">
                    <h2>Real-time Distance</h2>
                    <div class="metric-display">
                        <span class="value" id="current-distance">--</span>
                        <span class="unit">cm</span>
                    </div>
                    <div id="alert-box" class="status-text">Waiting...</div>
                </div>

                <!-- Last Detection Info -->
                <div class="card">
                    <h2>Latest Reading</h2>
                    <div class="info-row">
                        <span>Time</span>
                        <span id="latest-time">--:--:--</span>
                    </div>
                    <div class="info-row">
                        <span>Sensor</span>
                        <span>HC-SR04</span>
                    </div>
                    <div class="info-row">
                        <span>Status</span>
                        <span id="latest-status">--</span>
                    </div>
                </div>
            </div>

            <!-- Log Table -->
            <div class="table-card">
                <div class="table-header">
                    <h2>Detection History</h2>
                </div>
                <div class="table-container">
                    <table>
                        <thead>
                            <tr>
                                <th>ID</th>
                                <th>Sensor Type</th>
                                <th>Timestamp</th>
                                <th>Distance</th>
                                <th>Status</th>
                            </tr>
                        </thead>
                        <tbody id="log-body">
                            <!-- Data will be injected here -->
                        </tbody>
                    </table>
                </div>
            </div>
        </main>
    </div>

    <script src="assets/app.js"></script>
</body>

</html>