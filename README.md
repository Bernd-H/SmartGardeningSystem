# <img align="left" src="https://github.com/Bernd-H/SmartGardeningSystem/blob/main/Pictures/SmartSelect_20220403-142708_PENUP.png" width="50" height="50" /> Smart Gardening System
<p align="center">
  <img src="https://github.com/Bernd-H/SmartGardeningSystem/blob/main/Pictures/SmartGardeningSystem_V5_en.png">
</p>
<br>
This diploma thesis consists of a regulatory control system, which
takes in the local weather forecast, ground moisture and temperature,
in order to, automatically and ideally water plants, with the help of a
valve and a drip hose. The measurement for the ground moisture and
ambient temperature happens in the sensor-module, the switching of
the electrical valve takes place in the actuator-module. Both of these
modules are battery powered and charged via a solar panel or an
optional DC Input. They consist of a low power microcontroller and RFtransceiver
and communicate over a 433MHz wireless connection with
a base-station. The base station consists of a RF-transceiver module
and a Raspberry Pi Zero 2, which decides, according to current sensor
data and weather forecasts, whether to open or close one or more
valves. The base station has a Wi-Fi connection, which enables the
user to set and monitor, parameters and actions via a mobile app in the
local network. With the help of an external server the user can access
these things outside of their network as well.

## Features
- Irrigation zones
- No power or Wi-Fi needed by the sensor or valve modules
- Works also without internet
- End-to-end encrypted
- No data stored online
- Both automatic irrigation (algorithm) and manual irrigation possible
- Shows battery level / signal strength
- Can be operated anywhere via the internet
- Power efficient
- Sensor data stored in database on the basestation

## Source Code

- [Repository for the basestation](https://github.com/Bernd-H/SmartGardeningSystem_Basestation)
- [Repository for the mobile app](https://github.com/Bernd-H/SmartGardeningSystem_MobileApp)
- [Repository for the external server](https://github.com/Bernd-H/SmartGardeningSystem_ExternalServer)
- Code for the sensor/valve station: 

## Current versions

<b>Basestation:</b>&ensp;![](https://img.shields.io/github/release/Bernd-H/SmartGardeningSystem_Basestation)<br/>
<b>Mobile-App:</b>&ensp;![](https://img.shields.io/github/release/Bernd-H/SmartGardeningSystem_MobileApp)<br/>
<b>External Server:</b>&ensp;![](https://img.shields.io/github/release/Bernd-H/SmartGardeningSystem_ExternalServer)<br/>

## License
This project is licensed under the GPLv3 license.
