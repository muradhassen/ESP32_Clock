#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


const char* ssid = "Big_Sh@rk$";   //wifi ssid
const char* password = "bigbullnei";  // wifi pass


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


WiFiUDP UP;
NTPClient timeClient(UP, "bd.pool.ntp.org", 6 * 3600); 


String days[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
String months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", 
                     "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

void setup() {
  Serial.begin(115200);
  delay(1000);  
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED not found"));
    while(true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

 
  connectWiFi();

  timeClient.begin();
  
  // Serial.println("NTP Client started");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected - retrying...");
    delay(2000);
    connectWiFi(); 
    return;
  }

  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime);

  String timeStr = timeClient.getFormattedTime();

  int day = ptm->tm_mday;
  String monthName = months[ptm->tm_mon];
  int year = ptm->tm_year + 1900;
  String dateStr = String(day) + " " + monthName + " " + String(year);

  String dayName= days[timeClient.getDay()];
//  Serial.println(timeStr);
//  Serial.println(dateStr);
//  Serial.println(dayName);
//  Serial.println();

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
  delay(1000);
}

void connectWiFi() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Connecting WiFi...");
  display.display();

  Serial.print("Connecting WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);

    unsigned long cTime = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - cTime < 20000)
    {
      delay(500);
      display.print(".");
      display.display();
      
      // Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
      // Serial.println("\nWiFi connected!");
      // Serial.print("IP address: ");
      // Serial.println(WiFi.localIP());

      
      display.clearDisplay();
      display.println("WiFi connected!");
      display.display();
      break; 
    }
    else {
      // Serial.println("\n connection Failed Restart 2 sec ..");
      
      display.clearDisplay();
      display.println("WiFi connection failed.");
      display.println("Trying to connected again ...");
      display.display();
      delay(2000);  
    }
  }
}
