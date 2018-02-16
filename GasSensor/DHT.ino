void DHT_setup() {
	dht.begin();
}

void DHT_loop() {
	if(currentMillis - dht_previous_millis >= dht_interval) {
		dht_previous_millis = currentMillis;

		if (Humidity < 1) {
			Humidity = dht.readHumidity();
			delay(200);
		}
		
		Humidity = dht.readHumidity();
		Temperature = dht.readTemperature();
		if (isnan(Humidity) || isnan(Temperature)) {
			error_log("Error get Temperature");
			return;
		}
		
		if (Humidity != Last_Humidity || Temperature != Last_Temperature || currentMillis-Last_Send_millis >= 1*60*60*1000) {
			
			Last_Send_millis = currentMillis;
			Last_Temperature = Temperature;
			Last_Humidity = Humidity;
			
			HeatIndex = dht.computeHeatIndex(Temperature, Humidity, false);
		
			MQTT_publish("home/"+ _ssidAP + "/Humidity",(String) Humidity);
			MQTT_publish("home/"+ _ssidAP + "/Temperature",(String) Temperature);
			MQTT_publish("home/"+ _ssidAP + "/HeatIndex",(String) HeatIndex);
		}
		
		error_log("Humidity: " + String(Humidity) + " % \tTemperature: " + String(Temperature) + " *C \tHeat index: " + String(HeatIndex) + " *C");
	}
}

