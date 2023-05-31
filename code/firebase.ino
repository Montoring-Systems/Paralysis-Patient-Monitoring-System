#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// // Insert your network credentials
// #define WIFI_SSID "Redmi Note 9 Pro Max"
// #define WIFI_PASSWORD "12345678"

#define WIFI_SSID "Amal"
#define WIFI_PASSWORD "amalamal"

// Insert Firebase project API Key
#define API_KEY "AIzaSyAk-4VDBfv8mNWFOFEym-3KO_bozYR3Vk0"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://paralysis-monitoring-system-default-rtdb.firebaseio.com"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

void db_init() {
  // Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback;  //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void send_msg(int down, int left, int right, int up) {
  bool data_sent = false;
  while (!data_sent) {
    if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
      sendDataPrevMillis = millis();
      // Writing the value of smoke
      if (Firebase.RTDB.setInt(&fbdo, "node/down", down)) {
        Serial.println("PASSED");
        // Serial.println("PATH: " + fbdo.dataPath());
        // Serial.println("TYPE: " + fbdo.dataType());
        data_sent = true;
      } else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
        data_sent = false;
      }

      // Writing the value of flame
      if (Firebase.RTDB.setInt(&fbdo, "node/left", left)) {
        Serial.println("PASSED");
        // Serial.println("PATH: " + fbdo.dataPath());
        // Serial.println("TYPE: " + fbdo.dataType());
        data_sent = true;
      } else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
        data_sent = false;
      }

      // Writing the value of count
      if (Firebase.RTDB.setInt(&fbdo, "node/right", right)) {
        Serial.println("PASSED");
        // Serial.println("PATH: " + fbdo.dataPath());
        // Serial.println("TYPE: " + fbdo.dataType());
        data_sent = true;
      } else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
        data_sent = false;
      }

      // Writing the value of temperature
      if (Firebase.RTDB.setInt(&fbdo, "node/up", up)) {
        Serial.println("PASSED");
        // Serial.println("PATH: " + fbdo.dataPath());
        // Serial.println("TYPE: " + fbdo.dataType());
        data_sent = true;
      } else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
        data_sent = false;
      }

      // Writing the value of humidity
      /*if (Firebase.RTDB.setInt(&fbdo, "node_1/humidity", humid)) {
        Serial.println("PASSED");
        // Serial.println("PATH: " + fbdo.dataPath());
        // Serial.println("TYPE: " + fbdo.dataType());
        data_sent = true;
      } else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
        data_sent = false;
      }*/
    }
  }
}