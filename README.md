# ESP32 Smart Door Lock System

## 1. Project Overview

The **ESP32 Smart Door Lock System** is an embedded security project designed to provide password-based door access using an ESP32 microcontroller. The system integrates a 4×4 matrix keypad, servo motor, I2C LCD display, buzzer, and status LEDs to create an automated electronic door-locking mechanism.

Users can enter a predefined security password through the keypad to unlock the door. The ESP32 verifies the entered password and controls the servo motor to operate the locking mechanism. The LCD provides real-time feedback regarding access status, while the LEDs and buzzer indicate successful or unsuccessful authentication.

To enhance security, the system incorporates an automatic lockout mechanism that temporarily disables password entry for 30 seconds after three consecutive incorrect password attempts.

This project demonstrates the practical implementation of microcontroller-based access control, peripheral interfacing, password authentication, and automated security mechanisms.

## 2. Key Features

- **Password-Based Authentication:** Provides secure access through a predefined numerical password entered using a 4×4 matrix keypad.
- **Automated Door Locking:** Uses an ESP32-compatible servo motor to control the door-locking mechanism.
- **LCD Status Display:** Displays system instructions, password entry status, access permissions, and lockout notifications through a 16×2 I2C LCD.
- **Access Granted Notification:** Activates the green LED and generates a confirmation beep when the correct password is entered.
- **Access Denied Notification:** Activates the red LED and generates an error sound when an incorrect password is entered.
- **Automatic Relocking:** Automatically returns the servo motor to its locked position after four seconds.
- **Security Lockout:** Temporarily disables password entry for 30 seconds after three consecutive incorrect attempts.
- **Password Clearing:** Allows users to clear the currently entered password using the `*` key.
- **Password Submission:** Uses the `#` key to submit the entered password for verification.
- **Visual and Audio Feedback:** Combines LEDs, LCD messages, and buzzer notifications to communicate system events.

## 3. Hardware Components

The following hardware components are required to implement the project:

| Component | Quantity | Purpose |
|---|---:|---|
| ESP32 Development Board | 1 | Main microcontroller and system controller |
| 4×4 Matrix Keypad | 1 | Password input interface |
| SG90 Servo Motor | 1 | Door-locking mechanism |
| 16×2 I2C LCD Display | 1 | Displays system messages and status |
| Active/Passive Buzzer | 1 | Audio notifications and alerts |
| Red LED | 1 | Indicates locked or denied-access status |
| Green LED | 1 | Indicates successful authentication |
| 220Ω Resistors | 2 | Current limiting for LEDs |
| Breadboard | 1 | Circuit prototyping |
| Jumper Wires | As required | Electrical connections |
| External 5V Power Supply | 1 | Recommended for powering the servo motor |

**Note:** Use a suitable regulated power supply for the servo motor. Connect the external supply ground to the ESP32 ground to establish a common reference. Verify the voltage requirements of each peripheral before connecting it.

## 4. Circuit Wiring Instructions

Connect the hardware components to the ESP32 according to the following pin configuration.

### 4.1 Servo Motor Connections

| Servo Pin | ESP32 / Power Connection |
|---|---|
| Signal (Orange/Yellow) | GPIO 18 |
| VCC (Red) | External regulated 5V |
| GND (Brown/Black) | Common GND |

The servo motor is controlled through GPIO 18. The programmed positions are:

- Locked position: 0°
- Unlocked position: 90°

The servo returns to the locked position after the programmed unlocking interval.

### 4.2 4×4 Matrix Keypad Connections

The keypad uses eight GPIO pins for four rows and four columns.

| Keypad Connection | ESP32 GPIO |
|---|---|
| Row 1 (R1) | GPIO 13 |
| Row 2 (R2) | GPIO 12 |
| Row 3 (R3) | GPIO 14 |
| Row 4 (R4) | GPIO 27 |
| Column 1 (C1) | GPIO 26 |
| Column 2 (C2) | GPIO 25 |
| Column 3 (C3) | GPIO 33 |
| Column 4 (C4) | GPIO 32 |

