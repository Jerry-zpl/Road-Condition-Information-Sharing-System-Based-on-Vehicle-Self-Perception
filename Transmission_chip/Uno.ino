#define GPRMC_TERM "$GPRMC,"    //定义要解析的指令，因为这条指令包含定位和时间信息
#include <stdio.h>
#include <SoftwareSerial.h>

char nmeaSentence[68];
String latitude;    //纬度
String longitude;   //经度
String Deep;
String data;
double a;
double b;
double d;
double temp;
SoftwareSerial HC12(10, 11);

void setup() {
  Serial.begin(9600); 
  HC12.begin(9600); 
  delay(10); 
}
void loop() {
  for (unsigned long start = millis(); millis() - start < 1000;)  //一秒钟内不停扫描GPS信息
  {
      while (HC12.available()) 
      {  
        char c=HC12.read();
        
        switch (c)
        {
          case '$':          //若是$，则说明是一帧数据的开始
            HC12.readBytesUntil('*', nmeaSentence,100);   //读取接下来的数据，存放在nmeaSentence字符数组中，最大存放67个字节
            //Serial.println(nmeaSentence);
            latitude = parseGprmcLat(nmeaSentence); //获取纬度值
            longitude = parseGprmcLon(nmeaSentence); //获取经度值
            Deep=parseGprmcDeep(nmeaSentence);//获取水深数据
            if (latitude > "" && longitude > "" && Deep > "")    //当不是空时候打印输出
          {
            a=latitude.toDouble();
            b=longitude.toDouble();
            data=Deep;
            Serial.print("$GPRMC,");
            Serial.print(a,6);
            Serial.print(",");
            Serial.print(b,6);
            Serial.print(",");
            Serial.print(data);
            Serial.print(",");
          
    }
  }
 } 
}
}
String parseGprmcLat(String s)
{
  int pLoc = 0; //paramater location pointer
  int lEndLoc = 0; //lat parameter end location
  int dEndLoc = 0; //direction parameter end location
  String lat;
  /*make sure that we are parsing the GPRMC string.
    Found that setting s.substring(0,5) == "GPRMC" caused a FALSE.
    There seemed to be a 0x0D and 0x00 character at the end. */
  if (s.substring(0, 4) == "GPRM")
  {
    //Serial.println(s);
    for (int i = 0; i < 2; i++)
    {
      if (i < 1)
      {
        pLoc = s.indexOf(',', pLoc + 1);
        /*Serial.print("i < 3, pLoc: ");
          Serial.print(pLoc);
          Serial.print(", ");
          Serial.println(i);*/
      }
      if( i == 1 )
      {
        lEndLoc = s.indexOf(',', pLoc + 1);
        lat = s.substring(pLoc + 1, lEndLoc);
        /*Serial.print("i = 4, lEndLoc: ");
          Serial.println(lEndLoc);
          Serial.print("dEndLoc: ");
          Serial.println(dEndLoc);*/
      }
    }
    return lat;
  }
  //}
  //}
}

//Parse GPRMC NMEA sentence data from String
//String must be GPRMC or no data will be parsed
//Return Longitude
String parseGprmcLon(String s)
{
  int pLoc = 0; //paramater location pointer
  int lEndLoc = 0; //lat parameter end location
  int dEndLoc = 0; //direction parameter end location
  String lon;

  /*make sure that we are parsing the GPRMC string.
    Found that setting s.substring(0,5) == "GPRMC" caused a FALSE.
    There seemed to be a 0x0D and 0x00 character at the end. */
  if (s.substring(0, 4) == "GPRM")
  {
    //Serial.println(s);
    for (int i = 0; i < 3; i++)
    {
      if (i < 2)
      {
        pLoc = s.indexOf(',', pLoc + 1);
        /*Serial.print("i < 3, pLoc: ");
          Serial.print(pLoc);
          Serial.print(", ");
          Serial.println(i);*/
      }
      if (i == 2)
      {
        lEndLoc = s.indexOf(',', pLoc + 1);
        lon = s.substring(pLoc + 1, lEndLoc);
        /*Serial.print("i = 3, pLoc: ");
          Serial.println(pLoc);
          Serial.print("lEndLoc: ");
          Serial.println(lEndLoc);*/
      }
      else
      {
        dEndLoc = s.indexOf(',', lEndLoc + 1);
        lon = lon + " " + s.substring(lEndLoc + 1, dEndLoc);
        /*Serial.print("i = 4, lEndLoc: ");
          Serial.println(lEndLoc);
          Serial.print("dEndLoc: ");
          Serial.println(dEndLoc);*/
      }
    }
    return lon;
  }
}

String parseGprmcDeep(String s)
{
  int pLoc = 0; //paramater location pointer
  int lEndLoc = 0; //lat parameter end location
  int dEndLoc = 0; //direction parameter end location
  String Deep;

  /*make sure that we are parsing the GPRMC string.
    Found that setting s.substring(0,5) == "GPRMC" caused a FALSE.
    There seemed to be a 0x0D and 0x00 character at the end. */
  if (s.substring(0, 4) == "GPRM")
  {
    //Serial.println(s);
    for (int i = 0; i < 4; i++)
    {
      if (i < 3)
      {
        pLoc = s.indexOf(',', pLoc + 1);
        /*Serial.print("i < 8, pLoc: ");
          Serial.print(pLoc);
          Serial.print(", ");
          Serial.println(i);*/
      }
      if (i == 3)
      {
        lEndLoc = s.indexOf(',', pLoc + 1);
        Deep=s.substring(pLoc + 1, lEndLoc);
      } 
      else
      {
        lEndLoc = s.indexOf(',', pLoc + 1);
        Deep = s.substring(pLoc + 1, lEndLoc);
        /*Serial.print("i = 8, pLoc: ");
          Serial.println(pLoc);
          Serial.print("lEndLoc: ");
          Serial.println(lEndLoc);*/
      }
    }
    return Deep;
  }
}

String charToString(char *c)
{

  String val = "";

  for (int i = 0; i <= sizeof(c); i++)
  {
    val = val + c[i];
  }

  return val;
}
