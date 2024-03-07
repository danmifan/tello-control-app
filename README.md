# Autonomous navigation for DJI Tello

## Overview

This C++ project is about trying to see how we can add more functionalities (navigation, autonomous flight, stabilization...) to a DJI Tello drone, a ~100€ drone equipped with a fixed forward-facing camera and a downward facing infrared sensor.

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


### Commands

All the move and rotate commands (up,down,left,right,forward,cw...) are way too slow, the drone is unresponsive and cannot receive anymore command until the movement has been completed. The RC commands on the other hand are fast to execute and the drone can receive new commands (one every ~10ms), this is why we're only using RC commands to control the drone in this program.

| Name | Description |
|------|-------------|
| command | Enter SDK mode |
| takeoff | Auto takeoff |
| land | Auto landing |
| streamon | Enable video streaming |
| streamoff | Disable video streaming |
| emergency | Stops all motors |
| up,down,left,right,forward,back x | Move the drone in the desired direction, x = 20-500 cm |
| cw,ccw x | Rotate clockwise or anti clockwisze, x=1-360 degrees |
| flip x | Flips in x direction, x = "l"=left, "r", "f", "b" |
| go x y z speed | Fly to x y z at speed (cm/s) x,y,z = -500-500, speed = 10-200 |
| stop  | Hovers |
| curve x1 y1 z1 x2 y2 z2 speed | Fly at a curve... |
| speed x  | Set speed to x cm/s, x=10-100 ?? |
| rc a b c d  | Remote controller control : a = left/right (-100-100), b = forward/backward(-100-100), c = up/down(-100-100), d = yaw(-100,100) |

### Drone state

Data string received : 
``` C
“pitch:%d;roll:%d;yaw:%d;vgx:%d;vgy%d;vgz:%d;templ:%d;temph:%d;tof:%d;h:%d;bat:%d;baro:%
.2f; time:%d;agx:%.2f;agy:%.2f;agz:%.2f;\r\n”
```

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