The keypad is used to enter the security password and submit or clear the entered characters.

### 4.3 LCD Display Connections

The 16×2 LCD communicates with the ESP32 through the I2C interface.

| LCD Pin | ESP32 Connection |
|---|---|
| VCC | Compatible regulated supply |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

**I2C Configuration:**
- Display Type: 16×2 LCD
- I2C Address: `0x27`
- Communication Protocol: I2C

If your LCD module uses a different I2C address, update the address in the Arduino source code accordingly.

### 4.4 LED Connections

| LED | ESP32 GPIO | Purpose |
|---|---|---|
| Red LED | GPIO 4 | Locked state and access denied |
| Green LED | GPIO 5 | Access granted |

Connect a 220Ω current-limiting resistor in series with each LED.

### 4.5 Buzzer Connections

| Buzzer Pin | ESP32 GPIO |
|---|---|
| Positive (+) | GPIO 19 |
| Negative (-) | GND |

The buzzer provides different audio notifications for successful authentication, incorrect passwords, and security lockout events.

### 4.6 Complete GPIO Pin Reference

| Peripheral | GPIO Pin |
|---|---|
| Servo Motor | 18 |
| Buzzer | 19 |
| Red LED | 4 |
| Green LED | 5 |
| Keypad Row 1 | 13 |
| Keypad Row 2 | 12 |
| Keypad Row 3 | 14 |
| Keypad Row 4 | 27 |
| Keypad Column 1 | 26 |
| Keypad Column 2 | 25 |
| Keypad Column 3 | 33 |
| Keypad Column 4 | 32 |
| LCD SDA | 21 |
| LCD SCL | 22 |

**Wiring Precautions:**
- Avoid powering the servo directly from the ESP32 3.3V output.
- Ensure that all components share a common ground.
- Check the ESP32 GPIO voltage limitations before connecting peripherals.
- Verify the keypad row and column order before powering the circuit.
- Confirm the LCD I2C address before uploading the program.

## 5. Software Dependencies

The project is developed using the Arduino framework and requires the following software and libraries.

### 5.1 Development Environment

- Arduino IDE (Version 2.x recommended)
- ESP32 Board Support Package
- USB data cable for programming the ESP32

### 5.2 Required Arduino Libraries

Install the following libraries through the Arduino IDE Library Manager.

| Library | Purpose |
|---|---|
| `Keypad` | Reads input from the 4×4 matrix keypad |
| `LiquidCrystal_I2C` | Controls the I2C LCD display |
| `ESP32Servo` | Provides servo motor control on ESP32 |

The following libraries are included with the ESP32 Arduino environment or its standard toolchain:

- `Wire.h` – I2C communication.
- `Arduino.h` – Core Arduino functions, GPIO control, timing, and serial communication.

### 5.3 Configuration Parameters

The primary configuration values are defined in the Arduino source code.

| Parameter | Default Value |
|---|---|
| Default Password | `****` |
| Maximum Password Length | 8 characters |
| Maximum Failed Attempts | 3 |
| Lockout Duration | 30 seconds |
| Servo Locked Position | 0° |
| Servo Unlocked Position | 90° |
| Door Unlock Duration | 4 seconds |
| LCD I2C Address | `0x27` |
| Serial Baud Rate | 115200 |

## 6. Installation Steps

Follow these instructions to set up and run the ESP32 Smart Door Lock System.

### Step 1: Install Arduino IDE

Download and install the Arduino IDE from the official website:

https://www.arduino.cc/en/software

### Step 2: Configure ESP32 Board Support

1. Open Arduino IDE.
2. Navigate to **File → Preferences**.
3. Add the official Espressif ESP32 board package URL to Additional Boards Manager URLs:

   `https://espressif.github.io/arduino-esp32/package_esp32_index.json`

