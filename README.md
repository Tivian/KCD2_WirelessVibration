# KCD2_WirelessVibration
Fixes current bug in KCD2, which prevents wireless gamepad vibration from working.

## Installation
Copy _KCD2_WirelessVibration.dll_ into _Win64Shared_ folder of your KCD2 install folder.  
You can find pre-build .dll in [Releases](https://github.com/Tivian/KCD2_WirelessVibration/releases).

## Technical Info
As of 2025.02.08 Kingdom Come Deliverance II has a bug in it's GameInput implementation, which relies on deviceFamily member of IGameInputDeviceInfo structure, which requires the gamepad to present itself as Xbox One controller, which is not true in wireless mode.