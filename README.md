# STM32 Reversing Radar System

## 📌 Project Overview
The **STM32 Reversing Radar** is a **multi-node embedded system** that simulates an automotive reverse parking radar using **STM32 microcontrollers** and a **PC-based GUI**.

The system consists of:
- a **Transmitter (Sensor) node** measuring obstacle distance,
- a **Receiver node** processing and outputting proximity information,
- a **Python GUI** visualizing distance with colored bars similar to a real reversing radar.

The project focuses on embedded communication, modular firmware design, and real-time visualization.

---

## 🖼️ Prototype
![GUI Demo](docs/images/demo.gif)

---

## 🔌 Schematic
<img width="1912" height="687" alt="{E1CBD8BD-B3A7-4D25-B049-17AFF46C16C6}" src="https://github.com/user-attachments/assets/59625fe3-0b96-4c3e-9868-63f5b50d6584" />

---

## 🧩 System Architecture

### Nodes
- **Transmitter Node:** Ultrasonic distance measurement
- **Receiver Node:** Proximity indication (LEDs / buzzer / display)
- **GUI Node:** Serial interface showing distance with green, yellow, and red bars

### Communication
- **Transmitter ↔ Receiver:** CAN bus  
- **STM32 ↔ GUI:** Serial (UART over USB)

---

## 🎯 Objectives
- Real-time distance measurement  
- CAN-based node communication  
- Clear proximity feedback via hardware and GUI  
- Beginner-friendly and well-documented firmware  

---

## 📂 Repository Structure
```
STM32-Reversing-Radar/
├── README.md
├── docs/
├── firmware/
│   ├── transmitter_node/
│   │   └── core/
│   │       ├── Inc/
│   │       └── Src/            
│   └── receiver_node/
│       └── core/
│           ├── Inc/
│           └── Src/  
├── gui/
```

---

## 🛠️ Development Tools & Versions

### ⚠️ Important Notes
- Firmware is compatible **only with ARM Compiler 5** using **Keil µVision 5**. Regenerating HAL code with newer STM32CubeMX versions will disrupt exising code base.
- GUI python code is generated with Qt Designer software using **Qt5 (PyQt5)** and functionality further developed.

### Firmware
- STM32CubeMX: v6.3.0  
- Keil µVision: v5.43.1.0  
- ARM Compiler: v5.06  

### GUI
- Python: 3.10+  
- PyQt5 / Qt5: v5.15  
- pyserial: v3.5  

---

## ▶️ Build & Run

### STM32 Nodes
1. Open the `.uvprojx` file in Keil µVision  
2. Build and flash using ST-Link  

### GUI
```bash
pip install pyqt5 pyserial
python main.py
```

---

## 📄 Documentation
- Doxygen HTML: `docs/html/index.html`
- Doxygen PDF: `docs/latex/refman.pdf`

---

## 👤 Author
**Mohammed Halim**  
Embedded Systems Engineer  
GitHub: https://github.com/medHalim
Linkedin: https://www.linkedin.com/in/mohammed-halim/
