# ESP_SD_Config

## Introduction

ESP_SD_CONFIG is an Arduino sketch that reads WiFi configuration from SD card and store them in the EEPROM of ESP module

Using this sketch we don't have to hard-code WiFi credentials in the firmware. Instead we have file config.txt on an SD card, that has credentials store like this :,

```json
{
WIFI_SSID:your_ssid,
WIFI_PASSWORD:your_password
}
```

At the start of the program, ESP module checks whether configuration file is present or not. If present it loads the credentials from the file and saves it in EEPROM and then deletes the file from the SD card. Then it reads the WiFi credentials from EEPROM and begins connecting to it.

 

### Connection

------

- The Interface between the SD CARD module and  ESP32 is as follows:


```
     SD Card | ESP32
      CS       SS(GPIO5)
      CMD      MOSI(GPIO23)
      VSS      GND
      VDD      3.3V
      CLK      SCK(GPIO18)
      VSS      GND
      D0       MISO(GPIO19)
```

- Format SD card to FAT16/FAT32.


- Make a file in SD card as ***config.txt*** and edit the WiFi credentials in the format shown above.


- Save the file and attach the SD card to the ESP module.


 

