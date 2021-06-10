/*Air Quality Monitering project was built to monitor and sample atmospheric 
parameters such as Carbon monoxide concentration, Temperature and Humidity in targeted spots in a city. 
The collected data will be later sampled to analyse the results with the help of cloud ThingSpeak 
to determine the factors that affects these key parameters */
 

//Libraries
#include <ESP8266WiFi.h> // NodeMCU library
#include "dht.h" //Temperature and humidity sensor Library
#include "MQ7.h" //Calibrations Library for Gas sensor
//....Libraries end

//DHT setup
#define dht_apin D3 // Digital Pin sensor is connected to D3
dht DHT;
//...Setup end

//Wifi setup and Thingspeak Credentials

char* ssid="Your SSID"; //Enter your SSID
char* pass="Your PWD"; //Enter your password
char* host="api.thingspeak.com";
char* writeapikey="IIHQG7GURQAKBDDM";
//...end

//pin setup
const int mq07=A0; 
//...end

//Mq7 setup
  MQ7 mq7(A0, 3.3); //As for nodeMCU vcc=3.3v
//end.....

void setup() {
  Serial.begin(115200);
  WiFiClient client;

  //pinmodes
  pinMode(mq07,INPUT);
  //...end

  //WiFi connection
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED) //checking whether Wifi is connected or not
  {
    delay(200); //if not connected waits in the loop
    Serial.print("..");
  }
  Serial.println();
  Serial.println("NodeMCU is connected!"); //Once connected it is notified here through serial moniter
  Serial.println(WiFi.localIP());
  //-------------------------------------
  
}

  // variables required for project
  float Temperature,Humidity;
  float coquantity;
  const int httpport = 80;
  //---------------------------------------

   
void loop() {
  delay(2000);
  //temperature and humidity from DHT11 
   DHT.read11(dht_apin);
    Temperature=DHT.temperature;
    Humidity=DHT.humidity;
    Serial.print("Current humidity = ");
    Serial.print(Humidity);
    Serial.print("%  ");//percentage unit
    Serial.print("temperature = ");
    Serial.print(Temperature); 
    Serial.println("C  "); //celsius units
  //.....end
  
  //CO from mq07
    coquantity = mq7.getPPM(); //reading CarbonMonoxide in PPM
    Serial.print("COquantity : ");
    Serial.print(coquantity);
    Serial.println("ppm"); //units printing
    Serial.println("----------------------------------------------------");
  //....end
  // client and pushing data to server (cloud)
    WiFiClient client;
    if(!client.connect(host,httpport))
    { 
      Serial.println("No Client");
      return;
    }
    
    String url="/update?key=";
    url+=writeapikey;
    // uploding temperature data
      url+="&field1=";
      url+=String(Humidity);
    //---------------------------
    // uploading humidity data
       url+="&field2=";
       url+=String(Temperature);
    //--------------------------
    // uploading light intensity data
       url+="&field3=";
       url+=String(coquantity);
    //--------------------------
    // uploading the url to cloud and disconnecting the client
     client.print(String("GET ") + url + "HTTP/1.1\r\n" + "Host : " + host + "\r\n" + "Connection: close\r\n\r\n");
     client.stop(); 
    //--------------------------
    // Waiting for 60 seconds
      delay(60000);
}
