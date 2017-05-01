
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define updateRate 500 // The minimal time between two requests in ms
#define PORT 80
#define BASE_URL "requestb.in" //WITHOUT HTTP://
#define BIN_ID "/1edquiu1"

//WiFi informationer
const char* ssid     = "AU-Gadget";
const char* password = "augadget";

HTTPClient http;



void setup() {

  Serial.begin(9600);

  WiFi.persistent(false); //  Do this just to be on the safe side!!!!

  delay(10);
  wifiConnect();

  delay (1000);

  String response = post("Graunephar er sej!", BIN_ID);
  Serial.print("RESPONSE: ");
  Serial.println(response);

  response = get(BIN_ID);
  Serial.print("RESPONSE: ");
  Serial.println(response);


}

void loop() {
  wifiCheck(); //Maintain wifi connection
  yield(); //Let the ESPcore handle background tasks


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


    return response;
}


String get(String url) {

    String response = "Request not allowed because of timelimit";

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


      return response;
}
