# Cortex-M4 Industrial Machine Health Monitoring System

An embedded machine health monitoring system based on the STM32F446RE Cortex-M4 microcontroller that monitors temperature and vibration in real time, automatically controls a cooling fan, regulates motor speed using PWM, and generates alerts under abnormal operating conditions.

## Overview

Industrial machines can experience faults such as overheating and excessive vibration, which may lead to mechanical damage and unexpected downtime. This project implements a real-time embedded monitoring and control system that continuously measures temperature and vibration and takes appropriate control actions based on predefined threshold levels.

The system uses an STM32F446RE Cortex-M4 microcontroller to process sensor data and control the cooling fan, motor, and buzzer.

## Key Features

- Real-time temperature monitoring using DHT11
- Analog vibration monitoring using SW-420 vibration sensor
- Automatic cooling fan activation based on temperature thresholds
- PWM-based motor speed control according to vibration levels
- Buzzer alerts for abnormal vibration 
- Threshold-based machine condition monitoring
- Real-time display of sensor readings and system status through the Serial Monitor

## System Working

### Temperature Monitoring

The DHT11 sensor continuously measures temperature.

| Temperature | Condition |
|---|---|
| < 25°C | Normal |
| 25°C – 30°C | Moderate |
| 30°C – 35°C | High |
| > 35°C | Critical |

When the temperature crosses the defined threshold, the STM32 activates the cooling fan through a relay to reduce the temperature.

### Vibration Monitoring

The SW-420 vibration sensor provides an analog signal that is read by the STM32.

| Vibration Value | Condition |
|---|---|
| 0 – 300 | Low |
| 300 – 600 | Medium |
| 600 – 900 | High |
| 900 – 1023 | Critical |

As the vibration level increases, the motor speed is adjusted using PWM to reduce mechanical stress.

### Motor Speed Control

The motor is controlled through a MOSFET using the PWM output of the STM32.

| PWM Value | Approx. Duty Cycle | Motor Condition |
|---|---:|---|
| 220 | 86% | High Speed |
| 160 | 62% | Medium Speed |
| 90 | 35% | Low Speed |
| 0 | 0% | Motor OFF |

This allows the motor speed to be adjusted according to the detected vibration level.

## Hardware Components

- STM32F446RE Cortex-M4 Microcontroller
- DHT11 Temperature Sensor
- SW-420 Vibration Sensor
- 5V DC Motor / Water Pump Motor
- 5V Cooling Fan
- Relay Module
- IRLZ44N MOSFET
- Buzzer
- 10 kΩ Resistor
- Breadboard and Jumper Wires

## Pin Configuration

| STM32 Pin | Component | Function |
|---|---|---|
| PA0 | Vibration Sensor | Analog vibration input |
| PA1 | DHT11 | Temperature input |
| PA4 | Relay | Cooling fan control |
| PA8 | MOSFET | PWM motor speed control |
| PB0 | Buzzer | Alarm output |
