void Gas_setup() {
	pinMode(GAS_SENSOR_PIN, INPUT);
}

void Gas_loop() {

	if (currentMillis < gas_previous_millis) gas_previous_millis = 0;
	if(currentMillis - gas_previous_millis >= gas_interval) {
		gas_previous_millis = currentMillis;
		GasSensor = digitalRead(GAS_SENSOR_PIN);

		if (GasSensor != Last_GasSensor || currentMillis-Last_Send_millis2 >= 1*60*60*1000) {

			Last_GasSensor = GasSensor;
			Last_Send_millis2 = currentMillis;
			
			MQTT_publish("home/"+ _ssidAP + "/GasSensor",(String)GasSensor);
			
		}

		
		
		
		
		
		if (GasSensor > 0) {
			tone(BUZZER_PIN, 1480, gas_interval+500);
		} else {
			noTone(BUZZER_PIN);
		}
		
		error_log("GasSensor: " + String(GasSensor));
	}
	
}

