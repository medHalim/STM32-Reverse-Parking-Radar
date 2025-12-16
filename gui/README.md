# STM32 Reversing Radar GUI

A Python PyQt5 GUI to display radar distance readings from an STM32-based reversing radar system.

Ensure the radar is connected to the correct COM port (COM8 by default).

## Features
- Real-time distance display using QLCDNumber.
- Color-coded visual indicators (RED, YELLOW, GREEN LEDs).
- Asynchronous serial communication for smooth GUI updates.
- Clean modular code structure for easy maintenance and extension.

## Folder Structure
- `assets/` : LED images for the display
- `ui_generated.py` : Qt Designer generated UI
- `ui_main.py` : Radar display logic
- `serial_worker.py` : Serial reading thread
- `distance_logic.py` : Distance-to-zone mapping
- `main.py` : Entry point for the application

## Requirements
```bash
pip install -r requirements.txt
