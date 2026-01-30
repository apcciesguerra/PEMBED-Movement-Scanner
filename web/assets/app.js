const ALERT_THRESHOLD = 30; // Matches Arduino code

function fetchLatestData() {
    fetch('api.php?action=latest')
        .then(response => response.json())
        .then(data => {
            updateDashboard(data);
        })
        .catch(err => console.error('Error fetching data:', err));
}

function fetchHistory() {
    fetch('api.php?action=history')
        .then(response => response.json())
        .then(data => {
            updateLogTable(data);
        })
        .catch(err => console.error('Error fetching history:', err));
}

function updateDashboard(data) {
    if (!data || !data.distance) return;

    const distanceEl = document.getElementById('current-distance');
    const alertBox = document.getElementById('alert-box');
    const latestTime = document.getElementById('latest-time');
    const latestStatus = document.getElementById('latest-status');

    // Update Distance
    distanceEl.textContent = parseFloat(data.distance).toFixed(1);

    // Update Alert Status
    const isAlert = parseInt(data.alert_triggered) === 1;

    // Status Logic
    alertBox.className = 'status-text ' + (isAlert ? 'alert' : 'safe');
    alertBox.textContent = isAlert ? 'Proximity Alert' : 'Safe';

    // Detailed Info
    latestStatus.textContent = isAlert ? 'Detected' : 'Clear';
    latestTime.textContent = data.timestamp;
}

function updateLogTable(data) {
    const tbody = document.getElementById('log-body');
    tbody.innerHTML = ''; // Clear existing

    data.forEach(row => {
        const tr = document.createElement('tr');
        const isAlert = parseInt(row.alert_triggered) === 1;

        tr.innerHTML = `
            <td>#${row.id}</td>
            <td>Ultrasonic (HC-SR04)</td>
            <td>${row.timestamp}</td>
            <td>${parseFloat(row.distance).toFixed(2)} cm</td>
            <td><span class="badge ${isAlert ? 'alert' : 'safe'}">${isAlert ? 'Alert' : 'Safe'}</span></td>
        `;
        tbody.appendChild(tr);
    });
}

// Poll every 1 second
setInterval(() => {
    fetchLatestData();
    fetchHistory();
}, 1000);

// Initial Load
fetchLatestData();
fetchHistory();
