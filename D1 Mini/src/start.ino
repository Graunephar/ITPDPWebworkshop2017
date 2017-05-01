
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define updateRate 500 // The minimal time between two requests in ms
#define PORT 80
#define BASE_URL "webtekws.herokuapp.com"


const int red = D3;
const int green = D2;
const int blue = D1;

const int lightPin = A0;

double updateTimestamp = 0; // used for the update rate
double readingTimestamp   = 0; // used for the update rate

//WiFi informationer
const char* ssid     = "AU-Gadget";
const char* password = "augadget";

HTTPClient http;



void setup() {
  Serial.begin(9600);

  WiFi.persistent(false); //  Do this just to be on the safe side!!!!

  delay(10);
  wifiConnect();

  //Pin stuff
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);


}

void loop() {
  wifiCheck(); //Maintain wifi connection
  yield(); //Let the ESPcore handle background tasks

  readLightAndPost();
  getAndUpdateRGBValueFromServer();

delay(5000);
}

void getAndUpdateRGBValueFromServer() {

    String command = get("/api/getrgb"); //Gets response from server


    int Rcolorvalue = getValue(command, ',', 0).toInt();
    int Gcolorvalue = getValue(command, ',', 1).toInt();
    int Bcolorvalue = getValue(command, ',', 2).toInt();

    writeRGBColor(Rcolorvalue, Gcolorvalue, Bcolorvalue);

    printAllValues(command);

}

/**
 * Writes a RGB color to the RGB diode
 * @Param R: the red value of the color
 * @Param G: the green value of the color
 * @Param B: the blue value of the color
 */
void writeRGBColor(int R, int G, int B) {

   analogWrite(red, R);
   analogWrite(green, G);
   analogWrite(blue, B);


}


void readLightAndPost() {
  int value = analogRead(lightPin);
  String poststring = String(value);

  post(poststring, "/api/d1mini");

}




void printAllValues(String command) {


   int index = 0; //the value that is currently being acceses in the loop
   String param; //The value for the current variable that is being acceses in the loop

   //Loops through all the arguments in the command string putting them in the param variable as it goes along
   while((param = getValue(command, ',', index)) != "") {

       Serial.println("Param" + String(index) + " : " + param); //print the variable

       index++;

   }

}

/**
 * This function  can retrive an argument from a string with arguments deperatet by a particluar character
 * @Param data: a string containing the data seprated by a particular character
 * @Param seperator: the character that seperates the values e.g. ','
 * @Param index: the number of variable from the list that we want to retrieve
 * @Return a string with the value, NULL if out of range
 */
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}




void wifiConnect() {
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void wifiCheck()
{
  if (WiFi.status() != WL_CONNECTED) //if wifi is connected: do nothing.
  {
    int tickCount = 0;
    Serial.println("Wifi dropped. Retry in 60 seconds.");
    delay(60000); //wait 60 seconds
    Serial.println("Connecting");
    WiFi.begin(ssid, password); //reconnect

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println(".");
      tickCount++;
      if (tickCount > 100) //after awaiting reconnection for 50 seconds
      {
        Serial.println("Wifi fail...");
        //This is where you end up if the connection was lost and unrecoverable
        while (1); //Endless loop...
      }
    }

    //This is the place to do something in case the connection was lost but fixed.

  }
}



String post(String payload, String url) {

    String response = "Request not allowed because of timelimit";

    if( millis() - updateTimestamp > updateRate) { // Safeguards against server timeouts
      updateTimestamp = millis();

      Serial.println("[HTTP] POST begin...");
      // configure traged server and url
      http.begin(BASE_URL, PORT, url); //HTTP

      Serial.println("payload: " + payload);
      Serial.println("url: " + url);

      String status = String(http.POST(payload));
      response = http.getString();

      Serial.println("Status: " + status);
      Serial.println("Response: " + response);

      http.end();
    }

    return response;
}


String get(String url) {

    String response = "Request not allowed because of timelimit";

    if( millis() - updateTimestamp > updateRate) { // Safeguards against server timeouts

        Serial.println("[HTTP] GET begin...");
        http.begin(BASE_URL, PORT, url); //HTTP

        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.println("[HTTP] GET... code: " + String(httpCode));

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                response = http.getString();
                Serial.println("Response " + response);
            }
        } else {
            Serial.println("[HTTP] GET... failed, error: " + String(httpCode));
            response = "ERROR";
        }

        http.end();
      }

      return response;
}
