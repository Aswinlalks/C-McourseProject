/* Arduino code for measuring current with ACS712 Hall effect current sensor
**This project is a test code for the coming Measurements and Circuits 
**course project ,Dept. of Electrical and Electronics Engineering ,MITS
***Contributors Anfaz^ ArniPrem^ AnaghaRavikumar^ AswinLal^
*/


const int analogInPin = A0;  // Analog input pin for the sensor
const float ACS_OFFSET = 2.5;  // ACS712 offset voltage at zero current (usually 2.5V for ACS712)
const float ACS_SENSITIVITY = 0.185;  // Sensitivity of the ACS712 sensor for 5A version (185mV/A)

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Read the analog voltage from the sensor
  int sensorValue = analogRead(analogInPin);
  
  // Convert the analog value to voltage (0-5V)
  float voltage = (sensorValue / 1023.0) * 5.0;

  // Calculate the current based on the sensor sensitivity and offset
  float current = (voltage - ACS_OFFSET) / ACS_SENSITIVITY;

  Serial.print("Raw ADC Value: ");
  Serial.print(sensorValue);
  Serial.print("   Voltage: ");
  Serial.print(voltage, 2);
  Serial.print("V   Current: ");
  Serial.print(current, 2);
  Serial.println("A");

  delay(1000);  // You can adjust the sampling rate as needed
}
