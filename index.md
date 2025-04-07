---
layout: default
title: PHYS2020 Arduino Quick Start Guide
---

# PHYS2020 Arduino Quick Start Guide

This guide provides instructions for setting up and using various sensors with Arduino for PHYS2020.

## Supported Sensors

* [DS18B20](DS18B20Simple/) - Digital temperature sensor
* [MAX6675](MAX6675SerialLogger/) - Thermocouple amplifier
* [MAX31855](MAX31855SerialLogger/) - High-resolution thermocouple amplifier
* [MPU6050](MPU6050Basic/) - 6-axis accelerometer and gyroscope
* [SD Card Module](SDCardDataLogger/) - Data logging capability

## Getting Started

The Arduino setup process involves:

1. Board Selection
2. Bootloader Configuration
3. Upload Verification

![Arduino Setup Success](screenshots/arduino_setup-success.png)

## Hardware Setup

When working with sensors, proper connection to the breadboard is crucial:

![Breadboard Setup](screenshots/breadboard-and-voltage-rails.jpg)

### Power Supply Considerations

- Each sensor has specific voltage requirements
- Use the breadboard power rails appropriately
- Double-check connections before powering on

![Power Supply Setup](screenshots/breadboard-power-supply.jpg)

## Data Logging

Most examples include serial output capabilities for data logging. For permanent storage, the SD Card module can be used:

- CSV file format
- Real-time data capture
- Easy export to analysis software

## Additional Resources

Example log files and complete wiring diagrams are provided for each sensor configuration.