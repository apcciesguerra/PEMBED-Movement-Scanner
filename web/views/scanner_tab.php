<div id="tab-scanner" class="tab-content active">
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
                <span>HC-SR04</span>
                <!-- Fixed duplicate label in original? No, it was "Status" -> "Detected/Clear" in original. -->
                <!-- Original had: Status -> Detected/Clear. I should fix that if I can, but I'll stick to original structure logic. -->
                <!-- Checking original: Status -> <span id="latest-status">--</span>. -->
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
</div>