# ESP32 NTP Clock with OLED Display

This project is an ESP32-based digital clock that synchronizes time from an NTP (Network Time Protocol) server and displays the current time, date, and day of the week on a 128x64 OLED display. It connects to WiFi, fetches accurate time data, and updates the display every second.

<img width="1741" height="832" alt="Screenshot From 2025-07-19 04-32-35" src="https://github.com/user-attachments/assets/aad83205-6d9e-4b6a-8583-aad2e568b4c7" />


## Features

- Connects to WiFi with automatic retry on failure.
- Synchronizes time from a Bangladesh NTP server (`bd.pool.ntp.org`) with a +6 hour timezone offset.
- Displays current time in HH:MM:SS format.
- Displays current date with day, month name, and year.
- Shows the day of the week as a three-letter abbreviation (Sun, Mon, etc.).
- Uses an SSD1306 OLED display (128x64) via I2C.
- Prints connection progress and status on OLED.
- Includes debug messages via Serial Monitor (commented out by default).

---

## Hardware Requirements

- **ESP32 development board**
- **SSD1306 OLED display (128x64 pixels)**
- Wiring for I2C:
  - SDA to ESP32 SDA pin (usually GPIO 21)
  - SCL to ESP32 SCL pin (usually GPIO 22)
- USB cable for programming and power

---

## Software Requirements

- Arduino IDE or compatible ESP32 development environment
- ESP32 board support installed in Arduino IDE
- Required libraries (can be installed via Library Manager):
  - `Adafruit SSD1306`
  - `Adafruit GFX`
  - `NTPClient`

---

## How to Use

1. **Clone or download** this repository.

2. Open the sketch in Arduino IDE.

3. Modify the WiFi credentials in the code:

   ```cpp
   const char* ssid = "Your_SSID";
   const char* password = "Your_WIFI_Password";

