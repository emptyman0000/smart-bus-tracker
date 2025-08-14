# Smart Bus Tracker

A lightweight, event-driven student tracking system built on ESP32 using a custom RTOS kernel.

## 📌 Overview

This project enables real-time tracking and check-in/out of students on a school bus using the ESP32 platform. It integrates several modules for RFID scanning, GPS location, TFT display, SIM800A GSM communication, and Firebase database.

## ⚙️ Features

- 🧠 Event-driven task system using a lite thread kernel
- 🎫 Student check-in via MFRC522 RFID
- 📍 GPS tracking with periodic updates to Firebase
- 📞 SOS function using SIM800A (call/SMS)
- 🖥️ TFT 3.5" touchscreen interface with keypad input
- 🔔 Audio/visual alerts (buzzer, display)

## 🛠️ Hardware Used

- ESP32 DevKit V1
- MFRC522 RFID reader
- GPS NEO-M10
- SIM800A GSM module
- 3.5" TFT LCD with resistive touch (SPI)
- Speaker + Buzzer + Button
- Firebase Realtime Database (via WiFi)

## 🛠️ Project Structure

```text
src/
├── app/
│   ├── task_display.cpp
│   ├── app_signal.h
│   └── ...
├── common/
│   ├── bus_driver.c
│   └── bus_driver.h
├── driver/
│   ├── button/...
│   ├── led/...
│   └── io_config/...
├── kernel/
│   ├── lt_task.cpp
│   └── lt_message.cpp
|   └── ...
```
## 🚀 Getting Started

1. Clone the repository  
   `git clone https://github.com/emptyman0000/smart-bus-tracker.git`

2. Open with PlatformIO

3. Configure `WiFi`, `Firebase`, and `SIM` credentials in `config.h`

4. Compile and flash to your ESP32 board

## 📷 Screenshots

> (You can optionally insert images or UI GIFs here)

## 📝 License

MIT License — feel free to use, modify, and share.
