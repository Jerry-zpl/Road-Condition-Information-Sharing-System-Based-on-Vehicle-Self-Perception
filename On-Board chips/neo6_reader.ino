#define GPRMC_TERM "$GPRMC,"		//定义要解析的指令，因为这条指令包含定位和时间信息
#include <stdio.h>

char nmeaSentence[68];
String latitude;		//纬度
String longitude;		//经度
String lndSpeed;		//速度
String Heading;
String gpsTime;			//UTC时间，本初子午线经度0度的时间，和北京时间差8小时
String beiJingTime;		//北京时间
double a;
double b;
double d;
double A[2];
double B[2];
int N = 0;
int M = 0;
double X;
double y;

void setup()	//初始化内容
{
  Serial.begin(9600);			//定义波特率9600
}

void loop()		//主循环
{

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)	//一秒钟内不停扫描GPS信息
  {
    while (Serial.available())	//串口获取到数据开始解析
    {
      char c = Serial.read();	//读取一个字节获取的数据

      switch (c)					//判断该字节的值
      {
        case '$':					//若是$，则说明是一帧数据的开始
          Serial.readBytesUntil('*', nmeaSentence, 67);		//读取接下来的数据，存放在nmeaSentence字符数组中，最大存放67个字节
          //Serial.println(nmeaSentence);
          latitude = parseGprmcLat(nmeaSentence);	//获取纬度值
          longitude = parseGprmcLon(nmeaSentence); //获取经度值
          lndSpeed = parseGprmcSpeed(nmeaSentence);//获取速度值
           Heading = parseGprmcHeading(nmeaSentence);
          gpsTime = parseGprmcTime(nmeaSentence);//获取GPS时间
         

                    if (latitude > "")		//当不是空时候打印输出
          {
            a = latitude.toDouble();
            A[N] = a;
            N = !N;
            Serial.println("------------------------------------");
            Serial.println(a, 6);
            Serial.print("A0");
            Serial.println(A[0], 6);
            Serial.print("A1");
            Serial.println(A[1], 6);
          }

          if (longitude > "")		//当不是空时候打印输出
          {
            b = longitude.toDouble();
            B[M] = b;
            M = !M;
            Serial.println(b, 6);
            Serial.print("B0");
            Serial.println(B[0], 6);
            Serial.print("B1");
            Serial.println(B[1], 6);
            X = (A[0] - A[1]) * (A[0] - A[1]) + (B[0] - B[1]) * (B[0] - B[1]);
            y =  sqrt( X);
            Serial.println(y, 6);
          }

          if (lndSpeed > "")		//当不是空时候打印输出
          {
            Serial.println( lndSpeed);
          }
          if (Heading > "")    //当不是空时候打印输出
          {
            d = Heading.toDouble();
            Serial.println(d);
          }

          if (gpsTime > "")		//当不是空时候打印输出
          {
            Serial.println("gpsTime: " + gpsTime);
            beiJingTime = getBeiJingTime(gpsTime);	//获取北京时间
            Serial.println("beiJingTime: " + beiJingTime);
          }
      }
    }
  }
}

String getBeiJingTime(String s)
{
  int hour = s.substring(0, 2).toInt();
  int minute = s.substring(2, 4).toInt();
  int second = s.substring(4, 6).toInt();

  hour += 8;

  if (hour > 24)
    hour -= 24;
  s = String(hour) + String(minute) + String(second);
  return s;
}

