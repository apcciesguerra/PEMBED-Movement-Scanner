const ALERT_THRESHOLD = 30; // Matches Arduino code

// Tab Switching
function switchTab(tabName) {
    // Hide all tabs
    document.querySelectorAll('.tab-content').forEach(tab => {
        tab.classList.remove('active');
    });

    // Deactivate all buttons
    document.querySelectorAll('.nav-btn').forEach(btn => {
        btn.classList.remove('active');
    });

    // Show target tab
    document.getElementById(`tab-${tabName}`).classList.add('active');

    // Activate button (targeting by onclick match, simplified)
    const buttons = document.querySelectorAll('.nav-btn');
    buttons.forEach(btn => {
        if (btn.getAttribute('onclick').includes(tabName)) {
            btn.classList.add('active');
        }
    });

    // Save preference (optional)
    localStorage.setItem('activeTab', tabName);
}

// Restore Tab on Load
document.addEventListener('DOMContentLoaded', () => {
    const activeTab = localStorage.getItem('activeTab') || 'scanner';
    switchTab(activeTab);

    // Attach Event Listeners for Smart Home
    const redToggle = document.getElementById('red-toggle');
    const greenToggle = document.getElementById('green-toggle');
    const yellowToggle = document.getElementById('yellow-toggle');
    const redBrightness = document.getElementById('red-brightness');
    const greenBrightness = document.getElementById('green-brightness');
    const yellowBrightness = document.getElementById('yellow-brightness');
    const redBrightnessValue = document.getElementById('red-brightness-value');
    const greenBrightnessValue = document.getElementById('green-brightness-value');
    const yellowBrightnessValue = document.getElementById('yellow-brightness-value');
    const motorSlider = document.getElementById('motor-slider');

    if (redToggle) {
        redToggle.addEventListener('change', (e) => updateSmartHomeState({ red: e.target.checked ? 1 : 0 }));
    }
    if (greenToggle) {
        greenToggle.addEventListener('change', (e) => updateSmartHomeState({ green: e.target.checked ? 1 : 0 }));
    }
    if (yellowToggle) {
        yellowToggle.addEventListener('change', (e) => updateSmartHomeState({ yellow: e.target.checked ? 1 : 0 }));
    }

    if (redBrightness) {
        redBrightness.addEventListener('input', (e) => {
            if (redBrightnessValue) redBrightnessValue.textContent = e.target.value;
        });
        redBrightness.addEventListener('change', (e) => {
            updateSmartHomeState({ red_brightness: e.target.value });
        });
    }

    if (greenBrightness) {
        greenBrightness.addEventListener('input', (e) => {
            if (greenBrightnessValue) greenBrightnessValue.textContent = e.target.value;
        });
        greenBrightness.addEventListener('change', (e) => {
            updateSmartHomeState({ green_brightness: e.target.value });
        });
    }

    if (yellowBrightness) {
        yellowBrightness.addEventListener('input', (e) => {
            if (yellowBrightnessValue) yellowBrightnessValue.textContent = e.target.value;
        });
        yellowBrightness.addEventListener('change', (e) => {
            updateSmartHomeState({ yellow_brightness: e.target.value });
        });
    }

    if (motorSlider) {
        motorSlider.addEventListener('input', (e) => {
            document.getElementById('motor-value').textContent = e.target.value;
        });

        motorSlider.addEventListener('change', (e) => {
            updateSmartHomeState({ motor: e.target.value });
        });
    }
});


// Data Fetching
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

function fetchSmartHomeState() {
    fetch('api.php?action=get_states')
        .then(response => response.json())
        .then(data => {
            updateSmartHomeUI(data);
        })
        .catch(err => console.error('Error fetching smarthome states:', err));
}

function updateSmartHomeState(updates) {
    const formData = new FormData();
    formData.append('action', 'set_state');

    for (const [key, value] of Object.entries(updates)) {
        formData.append(key, value);
    }

    fetch('api.php', {
        method: 'POST',
        body: formData
    })
        .then(response => response.json())
        .then(data => {
            console.log('State updated', data);
        })
        .catch(err => console.error('Error setting state:', err));
}

// UI Updates
function updateDashboard(data) {
    if (!data || !data.distance) return;

    const distanceEl = document.getElementById('current-distance');
    const alertBox = document.getElementById('alert-box');
    const latestTime = document.getElementById('latest-time');
    const latestStatus = document.getElementById('latest-status');

    if (distanceEl) distanceEl.textContent = parseFloat(data.distance).toFixed(1);

    // Update Alert Status
    const isAlert = parseInt(data.alert_triggered) === 1;

    // Status Logic
    if (alertBox) {
        alertBox.className = 'status-text ' + (isAlert ? 'alert' : 'safe');
        alertBox.textContent = isAlert ? 'Proximity Alert' : 'Safe';
    }

    // Detailed Info
    if (latestStatus) latestStatus.textContent = isAlert ? 'Detected' : 'Clear';
    if (latestTime) latestTime.textContent = data.timestamp;
}

function updateLogTable(data) {
    const tbody = document.getElementById('log-body');
    if (!tbody) return;

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

function updateSmartHomeUI(data) {
    const redToggle = document.getElementById('red-toggle');
    const greenToggle = document.getElementById('green-toggle');
    const yellowToggle = document.getElementById('yellow-toggle');
    const redBrightness = document.getElementById('red-brightness');
    const greenBrightness = document.getElementById('green-brightness');
    const yellowBrightness = document.getElementById('yellow-brightness');
    const redBrightnessValue = document.getElementById('red-brightness-value');
    const greenBrightnessValue = document.getElementById('green-brightness-value');
    const yellowBrightnessValue = document.getElementById('yellow-brightness-value');
    const motorSlider = document.getElementById('motor-slider');
    const motorValue = document.getElementById('motor-value');

    if (redToggle) redToggle.checked = parseInt(data.red_state) === 1;
    if (greenToggle) greenToggle.checked = parseInt(data.green_state) === 1;
    if (yellowToggle) yellowToggle.checked = parseInt(data.yellow_state) === 1;

    const redBrightnessVal = data.red_brightness !== undefined ? parseInt(data.red_brightness) : 255;
    const greenBrightnessVal = data.green_brightness !== undefined ? parseInt(data.green_brightness) : 255;
    const yellowBrightnessVal = data.yellow_brightness !== undefined ? parseInt(data.yellow_brightness) : 255;

    if (redBrightness && document.activeElement !== redBrightness) {
        redBrightness.value = redBrightnessVal;
        if (redBrightnessValue) redBrightnessValue.textContent = redBrightnessVal;
    }

    if (greenBrightness && document.activeElement !== greenBrightness) {
        greenBrightness.value = greenBrightnessVal;
        if (greenBrightnessValue) greenBrightnessValue.textContent = greenBrightnessVal;
    }

    if (yellowBrightness && document.activeElement !== yellowBrightness) {
        yellowBrightness.value = yellowBrightnessVal;
        if (yellowBrightnessValue) yellowBrightnessValue.textContent = yellowBrightnessVal;
    }

    if (motorSlider && document.activeElement !== motorSlider) {
        motorSlider.value = parseInt(data.motor_speed);
        if (motorValue) motorValue.textContent = parseInt(data.motor_speed);
    }
}

// Poll every 1 second
setInterval(() => {
    fetchLatestData();
    fetchHistory();
    fetchSmartHomeState();
}, 1000);

// Initial Load
fetchLatestData();
fetchHistory();
fetchSmartHomeState();
