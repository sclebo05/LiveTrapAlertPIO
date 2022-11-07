#include "twilio.hpp"
#include <config.h>

//borrowed initial reed switch code from https://lastminuteengineers.com/reed-switch-arduino-tutorial/
//also borrowed from https://esp32io.com/tutorials/esp32-door-sensor as well

const int REED_PIN = 19;	// Pin connected to reed switch
bool messagesent = false; // limit sending of texts

Twilio *twilio;
WiFiUDP ntpUDP;

void sendit() {
  Serial.println("Sending MMS");
  twilio = new Twilio(account_sid, auth_token);
  delay(1000);
  String response;
  bool success = twilio->send_message(to_number, from_number, message, response);
  if (success) {
    Serial.println("Sent message successfully!");
    messagesent = true;
  } else {
    Serial.println(response);
  }
}

void setup() {
	Serial.begin(9600);
  Serial.println("*** System Starting ***");
  //   Serial.begin(115200);
	pinMode(REED_PIN, INPUT_PULLUP);	// Enable internal pull-up for the reed switch
	pinMode(LED_BUILTIN, OUTPUT);
  Serial.print("Connecting to WiFi network ");  // Connect to wifi
  Serial.print(ssid);
  Serial.println("'...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting...");
    delay(500);
  }
  Serial.print("Connected with IP ");  // show IP and boot message
  Serial.println(WiFi.localIP());
  setCpuFrequencyMhz(80);
  Serial.println("*** Startup Completed ***");
}

void loop() {
	int proximity = digitalRead(REED_PIN); // Read the state of the switch
	if (proximity == LOW) { // If the pin reads low, the switch is closed.
  	Serial.println("***Switch closed***");
		digitalWrite(LED_BUILTIN, HIGH);	// Turn the LED on
    if ( messagesent == false ) {
      sendit();
    }
	}
	else { //switch open
		//Serial.println("Switch opened");
		digitalWrite(LED_BUILTIN, LOW);		// Turn the LED off
	}
  delay(1000);
}