//Parse GPRMC NMEA sentence data from String
//String must be GPRMC or no data will be parsed
//Return Latitude
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
    for (int i = 0; i < 4; i++)
    {
      if (i < 3)
      {
        pLoc = s.indexOf(',', pLoc + 1);
        /*Serial.print("i < 3, pLoc: ");
          Serial.print(pLoc);
          Serial.print(", ");
          Serial.println(i);*/
      }
      if (i == 3)
      {
        lEndLoc = s.indexOf(',', pLoc + 1);
        lat = s.substring(pLoc + 1, lEndLoc);
        /*Serial.print("i = 3, pLoc: ");
          Serial.println(pLoc);
          Serial.print("lEndLoc: ");
          Serial.println(lEndLoc);*/
      }
      else
      {
        dEndLoc = s.indexOf(',', lEndLoc + 1);
        lat = lat + " " + s.substring(lEndLoc + 1, dEndLoc);
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
    for (int i = 0; i < 6; i++)
    {
      if (i < 5)
      {
        pLoc = s.indexOf(',', pLoc + 1);
        /*Serial.print("i < 3, pLoc: ");
          Serial.print(pLoc);
          Serial.print(", ");
          Serial.println(i);*/
      }
      if (i == 5)
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

//Parse GPRMC NMEA sentence data from String
//String must be GPRMC or no data will be parsed
//Return Longitude
String parseGprmcSpeed(String s)
{
  int pLoc = 0; //paramater location pointer
  int lEndLoc = 0; //lat parameter end location
  int dEndLoc = 0; //direction parameter end location
  String lndSpeed;

  /*make sure that we are parsing the GPRMC string.
    Found that setting s.substring(0,5) == "GPRMC" caused a FALSE.
    There seemed to be a 0x0D and 0x00 character at the end. */
  if (s.substring(0, 4) == "GPRM")
  {
    //Serial.println(s);
    for (int i = 0; i < 8; i++)
    {
      if (i < 7)
      {
        pLoc = s.indexOf(',', pLoc + 1);
        /*Serial.print("i < 8, pLoc: ");
          Serial.print(pLoc);
          Serial.print(", ");
          Serial.println(i);*/
      }
      if (i == 7)
      {
        lEndLoc = s.indexOf(',', pLoc + 1);
        lndSpeed = s.substring(pLoc + 1, lEndLoc);
      }
      else
      {
        lEndLoc = s.indexOf(',', pLoc + 1);
        lndSpeed = s.substring(pLoc + 1, lEndLoc);
        /*Serial.print("i = 8, pLoc: ");
          Serial.println(pLoc);
          Serial.print("lEndLoc: ");
          Serial.println(lEndLoc);*/
      }
    }
    return lndSpeed;
  }
}
String parseGprmcHeading(String s)
{
  int pLoc = 0; //paramater location pointer
  int lEndLoc = 0; //lat parameter end location
  int dEndLoc = 0; //direction parameter end location
  String Heading;

  /*make sure that we are parsing the GPRMC string.
    Found that setting s.substring(0,5) == "GPRMC" caused a FALSE.
    There seemed to be a 0x0D and 0x00 character at the end. */
  if (s.substring(0, 4) == "GPRM")
  {
    //Serial.println(s);
    for (int i = 0; i < 9; i++)
    {
      if (i < 8)
      {
        pLoc = s.indexOf(',', pLoc + 1);
        /*Serial.print("i < 8, pLoc: ");
          Serial.print(pLoc);
          Serial.print(", ");
          Serial.println(i);*/

      }
      if (i == 8)
      {
        lEndLoc = s.indexOf(',', pLoc + 1);
        Heading = s.substring(pLoc + 1, lEndLoc);
      }
      else
      {
        lEndLoc = s.indexOf(',', pLoc + 1);
        Heading = s.substring(pLoc + 1, lEndLoc);
        /*Serial.print("i = 8, pLoc: ");
          Serial.println(pLoc);
          Serial.print("lEndLoc: ");
          Serial.println(lEndLoc);*/

      }
    }
    return Heading;
  }
}

//Parse GPRMC NMEA sentence data from String
//String must be GPRMC or no data will be parsed
//Return Longitude
String parseGprmcTime(String s)
{
  int pLoc = 0; //paramater location pointer
  int lEndLoc = 0; //lat parameter end location
  int dEndLoc = 0; //direction parameter end location
  String gpsTime;

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
        /*Serial.print("i < 8, pLoc: ");
          Serial.print(pLoc);
          Serial.print(", ");
          Serial.println(i);*/
      }
      else
      {
        lEndLoc = s.indexOf(',', pLoc + 1);
        gpsTime = s.substring(pLoc + 1, lEndLoc);
        /*Serial.print("i = 8, pLoc: ");
          Serial.println(pLoc);
          Serial.print("lEndLoc: ");
          Serial.println(lEndLoc);*/
      }
    }
    return gpsTime;
  }
}

// Turn char[] array into String object
String charToString(char *c)
{

  String val = "";

  for (int i = 0; i <= sizeof(c); i++)
  {
    val = val + c[i];
  }

  return val;
}
