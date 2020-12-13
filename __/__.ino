#include <SoftwareSerial.h>    //需要调用这个库实现串口通讯
#include "ESP8266.h"

#define SSID "daolun123"    //填写2.4GHz的WIFI名称，不要使用校园网
#define PASSWORD "123456654321"//填写自己的WIFI密码
#define HOST_NAME "api.heclouds.com"  //API主机名称，连接到OneNET平台，无需修改
#define DEVICE_ID "642906909"       //填写自己的OneNet设备ID
#define HOST_PORT (80)                //API端口，连接到OneNET平台，无需修改
String APIKey = "KQnulKTjxC4ZDTpkaM=HYtgur=0="; //与设备绑定的APIKey

#define INTERVAL_SENSOR 5000 //定义传感器采样及发送时间间隔
int b=40;
SoftwareSerial mySerial(5, 6);ESP8266 wifi(mySerial);
SoftwareSerial softSerial(3,2); //新建一个softSerial对象，rx:3,tx:2
int a;                         //接收串口的值
long begin;//开始时的cpu当前时间,单位毫秒
long end;//结束时的cpu当前时间,单位毫秒
long total;//总计时间,单位分钟

void setup() {mySerial.begin(115200); 
  Serial.begin(9600);     
  Serial.print("setup begin\r\n");

  
  Serial.print("FW Version: ");
  Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStation()) {
    Serial.print("to station ok\r\n");
  } else {
    Serial.print("to station err\r\n");
  }

 
  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.print("Join AP success\r\n");
    Serial.print("IP: ");
    Serial.println(wifi.getLocalIP().c_str());
  } else {
    Serial.print("Join AP failure\r\n");
  }

unsigned long net_time1 = millis();



  


//初始化ESPN8266结束
  pinMode(13,OUTPUT);        //设置IO口13为输出模式
  
  softSerial.begin(9600);   //初始化ld3320软串口通信；
  digitalWrite(13,LOW);     //给13口低电平
  delay(3000);}


void loop() {
  a=softSerial.read();      //读取串行端口的值
  switch(a)
  {
    case 1:
       {digitalWrite(13,HIGH);begin=millis();}//当喊“开始”的时候，可以看到的Arduino上的led亮起，并开始计时
       break;
    case 2:{
      digitalWrite(13,LOW);end=millis();total=(end-begin)/60000;
      //一下为数据上传部分代码
      if (wifi.createTCP(HOST_NAME, HOST_PORT)) { 
      Serial.print("create tcp ok\r\n");
      String jsonToSend = "{\"阅读时长\":";
      jsonToSend += "\"" + String(b) + "\"";
      jsonToSend += "}";
       String postString = "POST /devices/";
      postString += DEVICE_ID;
      postString += "/datapoints?type=3 HTTP/1.1";
      postString += "\r\n";
      postString += "api-key:";
      postString += APIKey;
      postString += "\r\n";
      postString += "Host:api.heclouds.com\r\n";
      postString += "Connection:close\r\n";
      postString += "Content-Length:";
      postString += jsonToSend.length();
      postString += "\r\n";
      postString += "\r\n";
      postString += jsonToSend;
      postString += "\r\n";
      postString += "\r\n";
      postString += "\r\n";

      const char *postArray = postString.c_str();
      Serial.println(postArray);
      wifi.send((const uint8_t *)postArray, strlen(postArray)); 
      Serial.println("send success");
      if (wifi.releaseTCP()) { 
        Serial.print("release tcp ok\r\n");
      } else {
        Serial.print("release tcp err\r\n");
      }
      postArray = NULL; 
    } else {
      Serial.print("create tcp err\r\n");
    }

    }
    
   


         
    default:
      break; 
  }
}
