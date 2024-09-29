# 🚀 ESP32 Project

## 🎯 Project Goal

The final goal of the project is to create a floating base with all the sensors in a waterproof box. The data will be sent via XBee, passing through XBee relays to collect the information on a final XBee. This data will then be transferred to a Raspberry Pi, stored in a database, and displayed on a website using a database.

## 🛠️ Required Hardware

- ESP32 board
- Connection cables
- Project-specific components (sensors, LEDs, motors, etc.)

## 🔍 Sensor References

### 1. Analog TDS Sensor/Meter

- **Link**: [Analog TDS Sensor - DFRobot](https://fr.farnell.com/en-FR/dfrobot/sen0244/analogue-tds-sensor-meter-kit/dp/3517934)
- **Voltage Range**: 3.3 ~ 5.5 V

### 2. Industrial pH Electrode - SKU: FIT0348

- **Link**: [Industrial pH Electrode - DFRobot](https://wiki.dfrobot.com/Industrial_pH_electrode_SKU_FIT0348_)
- **Voltage Range**: 5.0 V

### 3. Analog Dissolved Oxygen Sensor - SKU: SEN0237

- **Link**: [Gravity Analog Dissolved Oxygen Sensor SEN0237 - DFRobot](https://wiki.dfrobot.com/Gravity__Analog_Dissolved_Oxygen_Sensor_SKU_SEN0237)
- **Voltage Range**: 3.3 ~ 5.5 V

### 4. Analog Electrical Conductivity Sensor - SKU: SEN0451

- **Link**: [Gravity Analog Electrical Conductivity Sensor SEN0451 - DFRobot](https://wiki.dfrobot.com/SKU_SEN0451_Gravity_Analog_Electrical_Conductivity_Sensor_PRO_K_1)
- **Voltage Range**: 3.3 ~ 5.0 V

## 📊 Data to Retrieve

**List of data to retrieve:**

- **TDS (ppm)**: Total Dissolved Solids concentration (parts per million).
- **pH**: Acidity or basicity level of the solution.
- **Dissolved Oxygen (mg/L)**: Concentration of dissolved oxygen in water.
- **Conductivity (µS/cm)**: Electrical conductivity of the solution.
- **Temperature (°C)**: Water temperature (may require a separate temperature sensor).

## 🛠️ Installation

1. Download and install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Clone this repository.
3. Open `main.ino` with the Arduino IDE.

## 🚀 Usage

1. Connect your ESP32 board to your computer via a USB cable.
2. Select the appropriate board type and port in the Arduino IDE.
3. Upload the code to the board by clicking the "Upload" button.

## 🤝 Contributing

Contributions are welcome! Please submit a pull request or open an issue to discuss the changes you want to make.
