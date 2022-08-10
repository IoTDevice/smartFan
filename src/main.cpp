#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define AP_SSID "smartFan"
#define AP_PSW "88888888"

#define PIN_FAN 0

IPAddress local_IP(192, 168, 1, 1); 
// 设置网关
IPAddress gateway(192, 168, 0, 1);
// 设置子网掩码
IPAddress subnet(255, 255, 255, 0); 

/* 2. 创建一个web服务器对象，使用80端口，HTTP网络服务器标准端口号即为80 */
ESP8266WebServer http_server(80);

/* 3. 处理访问网站根目录“/”的访问请求 */
void handleRoot() { 
  http_server.send(200, "text/plain", "Hello from ESP8266");
}

/* 4. 设置处理404情况的函数'handleNotFound' */
void handleNotFound(){
  http_server.send(404, "text/plain", "404: Not found");
}

void handlePWMApi(){
  String valStr = http_server.arg("value");
  int val = atoi(valStr.c_str());
  analogWrite(PIN_FAN,val);//满占空比灯灭
  delay(1);
  http_server.send(200, "application/json", "{\"code\":0,\"message\":\"success\"}");
}

void setup() {
  pinMode(PIN_FAN,OUTPUT); //设置引脚模式
  analogWrite(PIN_FAN,0); //设置初始化占空比0%
  // put your setup code here, to run once:
  WiFi.mode(WIFI_AP);
  // 设置IP地址网关和子网掩码  
  // WiFi.softAPConfig(local_IP, gateway, subnet);  
  // 设置wifi的名字和密码
  WiFi.softAP(AP_SSID);

  http_server.begin();
  http_server.on("/", handleRoot);
  http_server.onNotFound(handleNotFound);  
  http_server.on("/pwm", handlePWMApi);
  Serial.println("HTTP http_server started");//
}

void loop() {
  // put your main code here, to run repeatedly:
  http_server.handleClient();
}