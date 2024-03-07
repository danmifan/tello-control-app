# Autonomous navigation for DJI Tello

## Overview

This C++ project is about trying to see how we can add more functionalities (navigation, autonomous flight, stabilization...) to a DJI Tello drone, a 100€ drone equipped with a fixed forward-facing camera and a downward facing infrared sensor.

## Table of contents

## Introduction

## Goals

- Control the drone using a GUI, a gamepad or keyboard
- Indoor navigation between defined waypoints
- 3D visualization of the drone trajectory and attitudes, maybe even of the mapped environment obtained by visual SLAM ?

## Features 

- [x] Drone manual control with GUI and getting drone data back
- [x] Drone video stream decoding
- [x] Face detection and tracking
- [x] Aruco marker detection
- [ ] Use a visual SLAM for navigations

## DJI Tello SDK

### Data sent by the drone

| Name | Description | Unit |
|------|-------------|------|
| roll, pitch, yaw | drone attitude           | degree |
| vgx, vgy, vgz| drone velocity             | cm/s |
| agx, agy, agz| drone acceleration             | cm/s²|
| templ  | lowest temperature             | °C |
| temph  | highest temperature             | °C |
| tof  | time of flight : current drone distance to ground            | cm |
| h  | height above drone starting point         | cm |
| baro  | barometer measurement         | cm |
| bat | drone battery         | % |
| time | amount of time since motor start         | seconds |





## Limitations

## Getting started

### Prerequisites

