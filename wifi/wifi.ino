
#include <SoftwareSerial.h>
#define DEBUG true

// light
int Pin = 2;
int Val = 0;

//connect
String SID = "SensorNetwork";
//String PWD = "12369874125";
String PWD = "123698745";
String IP = "140.120.54.157";
String file ="wifi_web/receive.php";
boolean upload = false;


SoftwareSerial esp8266(8,9); // RX, TX
// connect 8 to TX of wifi-esp8266
// connect 9 to RX of wifi-esp8266



void setup() {
    // enable debug serial
    Serial.begin(9600); 
    // enable software serial
    esp8266.begin(115200);
    init_wifi();//Reset esp8266,change mode,connect wifi
}
 
void loop() {
  
  // 讀取光敏電阻
  Val = analogRead(A2);

  
  // 傳送command給esp8266
  while(Serial.available())
  {
     char input = Serial.read();
      
     if(input =='o')//uplao
     {
        Serial.println("|---Start Upload---|");
        upload = true;
     }
     else if(input =='x')
     {
        Serial.println("|---Stop Upload---|");
        upload = false;
     }
     else if(input == 'r')
     {
        init_wifi();
     }
     else  
        Serial.print(input);   
  }
 
  // esp8266回覆狀態
  while(esp8266.available())
  {
     Serial.write(esp8266.read());
  }
   
  // 是否上傳
  if(upload)
  { 
     uploadData();
  }
  
}


// reset ESP8266
void init_wifi(){
  Serial.println("|---Reset---|");
  sendCommand("AT+RST\r\n",5000,DEBUG); // reset module
  sendCommand("AT+CWMODE=1\r\n",2000,DEBUG); // configure as access point
  sendCommand("AT+CWJAP=\""+SID+"\",\""+PWD+"\"\r\n",5000,DEBUG);
  sendCommand("AT+CIPMUX=0\r\n",2000,DEBUG); // configure for single connections
  Serial.println("|---Reset Finish---|");
}

// send command
String sendCommand(String command, const int timeout, boolean debug)
{
    String response = "";
    
    esp8266.print(command); // send the read character to the esp8266
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(esp8266.available())
      {    
        // The esp has data so display its output to the serial window 
        char c = esp8266.read(); // read the next character.
        response+=c;
      }  
    }
    
    if(debug)
    {
      Serial.print(response);
    }
    
    return response;
}

// upload data
void uploadData()
{
  // convert to string
  // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP; //host
  cmd += "\",8001";
  esp8266.println(cmd);
   
  if(esp8266.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }
  
  // prepare GET string
  String getStr = "GET /"+file+"?value=";
  getStr += String(Val);
  getStr +=" HTTP/1.1\r\nHost:"+IP;
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  esp8266.println(cmd);

  if(esp8266.find(">")){
    esp8266.print(getStr);
  }
  else{
    esp8266.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }
  delay(1000);  
}
