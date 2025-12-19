# CanSat-IIST-2025
Participated in CanSat Compitition of IIST's Consientia.
📌 Project Overview

This project presents the design and implementation of a CanSat capable of capturing stabilized video and environmental data during a controlled descent after drone deployment from 100 meters altitude.
The CanSat is fully autonomous, lightweight, modular, and compliant with all IIST CanSat 2025 constraints.

The system integrates sensing, data logging, parachute deployment, and vibration-isolated video capture within a compact structure.

🎯 Mission Objectives

Autonomous deployment from drone at 100 m altitude

Stable descent using a parachute (< 5 m/s landing velocity)

Capture and store:

Temperature & humidity

Acceleration and angular motion

Time-stamped sensor data

Onboard video during descent

Ensure safe landing and data integrity

🧠 System Architecture

Main Controller: Arduino Uno

Sensors & Modules

DHT22 – Temperature & Humidity

MPU6050 – 6-axis Accelerometer + Gyroscope

DS1307 RTC – Real-time clock and failsafe timing

OV7670 Camera – Video capture

microSD Module – Data logging

Power System

Li-Po Battery

Onboard voltage monitoring via ADC pins

Actuation

Servo motor for autonomous parachute deployment

(Refer to system block diagram and architecture description in the design presentation 

)

🏗️ Mechanical Design

Body Shape: Trapezoidal (frustum) for aerodynamic stability

Material: Styrofoam (lightweight + vibration absorbing)

Total Height: ≈ 200 mm

Mass: ≈ 650 g (within 700 g limit)

Internal Sections

Camera Bay (49 mm)

OV7670 mounted on vibration isolator

Bottom aperture for clear video capture

Electronics Bay (69 mm)

Arduino Uno

Sensors and SD card

Ventilation holes for accurate DHT22 readings

Parachute Bay (64 mm)

Servo-based latch mechanism

Spring-assisted deployment

🪂 Parachute System

Canopy Diameter: 90 cm

Material: Cotton cloth

Cord Length: 30 cm

Deployment Method: Servo-actuated internal release

Deployment Logic

Primary trigger: Acceleration spike > 0.3 g (free-fall detection)

Failsafe: RTC-based timed deployment after 2 seconds

Target descent velocity: ~4.9 m/s (validated via OpenRocket simulation)

💾 Software Logic
High-Level Flow

Initialize sensors, RTC, SD card

Wait for release detection (MPU6050)

Start data logging and video capture

Deploy parachute on acceleration trigger

Failsafe deployment via RTC if required

Continue logging until landing detected

Safely close SD card files

Data Logging

Sampling rate: 1 Hz

Logged parameters:

Time stamp

Temperature

Humidity

Acceleration & gyro values

Battery voltage

🧪 Testing Plan
Ground Tests

Sensor calibration

SD card read/write verification

Servo endurance tests (100+ cycles)

Battery discharge tests

Mechanical Tests

Drop tests (5–10 m)

Structural integrity checks

Camera vibration isolation validation

Parachute Tests

Static deployment checks

Drop tests with dummy payload

⚠️ Risk Analysis & Mitigation
Risk	Mitigation
Parachute failure	Servo + RTC failsafe
SD card error	High-quality SD + frequent flush
Camera vibration	Dedicated isolator mount
Power failure	Pre-flight voltage check
Weight limit	Styrofoam structure
📊 Simulation Results

Tool: OpenRocket

Total Mass: 650 g

Predicted Descent Velocity: ~4.90 m/s

Stable vertical descent and upright orientation observed
