# ESP32 LED Control
Remotely control LED strips via a webinterface hosted locally on the ESP32.

## Flash via USB
platformio.ini
```
upload_port = 192.168.0.108
```
make sure upload_protcol is commented out and not set to "espota".
use the "upload" task to flash.

this only flashes the firmware, to flash the filesystem follow **SPIFFS**.

## SPIFFS
use the "uploadfs" task to flash.
All files in /data will be written to the SPIFFS filesystem.

## Flash via OTA
platformio.ini
```
upload_protocol = espota
upload_port = 192.168.0.108
```
upload_port = ip address of ESP32.
use the "upload" task to flash.
