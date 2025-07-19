#include <ESP8266WiFi.h>       
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FS.h>                


const char* ssid = "Big_Sh@rk$";
const char* password = "bigbullnei";


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define SAVE_INTERVAL 3600 
time_t lastSaveTime = 0;
time_t lastEp = 0;
unsigned long lastM = 0;
unsigned long lastDisplayUpdate = 0;
const unsigned long displayInterval = 1000;


WiFiUDP udp;
NTPClient timeClient(udp, "pool.ntp.org", 6 * 3600);


String days[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
String months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", 
                     "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};


unsigned long millisDiff(unsigned long present, unsigned long past) {
  if (present >= past) return present - past;
  return (0xFFFFFFFF - past) + present + 1;
}

void setup() {
  Serial.begin(9600);
  delay(1000);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED not found"));
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount SPIFFS");
    SPIFFS.format();
    SPIFFS.begin();
  } else {
    readFile();
  }

  connectWiFi();
  timeClient.begin();
}

void loop() {
  unsigned long now = millis();
  if (WiFi.status() == WL_CONNECTED) {
    timeClient.update();
    time_t epochTime = timeClient.getEpochTime();
    unsigned long pMillis = now;

    if (epochTime > lastEp + millisDiff(pMillis, lastM) / 1000 - 5) {
      lastEp = epochTime;
      lastM = pMillis;

      if (epochTime - lastSaveTime >= SAVE_INTERVAL) {
        saveFile(lastEp);
        lastSaveTime = epochTime;
      }
    }
  } else {
    Serial.println("fallback mode active");
  }

  if (millisDiff(now, lastDisplayUpdate) >= displayInterval) {
    lastDisplayUpdate = now;

    time_t presentTime = lastEp + millisDiff(now, lastM) / 1000;
    struct tm *ptm = gmtime((time_t *)&presentTime);

    char timeStr[12];
    int hour = ptm->tm_hour;
    String amPm = "AM";

    if (hour >= 12) {
      amPm = "PM";
      if (hour > 12)
      hour -= 12;
    }
   else if (hour == 0) hour = 12;

    sprintf(timeStr, "%02d:%02d:%02d %s", hour, ptm->tm_min, ptm->tm_sec, amPm.c_str());

   char dateStr[20];
   sprintf(dateStr, "%02d %s %04d", ptm->tm_mday, months[ptm->tm_mon].c_str(), ptm->tm_year + 1900);


    String dayName = days[ptm->tm_wday];
    Serial.println(timeStr) ;
    Serial.println(dateStr) ;
    Serial.println(dayName) ;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.println(timeStr);

    display.setTextSize(1);
    display.setCursor(0, 25);
    display.println(dateStr);

    display.setCursor(0, 40);
    display.println("Day: " + dayName);
    display.display();
  }
}

void connectWiFi() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Connecting WiFi...");
  display.display();

  Serial.print("Connecting WiFi");
  WiFi.begin(ssid, password);
  unsigned long cTime = millis();

  while (WiFi.status() != WL_CONNECTED && millisDiff(millis(), cTime) < 20000) {
    delay(500);
    display.print(".");
    display.display();
  }

  if (WiFi.status() == WL_CONNECTED) {
    display.clearDisplay();
    display.println("WiFi connected!");
    display.display();
  } else {
    display.clearDisplay();
    display.println("WiFi failed.");
    display.println("Will use last saved time");
    display.display();
  }
}

void saveFile(time_t presentTime) {
  File file = SPIFFS.open("/time.txt", "w");
  if (!file) {
    Serial.println("Failed to open time.txt");
    return;
  }
  file.write((uint8_t*)&presentTime, sizeof(time_t));
  file.close();
  Serial.printf("Saved time to SPIFFS: %lu\n", presentTime);
}

void readFile() {
  File file = SPIFFS.open("/time.txt", "r");
  if (!file) {
    Serial.println("No time.txt found");
    return;
  }

  time_t storedTime = 0;
  file.read((uint8_t*)&storedTime, sizeof(time_t));
  file.close();

  if (storedTime > 1000000000) {
    lastEp = storedTime;
    lastM = millis();
    Serial.printf("Loaded time from SPIFFS: %lu\n", lastEp);
  } else {
    Serial.println("Stored time invalid");
  }
}
