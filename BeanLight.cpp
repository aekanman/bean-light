/* 
This sketch automizes turning a bike taillight ON and OFF. 

The bike light will turn on when the taillight is in vertical position and it moves.

This example code is in the public domain.
*/

// When acceleration change goes beyond this threshold, the LED will blink.
#define THRESHOLD 100
#define BUTTON_PIN 0
#define LED_STATUS_PIN A0

AccelerationReading prevAcc;
uint16_t ledReading = 0;
bool ledIsOn;
// Timer counts how long the taillight has been still
int timer = 0;
//String state;

void setup() {
	// Initial reading
	prevAcc = Bean.getAcceleration(); 
}

void loop() {
	// Check if the taillight is ON or OFF
	ledReading = getLedReading();
	if(ledReading == 0){
		ledIsOn = false;
	}
	else{
		ledIsOn = true;
	}

	// Get the current acceleration with a conversion of 3.9110-3 g/unit.
	AccelerationReading currAcc = Bean.getAcceleration();

	// Find the difference between the current acceleration and that of 200ms ago.
	int accDiff = getaccDiff(prevAcc, currAcc);

	// Update prevAcc for the next loop. 
	prevAcc = currAcc;

	// Check if the conditions are right to turn ON the taillight
	if(accDiff > THRESHOLD// Taillight is moving
	&& !ledIsOn // LED is OFF
	&& currAcc.xAxis < -200){// Taillight is in a vertical position
		// state = "Active";
		timer = 0;
		// Turn the LED ON
		simBtnPress(2000);
	}
	// Check if the conditions are right to turn OFF the taillight
	else if(accDiff < THRESHOLD // Taillight is still && ledIsOn){// LED is ON // state = "Inactive"; timer ++; if(timer > 30){
		 // Turn the LED OFF
		 timer = 0;
		 simBtnPress(2000);
	}
	}else{
		// state = "Other";
		timer = 0;
	}

	// String output = String();
	// output = output + "State: " + state + "\tTimer: " + timer + "\tLED Reading: " + ledReading + "\tAccel Diff: " + accDiff + "\tAccel X Axis: " + currAcc.xAxis;
	// Serial.println(output);

 	Bean.sleep(500);
}

// This function calculates the difference between two acceleration readings
int getaccDiff(AccelerationReading readingOne, AccelerationReading readingTwo)
{
	int deltaX = abs(readingTwo.xAxis - readingOne.xAxis);
	int deltaY = abs(readingTwo.yAxis - readingOne.yAxis);
	int deltaZ = abs(readingTwo.zAxis - readingOne.zAxis);
	// Return the magnitude
	return deltaX + deltaY + deltaZ; 
}

// This function simulates a button press to turn the light ON or OFF
void simBtnPress(int milliseconds)
{
	// Simulate a button press for a duration 
	pinMode(BUTTON_PIN, OUTPUT);
	digitalWrite(BUTTON_PIN, LOW);
	delay(milliseconds);
	// Set BUTTON_PIN as a high impedance input to simulate a "button release" and allow the physical button to function again
	pinMode(BUTTON_PIN, INPUT);
}

// Used a low-pass filter connected to LED_STATUS_PIN to monitor the LED state
// When LED_STATUS_PIN is above a certain value it corresponds to the LED being OFF
uint16_t getLedReading()
{
	uint32_t reading = 0;
	// Take the average of 8 readings over a period of time
	for(int i = 0; i<8; i++){
		reading += analogRead(LED_STATUS_PIN);
		Bean.sleep(40);
	}
	reading = reading/8;
	return reading;
}
