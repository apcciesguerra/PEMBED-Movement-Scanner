# Troubleshooting & Contingency

If the Arduino IDE fails to compile the multi-file project (e.g., "undefined reference" errors), use this **Manual Merge Method** to combine everything into a single file.

## How to Manually Combine Files

### For Board A (Movement Scanner)
1.  Open `arduino/movement_scanner/movement_scanner.ino`.
2.  Open `arduino/movement_scanner/scanner_logic.ino` in a text editor (Notepad).
3.  **Copy all text** from `scanner_logic.ino`.
4.  **Paste** it at the very bottom of `movement_scanner.ino`.
5.  Save and Upload.

### For Board B (Smart Home)
1.  Open `arduino/smart_home/smart_home.ino`.
2.  Open `arduino/smart_home/smarthome_logic.ino` in a text editor.
3.  **Copy all text** from `smarthome_logic.ino`.
4.  **Paste** it at the very bottom of `smart_home.ino`.
5.  Save and Upload.

---

## Common Errors
*   **"variable was not declared in this scope"**: This usually happens if the logic file tries to use a variable (like `client`) that is defined in the main file, but the compiler order is messed up. Merging them into one file (Logic at the bottom) fixes this 100% of the time.
