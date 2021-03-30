/*
   Title: SMART TRANSMISSION LINE FAULT DETECTION
   Last Edit: 18/03/2021
   Serial data Send: 1 
   Serial data Get: <230*1>
   URL : http://flextronteckiot.atwebpages.com/CIT_Fault/Live_Fault_Check.html
   Developer: Rajan K
*/


#include <ESP8266WiFi.h>
#include<FirebaseArduino.h>

#define FIREBASE_HOST "flextron-tech-iot-projects.firebaseio.com"
#define FIREBASE_AUTH "avQrceoBXd0Dg3Bt7HI7X8xHVCnK37iABeOzjIf5"

#define WIFI_SSID       "Flextronix_4G"
#define WIFI_PASSWORD   "9543522551"


void setup()
{
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  //Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    //Serial.print(".");
    delay(500);
  }
  //Serial.println();
  //Serial.print("connected:");
  //Serial.println(WIFI_SSID);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}
void loop()
{
  String R_Input_Data = "";
  bool flag = 1;
  while (Serial.available() > 0) {
    R_Input_Data = Serial.readString();
    delay(10);
    flag = 0;
  }

  if (flag == 0)
  {
    flag = 1;
    int i = 0, volt = 0, fault = 0;
    String Split_Data = "";

    while (R_Input_Data[i] != '\0')
    {
      if (R_Input_Data[i] == '<')
      {
        i ++;
        while (R_Input_Data[i] != '*') {
          Split_Data += String(R_Input_Data[i]);
          i ++;
        }
      }
      i++;
    }
    volt = Split_Data.toInt();
    i = 0;
    Split_Data = "";

    while (R_Input_Data[i] != '\0')
    {
      if (R_Input_Data[i] == '*')
      {
        i ++;
        while (R_Input_Data[i] != '>') {
          Split_Data += String(R_Input_Data[i]);
          i ++;
        }
      }
      i++;
    }
    fault = Split_Data.toInt();

    //Serial.print("volt= "); Serial.println(volt);
    //Serial.print("fault= "); Serial.println(fault);
    Firebase.setInt("CIT_Fault/Volt", volt);
    Firebase.setInt("CIT_Fault/Fault", fault);
  }
  int fb_RS = Firebase.getInt("CIT_Fault/Reset");
  if (fb_RS == 1)
  {
    Serial.print(fb_RS);
    delay(500);
    Serial.print(fb_RS);
    Firebase.setInt("CIT_Fault/Reset",0);
  }
}
