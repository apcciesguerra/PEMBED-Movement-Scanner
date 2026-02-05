<div id="tab-smarthome" class="tab-content">
    <div class="dashboard-grid">
        <!-- Light Controls -->
        <div class="card">
            <h2>Smart Lights</h2>

            <div class="control-display">
                <span class="label">Red</span>
                <label class="switch">
                    <input type="checkbox" id="red-toggle">
                    <span class="slider round"></span>
                </label>
            </div>

            <div class="control-display">
                <span class="label">Green</span>
                <label class="switch">
                    <input type="checkbox" id="green-toggle">
                    <span class="slider round"></span>
                </label>
            </div>

            <div class="control-display">
                <span class="label">Yellow</span>
                <label class="switch">
                    <input type="checkbox" id="yellow-toggle">
                    <span class="slider round"></span>
                </label>
            </div>
        </div>

        <!-- Motor Control -->
        <div class="card">
            <h2>Fan Speed (Motor)</h2>
            <div class="control-display">
                <input type="range" min="0" max="255" value="0" class="range-slider" id="motor-slider">
                <div class="metric-display">
                    <span class="value" id="motor-value">0</span>
                    <span class="unit">PWM</span>
                </div>
            </div>
        </div>
    </div>
</div>