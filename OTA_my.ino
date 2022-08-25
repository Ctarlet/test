
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef STASSID
#define STASSID "😼Azazel's hommiez"
#define STAPSK  "9639252543"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

String start;

ESP8266WebServer server(80);

void handleRoot() {

  if (digitalRead(BUILTIN_LED)) // при обновлении страницы проверяем состояние пина
  {
    start = "0";
  }
  else
  {
    start = "1";
  }
  String out = R"=====(

<!DOCTYPE html>
<html lang="ru">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width,initial-scale=1">

    <title>IoT "Azzazel hommies"</title>
    <meta name="description" content="">
</head>

<body>
    <style>
        * {margin: 0; padding: 0;}
        body {
            text-align: center;
            color: WhiteSmoke;
            font-size: 16px;
            height: 100%;
            background: black;
            padding: 30px;
            align-content: center;
        }
        p {
            margin: 10px;
            padding: 10px;
        }
        hr {
            border: none;
        }
        .state {
            display: inline-flex;
            align-content: center;
            border-radius: 50%;
            border: 2px outset whitesmoke;
            box-shadow: 4px 4px 4px;
            width: 10px;
            height: 10px;
        }
        .indicatorText {
            display: inline-flex;
            border: 2px solid black;
        }
        button {
            color: WhiteSmoke;
            border-color: WhiteSmoke;
            background: black;
            margin: 10px;
            padding-top: 10px;
            padding-bottom: 10px;
            padding-left: 15px;
            padding-right: 15px;
            width: 100px;
            height: 50px;
            text-shadow: 2px 2px 2px;
        }
        button:active {
            color: dimgrey;
            text-shadow: none;
        }
    </style>

    <h2>LED Control</h2>
    <h3>ESP8266</h3>

    <p>•- −−•• •− −−•• • •−•• −••− </p>

    <p class="state" id="state">&nbsp;</p>
    <p class="indicatorText">LED TEST</p>
    <hr>
    <button id="led_button_on" onclick="send(1)">ON</button>
    <button id="led_button_off" onclick="send(0)">OFF</button>

    <p>•− −−•• •− −−•• • •−•• −••− </p>

    <p id="current_date_time_block"></p>

    <script type="text/javascript">
        function zero_first_format(value) {
            if (value < 10) {
                value = '0' + value;
            }
            return value;
        }
        function date_time() {
            var current_datetime = new Date();
            var day = zero_first_format(current_datetime.getDate());
            var month = zero_first_format(current_datetime.getMonth() + 1);
            var year = current_datetime.getFullYear();
            return day + "." + month + "." + year;
        }
        document.getElementById('current_date_time_block').innerHTML = date_time();

        function loadDoc()
{
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      //document.getElementById("state").innerHTML = this.responseText;
      if (this.responseText == "ON"){
        document.getElementById("state").style.backgroundColor = "blue";
        }
      else {document.getElementById("state").style.backgroundColor = "black";}
      
    }
  };
  xhttp.open("GET", "led_set?state="+)=====";

  out += start; // переменная состояния пина

  out += R"=====(, true);
  xhttp.send();
}

function send(led_sts) 
{
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      //document.getElementById("state").innerHTML = this.responseText;
            if (this.responseText == "ON"){
        document.getElementById("state").style.backgroundColor = "blue";
        }
      else {document.getElementById("state").style.backgroundColor = "black";}
    }
  };
  xhttp.open("GET", "led_set?state="+led_sts, true);
  xhttp.send();
}


loadDoc();

    </script>
</body>
</html>

)=====";


   server.send ( 200, "text/html", out );
}

void led_control() 
{

 String state;
 String act_state = server.arg("state");

 if(act_state == "1")
 {
  digitalWrite(LED_BUILTIN,LOW); //LED ON
  state = "ON";
 }

 else
 {
  digitalWrite(LED_BUILTIN,HIGH); //LED OFF
  state = "OFF";
 }
 server.send(200, "text/plane", state);
}


void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
   ArduinoOTA.setHostname("TestESP8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  
    WiFi.softAP(ssid, password);
     pinMode(LED_BUILTIN, OUTPUT);
     digitalWrite(LED_BUILTIN,HIGH); //LED OFF

  server.on("/", handleRoot);
  server.on("/led_set", led_control);
  server.begin();
}

void loop() {
  ArduinoOTA.handle();
  server.handleClient();
}