4. Open **Tools → Board → Boards Manager**.
5. Search for `ESP32`.
6. Install the ESP32 board package by Espressif Systems.

### Step 3: Install Required Libraries

Open **Sketch → Include Library → Manage Libraries**.

Search for and install:

1. Keypad
2. LiquidCrystal_I2C
3. ESP32Servo

Ensure that all required libraries are installed before compiling the project.

### Step 4: Connect the Hardware

1. Assemble the circuit using the GPIO pin configuration provided in the wiring section.
2. Connect the keypad, LCD, servo motor, buzzer, and LEDs.
3. Verify all power and ground connections.
4. Ensure the servo has an appropriate external power supply.
5. Connect the ESP32 development board to your computer using a USB cable.

### Step 5: Open the Project

1. Download or clone this repository.
2. Open the Arduino source file (`sketch.ino`) in Arduino IDE.
3. Review the GPIO definitions and password configuration.
4. Confirm that the LCD I2C address matches your hardware.

### Step 6: Select the ESP32 Board and Port

In Arduino IDE:

1. Navigate to **Tools → Board**.
2. Select your compatible ESP32 development board.
3. Navigate to **Tools → Port**.
4. Select the COM port or serial device corresponding to your ESP32.

### Step 7: Compile and Upload

1. Click the Verify button to compile the sketch.
2. Resolve any missing-library or compilation errors.
3. Click Upload to flash the program onto the ESP32.
4. If required, press and hold the ESP32 BOOT button during the connection phase.
5. Wait for the upload process to complete.

### Step 8: Test the System

After uploading:

1. Power on the ESP32.
2. Confirm that the LCD displays `SMART DOOR LOCK` and `Enter Password:`.
3. Enter the default password: `2226`.
4. Press `#` to submit the password.
5. Verify that the green LED turns on, the buzzer generates a confirmation sound, and the servo moves to the unlocked position.
6. Wait four seconds and verify that the servo returns to the locked position.
7. Enter an incorrect password three consecutive times.
8. Verify that the system activates the 30-second lockout period.
9. Test the `*` key to confirm that it clears the entered password.

## Project Operation

The system follows a straightforward authentication and access-control sequence:

1. The ESP32 initializes the LCD, keypad, servo motor, buzzer, and LEDs.
2. The door starts in the locked position.
3. The user enters a numerical password using the keypad.
4. The `#` key submits the password for verification.
5. If the password is correct, the servo unlocks the door and the system provides confirmation through the LCD, green LED, and buzzer.
6. After four seconds, the servo automatically returns to the locked position.
7. If the password is incorrect, the system displays an access-denied message and increments the failed-attempt counter.
8. After three consecutive incorrect attempts, the system activates a 30-second lockout.
9. Once the lockout expires, the failed-attempt counter resets and password entry becomes available again.

## Security Considerations and Limitations

- The default password is stored in the source code and should be changed before deployment.
- The current implementation does not provide encrypted password storage.
- The lockout mechanism is implemented in software and resets when the ESP32 restarts.
- The system does not include a door-position sensor or physical confirmation that the door has successfully locked.
- The servo mechanism should be mechanically designed to prevent unauthorized physical access.
- Additional security measures, such as persistent lockout tracking, encrypted credential storage, and tamper detection, are recommended for real-world installations.

## Future Enhancements

Potential improvements include:

- Password modification through the keypad.
- Multiple authorized user passwords.
- RFID-based authentication.
- Mobile application integration using Wi-Fi or Bluetooth.
- Remote door monitoring and control.
- Door-position detection using magnetic or limit sensors.
- Event logging and access history.
- Improved protection against power interruptions and unauthorized tampering.

## License

This project is intended for educational, experimental, and personal development purposes.

You may modify and extend the source code according to your requirements. If you plan to distribute this project, consider adding an explicit open-source license to the repository.
