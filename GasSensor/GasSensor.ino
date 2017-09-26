#include <ESP8266WiFi.h> 
#include <ESP8266WebServer.h>
#include <ESP8266SSDP.h> 
#include <FS.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <ESP8266HTTPUpdateServer.h>
#include "DHT.h"

#define GAS_SENSOR_PIN   13
#define BUZZER_PIN 	     14
#define DHT22_PIN 	      4
#define INNER_LED_PIN     2

#define DHTTYPE DHT22

// Объект для обнавления с web страницы 
ESP8266HTTPUpdateServer httpUpdater;

// Web интерфейс для устройства
ESP8266WebServer HTTP;

// Для файловой системы
File fsUploadFile;

DHT dht(DHT22_PIN, DHTTYPE);

String _revision = "1.1"; // Версия кода

// Определяем переменные wifi
String _ssid     = "AKEB"; // Для хранения SSID
String _password = "{password}"; // Для хранения пароля сети
String _ssidAP = "GasSensor";   // SSID AP точки доступа
String _passwordAP = ""; // пароль точки доступа
IPAddress apIP(192, 168, 0, 1);

String _http_user = "admin";
String _http_password = "0000";

String _mqtt_host     = "192.168.1.200";
int _mqtt_port     = 1883;
String _mqtt_user     = "";
String _mqtt_password     = "";

String SSDP_Name = "GasSensor"; // Имя SSDP
int timezone = 3;               // часовой пояс GTM
int SaveCount = 0;

String jsonConfig = "{}";
int port = 80;

int HighMillis=0;
int Rollover=0;

unsigned long currentMillis;
int save_time_interval = 86400*1000;
unsigned long save_previous_millis = 0;
int wifi_mode_time = 2000;
int wifi_mode = 0;
unsigned long wifi_mode_previous_millis = 0;
int wifi_mode_reconnect_millis = 10*60*1000;
unsigned long wifi_mode_reconnect_previous_millis = 0;
int inner_led_state = LOW;
unsigned long mqtt_reconnect_previous_millis = 0;
int mqtt_reconnect_interval = 5000;


unsigned long dht_previous_millis = 0;
int dht_interval = 5000;

float Humidity = 0.0;
float Temperature = 0.0;
float HeatIndex = 0.0;

unsigned long gas_previous_millis = 0;
int gas_interval = 1000;

int GasSensor = 0;

WiFiClient wifiClientForMQTT;
PubSubClient clientForMQTT(wifiClientForMQTT);

String Log = "";

void error_log(String str) {
	error_log(str, true);
}

void error_log(String str, boolean ln) {
	Serial.print(str);
	
	if (ln) Serial.println("");

	Log = "[" + (String)GetTime() + "] " + str + "<br>" + Log;
	
	if (Log.length() >= 4096) {
		Log = Log.substring(0, 4096);
	}
}

void setup() {
	HTTP = ESP8266WebServer (port);
	
	Serial.begin(115200);
	error_log("");
	
	//Запускаем файловую систему
	error_log("Start File System");
	FS_init();
	
	error_log("Load Config File");
	loadConfig();
	
	error_log("Start WiFi");
	//Запускаем WIFI
	WIFI_init();
	
	error_log("Start Time module");
	// Получаем время из сети
	Time_init();

	error_log("Start MQTT module");
	MQTT_init();
	
	//Настраиваем и запускаем SSDP интерфейс
	error_log("Start SSDP");
	SSDP_init();
	
	//Настраиваем и запускаем HTTP интерфейс
	error_log("Start WebServer");
	HTTP_init();
	
	//Настраиваем и запускаем HTTP интерфейс
	error_log("Start mDNS");
	mDNS_init();

	error_log("Start DHT");
	DHT_setup();
	
	error_log("Start DHT");
	Gas_setup();
}

void loop() {
	HTTP.handleClient();
	
	delay(1);
	Time_loop();
	
	WIFI_loop();
	
	FileConfig_loop();
	
	MQTT_loop();

	DHT_loop();

	Gas_loop();
}



