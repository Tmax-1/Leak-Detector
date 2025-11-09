// THIS WAS CONVERTED FROM MY CODE BY CHAT GPT -- MAY NOT WORK PROPERLY

#include "HomeSpan.h"
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

const char* ap_ssid = "ESP32_AP";
const char* ap_password = "12345678";

WebServer server(8080);  

int ledPin = 2; 
int powerPin = 25;
int readingPin = 34;
int val = 0;

bool ledState = false;

// Function to get water level
int Get_Water_level() {
  digitalWrite(powerPin, HIGH);
  delay(10);
  val = analogRead(readingPin);
  digitalWrite(powerPin, LOW);
  return val;
}

// HomeKit Leak Sensor
struct DEV_LeakSensor : Service::LeakSensor {
  SpanCharacteristic *leakDetected;
  
  DEV_LeakSensor() : Service::LeakSensor() {
    leakDetected = new Characteristic::LeakDetected(0);
  }
  
  void loop() {
    int leakLevel = Get_Water_level();
    
    if (leakLevel >= 250) {
      if (leakDetected->getVal() == 0) {
        leakDetected->setVal(1);
        Serial.println("⚠️ ALERT: LEAK DETECTED!");
      }
    } else {
      if (leakDetected->getVal() == 1) {
        leakDetected->setVal(0);
        Serial.println("✅ Leak level normal");
      }
    }
  }
};

// Web page HTML
const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Leak Level Monitor</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      text-align: center;
      font-family: Arial, sans-serif;
      color: white;
      transition: background-color 0.5s;
    }
    h1 {
      font-size: 3em;
      margin-top: 40px;
    }
    h2 {
      font-size: 1.5em;
    }
    #waterValue {
      font-weight: bold;
      font-size: 2em;
    }
    #status {
      font-size: 1.8em;
      margin-top: 20px;
      font-weight: bold;
    }
  </style>
</head>
<body>
  <h1>LEAK LEVEL MONITOR</h1>
  <h2>Current level: <span id="waterValue">--</span></h2>
  <div id="status">Status: --</div>

<script>
function updateWater() {
  fetch('/water').then(response => response.text()).then(value => {
    document.getElementById('waterValue').innerText = value;

    let val = parseInt(value);
    let statusText = '';
    
    if (val < 50) {
      document.body.style.backgroundColor = 'green';
      document.body.style.color = 'white';
      statusText = ' Normal';
    } else if (val < 250) {
      document.body.style.backgroundColor = 'yellow';
      document.body.style.color = 'black';
      statusText = ' Elevated';
    } else {
      document.body.style.backgroundColor = 'red';
      document.body.style.color = 'white';
      statusText = ' LEAK DETECTED!';
    }
    
    document.getElementById('status').innerText = 'Status: ' + statusText;
  });
}

window.onload = function() {
  updateWater();
  setInterval(updateWater, 1000);
};
</script>
</body>
</html>
)rawliteral";

// Web handlers
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleWater() {
  int waterValue = Get_Water_level();
  server.send(200, "text/plain", String(waterValue));
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin, LOW);

  // HomeKit setup FIRST (uses port 80)
  homeSpan.begin(Category::Bridges, "Leak Monitor");
  homeSpan.setPairingCode("46637726");
  homeSpan.setControlPin(0);
  homeSpan.setWifiCredentials(ssid, password);
  
  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Identify();
      new Characteristic::Name("Leak Sensor");
    new DEV_LeakSensor();
  
  Serial.println("\n📱 HomeKit Pairing Code: 4663-7726");

  // Access Point setup
  WiFi.softAP(ap_ssid, ap_password);
  Serial.println("✅ Access Point started");
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  // Start web server on port 8080
  server.on("/", handleRoot);
  server.on("/water", handleWater);
  server.begin();

  Serial.println("✅ HTTP server started on port 8080");
  Serial.println("\n========================================");
  Serial.println("Access web interface at:");
  Serial.println("  http://192.168.1.173:8080");
  Serial.println("  http://192.168.4.1:8080");
  Serial.println("========================================\n");
}

void loop() {
  homeSpan.poll();
  server.handleClient();
}
