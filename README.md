# 🌟 LED Intensity Controller

This project implements an **LED Intensity Controller** using a microcontrolle. It showcases real-time control of LED brightness using Pulse Width Modulation (PWM), push button inputs, and potentiometer adjustments. Additionally, it features UART communication for logging data to a PC, with Python scripts for data visualization.

---

## ✨ Features

### Core Functionality
- **🔘 ON/OFF Control** (PB1):
  - Toggle LED system on/off.
  - Adjust brightness dynamically using a potentiometer in ON mode.
- **🔄 Blink Mode** (PB2):
  - Blink LED at adjustable intensity (ON mode) or full intensity (OFF mode).
  - Toggle blinking with a second press.
- **📊 Data Logging** (PB3):
  - Transmit LED intensity levels and ADC readings via UART.
  - Generate a CSV log and graphical plots with a Python script.

### Technical Highlights
- **🧠 Finite State Machine (FSM)**: Power-efficient design with state-driven transitions.
- **📈 Python Visualization**: CSV logging and graphical analysis of intensity levels and ADC readings.
- **⚡ Interrupt-Driven Design**: Ensures real-time responsiveness and low power consumption.

---

## 🛠️ Technologies Used
- **💻 C Programming**: Firmware development with MPLAB X IDE and XC16 Compiler.
- **🐍 Python**: Data logging and visualization using:
  - `pyserial` for UART communication.
  - `pandas` and `plotly` for data processing and plotting.
- **🔗 UART Communication**: Serial data transfer between microcontroller and PC.

---

## 📂 Project Structure
```plaintext
log/
├── VoltageADCPlotter.py             # Python script for data logging and plotting
├── Group_26.csv                     # Sample logged data
└── README.pdf                       # Documentation for Python script

src/
├── Makefile                         # Build system for microcontroller firmware
├── ADC.c / ADC.h                    # ADC module for analog input
├── clkChange.c / clkChange.h        # Clock configuration module
├── IOs.c / IOs.h                    # Input/Output initialization and control
├── main.c                           # Main microcontroller firmware
├── PWM.c / PWM.h                    # PWM module for LED control
├── stateMachine.c / stateMachine.h  # Finite State Machine logic
├── timeDelay.c / timeDelay.h        # Time delay utilities
├── UART2.c / UART2.h                # UART communication module

README.md                            # Main project documentation
```

---

## 🧩 Setup Instructions

### 🔌 Hardware Requirements
- Microcontroller (e.g., PIC24 or equivalent).
- LED with a 1kΩ resistor.
- Push buttons (PB1, PB2, PB3).
- Potentiometer for brightness adjustment.
- USB connection for UART communication.

### 🖥️ Software Requirements
- **MPLAB X IDE** and **XC16 Compiler** for firmware development.
- **Python 3.x** with the following libraries:
  ```bash
  pip install pyserial pandas plotly
  ```

---

## 🚀 How to Use

### ⚙️ Microcontroller Setup
1. Clone this repository and open the project in MPLAB X IDE.
2. Connect the hardware components as per the documentation.
3. Compile and upload the firmware to your microcontroller.
4. Power on the microcontroller.

### 🐍 Python Data Logger
1. Open `VoltageADCPlotter.py` and configure the serial port settings:
   ```python
   PORT = 'COM5'
   BAUD_RATE = 4800
   ```
2. Run the script:
   ```bash
   python VoltageADCPlotter.py
   ```
3. Press PB3 on the hardware to start data transmission.
4. View the generated CSV file and plots in the `/log` folder.

---

## 📁 Output Files
- **📄 CSV File**: 
  - Format: `Group_26.csv`
  - Columns: `index`, `timestamp`, `intensity_level`, `adc_reading`
- **📊 Plots**:
  - Intensity vs. Time
  - ADC Readings vs. Time

---

## 🛠️ Troubleshooting

### 💡 LED Issues
- **No Response**:
  - Verify connection to pin 12.
  - Check 1kΩ resistor and ground connections.
- **Flickering**:
  - Ensure PWM frequency is set appropriately.

### 🖧 Data Logging Issues
- **No Data Received**:
  - Verify the correct COM port is selected.
  - Confirm UART baud rate matches.
- **Script Errors**:
  - Ensure required Python libraries are installed.

### 🔘 Button Responsiveness
- **Unresponsive Buttons**:
  - Check connections and interrupts.
  - Verify proper button debouncing implementation.

---

## 👥 Contributors
- **Ahron Ramos**
- **Adrian Co**
- **Zaira Ramji**

---

## 📜 License
This project is licensed under the **MIT License**. See the LICENSE file for details.
This project is created for Fall 2024, University of Calgary.
