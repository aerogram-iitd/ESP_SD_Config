/*
   Connect the SD card to the following pins:

     SD Card | ESP32
      D2       -
      D3       SS(GPIO5)
      CMD      MOSI(GPIO23)
      VSS      GND
      VDD      3.3V
      CLK      SCK(GPIO18)
      VSS      GND
      D0       MISO(GPIO19)
      D1       -
*/
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "EEPROM.h"
#include <WiFi.h>


char ssid[32];
char password[64];
int address_ssid = 0;
int address_password = sizeof(ssid) + 1;

/*
 *@brief This function reads the file and save the credentials in eeprom
 *@param path file name that needs to be read
 */
void readFile(fs::FS &fs, const char * path) {
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }
  else {
    char WIFI_SSID[32] ;
    char WIFI_PASSWORD[64];
    char buffer[40];
    byte index = 0;

    Serial.println("Read from file: ");
    while (file.available()) {
      char c = file.read();
      if (c == '\n' || c == '\r') // Test for <cr> and <lf>
      {
        parseAndSave(buffer , WIFI_SSID , WIFI_PASSWORD);
        index = 0;
        buffer[index] = '\0'; // Keep buffer NULL terminated
      }
      else
      {
        buffer[index++] = c;
        buffer[index] = '\0'; // Keep buffer NULL terminated
      }
    }
    file.close();
    if (WIFI_SSID[0] == '\0' || WIFI_PASSWORD[0] == '\0')
      return;
    else {
      EEPROM.writeString(address_ssid, WIFI_SSID);
      EEPROM.writeString(address_password, WIFI_PASSWORD);
      EEPROM.commit();
      Serial.println(WIFI_SSID);
      Serial.println(WIFI_PASSWORD);
      deleteFile(SD, "/config.txt");
    }
  }
}

/*
* @brief This function parses the string and save them in the variables
* @param buff           string from which data needs to parsed
*        WIFI_SSID      holds wifi ssid after being parsed from buff
*        WIFI_PASSWORD  holds wifi password after being parsed from buff
*/
void parseAndSave(char *buff , char *WIFI_SSID, char *WIFI_PASSWORD)
{
  char* token;
  char* token1;
  char* temp_var;

  if ( strncmp("WIFI_SSID:", buff, 10) == 0) {
    token = strtok(buff, ",");
    token1 = strtok(token, ":");

    while (token1 != 0) {
      temp_var = token1;
      token1 = strtok(0, ":");
    }
    strcpy(WIFI_SSID , temp_var);
  }
  else if ( strncmp("WIFI_PASSWORD:", buff, 10) == 0) {
    token = strtok(buff, ",");
    token1 = strtok(token, ":");

    while (token1 != 0) {
      temp_var = token1;
      token1 = strtok(0, ":");
    }
    strcpy (WIFI_PASSWORD , temp_var);
  }
}
void deleteFile(fs::FS &fs, const char * path) {
  Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path)) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}


void setup() {
  // Intialize serial communication at 115200 baud rate
  Serial.begin(115200);
  // Intialize EEPROM with 1000 bytes size
  if (!EEPROM.begin(1000)) {
    Serial.println("Failed to initialise EEPROM");
    Serial.println("Restarting...");
    delay(1000);
    ESP.restart();
  }
  // Intialize SD card 
  if (!SD.begin()) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
  readFile(SD, "/config.txt");
  // Read Credentils from EEPROM
  strcpy(ssid, EEPROM.readString(address_ssid).c_str());
  strcpy(password, EEPROM.readString(address_password).c_str());
  
  Serial.println("Connecting to ");
  Serial.println(ssid);
  Serial.println(password);
  // Intilize wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}

void loop() {

}
