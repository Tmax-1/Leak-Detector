# **💧 ESP32 Smart Water Leak Detector**
A Wi-Fi-enabled **water leak detection** system built with an ESP32 and water sensor, featuring:  
🌐 A live web dashboard to monitor water levels in real-time  
🍎 Apple HomeKit integration for instant alerts when a leak is detected  

View live water level data through a local web interface hosted by the ESP32 (only works while connected to the same Wi-Fi network).    
HomeKit Integration: Receive instant notifications on your Apple devices when a leak occurs (always works).  
Compact & Low Power: Designed for continuous operation with minimal energy usage.   

The water sensor continuously detects the presence of water, then when water is detected beyond a threshold, the device:  
* Sends an alert through Apple HomeKit  
* Displays the leak status live on the web page  

# **Hardware Requirements**
ESP32	  
Water Sensor  
Power Supply (I used 5V USB)  


# **Software**
* Arduino IDE or a way to upload code (VS Code's PlatformIO)
* Libraries:
  * WiFi.h
  * WebServer.h
  * HomeSpan.h
 
# **Code to change**
const char* ssid = "YOUR_WIFI_SSID";  
const char* password = "YOUR_WIFI_PASSWORD";  


# **Using**
Upload the code to the ESP32. To connect to an Apple device, open the "Home" app and add an accessory. Click More Options and select the sensor. Enter the access code, and you're done.


<sub>This project is licensed under the MIT License — feel free to use and modify it.</sub>
