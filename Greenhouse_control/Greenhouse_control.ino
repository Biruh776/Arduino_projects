/**********************************************************
Project title: Automated tabletop greenhouse using
Arduino UNO R3
Sensors: DHT11 temperature and humidity sensor, LDR, 
YL-69 soil moisture sensor
Actuators: Motor, LED, pump, servo
**********************************************************/

#include <Servo.h>
#include <LiquidCrystal.h>
#include <dht.h>

#define DHT11_PIN A0     // what pin it is connected to
Servo my_servo;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
dht DHT;

//watering system wiring
const int pump = 12;
const int moisture_sensor = A2;
int moisture_value;

//lighting system wiring
const int lamp = 10;
const int ldr = A1;
int ldr_value;
//int brightness;

//cooling system wiring
float hum;  //Stores humidity value
float tempC; //Stores temperature value
const int fan = 11;

//LCD control variables
char LargeText[100]="";
int k=0; 
int iCursor=0;

void setup() {
  
  //initialize the watering system
  pinMode(pump, OUTPUT);
  pinMode(moisture_sensor, INPUT);

  //initialize the lighting system
  pinMode(lamp, OUTPUT);
  pinMode(ldr, INPUT);

  //initialize the cooling system
  pinMode(fan, OUTPUT);
  pinMode(DHT11_PIN, INPUT);
  my_servo.attach(9, 0, 2400);  //define my_servo object
  Serial.begin(9600);  //initializes the serial communication

  //initialize LCD
  lcd.begin(16, 2); 
}

void loop() {
 
  strcpy(LargeText, ""); //Empty string
  //watering system operation control
  digitalWrite(pump, LOW);	//turn off the pump motor before the sensor reading
  moisture_value = analogRead(moisture_sensor);
  moisture_value = constrain(moisture_value, 300, 1023);
  moisture_value = map(moisture_value, 1023, 300, 0, 100);
  if (moisture_value <= 30){
    digitalWrite(pump, HIGH);	//turn on the pump
    strcat(LargeText, "Pump:on  ");
  }
  else if (moisture_value >= 50){
    digitalWrite(pump, LOW);	//turn off the pump
    strcat(LargeText, "Pump:off  ");
  }
    
  //lighting system operation control
  digitalWrite(lamp, LOW);	//turn off the led before the sensor reading
  ldr_value = analogRead(ldr);
  ldr_value = map(ldr_value, 0, 1023, 0, 100);
  if (ldr_value <= 25){ 
  digitalWrite(lamp, HIGH);	//turn lamp on
  strcat(LargeText, "Light:on  ");
  }
  else{
  digitalWrite(lamp, LOW);	//turn lamp off
  strcat(LargeText, "Light:off  ");
  }
  
  //cooling system operation control
  digitalWrite(fan, LOW);  //turn off the fan motor before reading sensor
  delay(1);               //short 1 ms delay before reading sensor
  int chk = DHT.read11(DHT11_PIN); //Read temperature value and store it to variables hum and tempC
  hum = DHT.humidity;
  tempC= DHT.temperature;
  if(tempC > 30)
  {
    my_servo.write(180);	//open the ceiling vent
    digitalWrite(fan, HIGH);  //turn the fan on
    strcat(LargeText, "Fan:on  ");
    strcat(LargeText, "Roof:open  ");    
  }
  else if(tempC < 25)
  {
    my_servo.write(0);    //close the ceiling vent
    digitalWrite(fan, LOW);  //turn the fan off
    strcat(LargeText, "Fan:off  ");
    strcat(LargeText, "Roof:closed  ");
  }
  else
  {
    strcat(LargeText, "Fan:off  ");
    strcat(LargeText, "Roof:closed  ");
  }
  delay(400);
  
  //LCD operation control
  lcd.clear();
  if (k<15){
    lcd.print("Temp: ");	//display temperature
    lcd.print(round(tempC));
    lcd.print("degree C ");
    }
  else if (k<30){
    lcd.print("Humidty: ");	//display humidity
    lcd.println(round(hum));
    }
  else if (k<45){
    lcd.print("Light: ");	//display light intensity
    lcd.println(round(ldr_value));
    }
  else{
    lcd.print("Moisture: ");	//display moisture
    lcd.println(round(moisture_value));
    }
  k++;
  if (k>=60)
    k=0;
    
  Updatelcd();
    
}

//display the scrolling message on the LCD bottom row
void Updatelcd()
{
  int lentxt=strlen(LargeText);
  if (iCursor == lentxt) //if scrolled a full lap 
    iCursor = 0;		//set the iCursor to zero
  lcd.setCursor(0, 1); //set the cursor on the second row
  if(iCursor<lentxt - 16) //if it has 16 more characters after the iCursor'th index
  {
    for (int i = iCursor; i < iCursor + 16; i++) // print 16 characters starting from iCursor'th index
    {
      lcd.print(LargeText[i]);
    }
  }
  else //if it has less than 16 characters after the iCursor'th index
  {
    for (int i=iCursor; i < lentxt; i++) // print the characters left starting from iCursor'th index
    {
      lcd.print(LargeText[i]);
    }
    for (int i = 0; i <= 16 - lentxt+iCursor; i++) // print the next round characters on the spaces left
    {
      lcd.print(LargeText[i]);
    }
  }
  iCursor++;
}
