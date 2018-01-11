#include <ESP8266WiFi.h>
#include <WiFiClient.h>

/* VARIABLES*/
//const char http_site[] = "http://azerfvgt.comxa.com";
const char http_site[] = "195.221.246.8";
//"url" is formated for the corresponding PHP file on server
String url = "http://195.221.246.8/ESP8266_INSERT_BDD.php?temperature=";
String WifiName = "BORNE-ML"
String WifiPassword = "ednzelzdzd"
const int http_port = 80;

long timestamp;
bool connected = false;

WiFiClient client;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);  
  pinMode(A0, INPUT);
  Serial.begin(9600);
  Serial.println("Serial started.");
  Serial.println("WiFi init.");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WifiName,WifiPassword);
  
  Serial.print("Starting WiFi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Done.");

  // Attempt to connect to website
  if (!getPage())
  {
    Serial.println("GET request failed.");
  }
  else
  {
    connected = true;
  }
  timestamp = millis();
}

void loop()
{
  while(client.available())
  {
    char c = client.read();
    Serial.print(c);
    delay(0);
  }
 

  // If the server has disconnected, stop the client and WiFi
  if(!client.connected() && connected)
  {
    Serial.println();
    client.stop();
    Serial.println("Finished GET request.");
    connected = false;
  }

  if((millis() > (timestamp + 500)) && !connected)
  {
    if (!getPage())
    {
      Serial.println("GET request failed.");
    }
    else
    {
      connected = true;
    }
    timestamp = millis();
  }
}

// Perform an HTTP GET request to a remote page
bool getPage()
{
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.print("demande du serveur...");
  if (!client.connect(http_site, http_port))
  {
    return false;
  }
Serial.println("ok");
  // Mesure de la Temperarture
  int sensorValue = analogRead(A0);
   // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  int temperature = sensorValue * (5.0 / 1023.0)*1000;
  int humidity=0;
  // print out the value you read:
  Serial.print("------------------------");
  Serial.println(temperature);
  

// This will send the request to the server
    client.print(String("GET ") + url + temperature + "&humidity=103/" + " HTTP/1.0\r\n" +
 "Host: " + http_site 
 + "\r\n" + "Connection: close\r\n\r\n");

 delay(10);
 
  return true;
}
