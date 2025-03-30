# KCD2_WirelessVibration
Fixes current bug in KCD2, which prevents wireless gamepad vibration from working.

## Installation
Copy _KCD2_WirelessVibration.dll_ into _Win64Shared_ folder of your KCD2 install folder.  
You can find pre-build .dll in [Releases](https://github.com/Tivian/KCD2_WirelessVibration/releases/latest).

## Reporting issues
The mod is inteded to work with a single controller connected to the system, by design it always uses first detected controller.  
It has been tested only with standard **Xbox Series Wireless controller (model 1914) on Windows 11**.  
In case you'd like to open ticket related to any issues with the mod please provide information about your computer and controller you're using.  
If you're experiening any crashes related to this mod, **please include crash report** - the most important files are **.log** and **.dmp**. Without them it's nearly impossible to debug the issue.  
If you'd like the issue to be resolved quicker I recommend to try to reproduce the issue using **-debug.dll** version of the mod - it will create **KCD2_WirelessVibration.log** file in the main game folder with logs from the mod. Make sure to remove the non-debug version first to avoid running two versions of the mod at the same time.  
*The debug version is meant **only** for debugging and I can't guarantee it won't cause any performance issues or excessive log file size if kept for normal operation.*

## Technical Info
As of 2025.02.08 Kingdom Come Deliverance II has a bug in it's GameInput implementation, which relies on deviceFamily member of IGameInputDeviceInfo structure, which requires the gamepad to present itself as Xbox One controller, which is not true in wireless mode.  
As of 2025.03.30 with version 1.2.2 this mod is still required for the wireless vibration to work in KCD2.
