





#include <SoftwareSerial.h>
#include<string.h>
#include <RegloCPF.h>

#include <Time.h>

// Create a software serial device to communicate via bit-blasting with the Reglo-CPF
// pump.
SoftwareSerial regloSerial(10, 11);    // RX, TX

// Create a pump controller using the serial port, with the default address.
RegloCPF pump(&regloSerial, 2);

int pin_lamp1=2;
int pin_lamp2=3;

int pin_PV_cell_voltage_meassure=A0;
int pin_PV_cell_current_meassure=A1;
int pin_waterflow_temperature_spiral_middle_outlet_meassure=A2;
int pin_waterflow_temperature_spiral_edge_outlet_meassure=A3;
int pin_solar_cell_temperature_top=A4;
int pin_solar_cell_temperature_bottom=A5;




long R_2=11000;  
long R_1=220000; 



void collect_data_solar_cell_temperature_current_voltage(){

  digitalWrite(pin_lamp2, HIGH) ;
  digitalWrite(pin_lamp1, HIGH) ;

  
  double temperature_bottom= meassure_temperature(10,0,pin_solar_cell_temperature_bottom);
  Serial.print(temperature_bottom);
  Serial.print (", " );

  double temperature_top= meassure_temperature(10,0,pin_solar_cell_temperature_top);
  Serial.print(temperature_top);
  Serial.print (", " );

  double voltage= meassure_voltage(10);
  Serial.print(voltage);
  Serial.print (", " );
 
  double current= meassure_current(50);
  Serial.println(current,5);  


  


  
}




void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(day());
  Serial.print(".");
  Serial.print(month());
  Serial.print(".");
  Serial.print(year()); 
  Serial.print(" ");
  Serial.print(hour());
  Serial.print(":");
  Serial.print(minute());
  Serial.print(":");
  Serial.print(second());
}





void setup() {
  setTime(14,00,0,01,9,15);  // 14:00:00 on 01.9.2015
 
  // Start serial communication.
  Serial.begin(9600);
  regloSerial.begin(9600);

  pinMode(pin_lamp1,OUTPUT);
  pinMode(pin_lamp2,OUTPUT);
  pinMode(pin_PV_cell_voltage_meassure,INPUT);
  pinMode(pin_PV_cell_current_meassure,INPUT);
  pinMode(pin_waterflow_temperature_spiral_middle_outlet_meassure,INPUT);
  pinMode(pin_waterflow_temperature_spiral_edge_outlet_meassure,INPUT);
  pinMode(pin_solar_cell_temperature_top,INPUT);
   pinMode(pin_solar_cell_temperature_bottom,INPUT);

  //analogReference(INTERNAL);
}



//The function calculates the voltage average over number_of_meassure measurements.
double meassure_voltage(int number_of_meassure){
  
  double add=0;

  for (int i=0;i<number_of_meassure;i++){
    int sensorValue=analogRead(pin_PV_cell_voltage_meassure);  
    double voltage_solar_cell=sensorValue/(double)1024*5*(R_1+R_2)/(double)R_2;
    add=add+voltage_solar_cell;
    delay(0.001);  
  }
  return add/(double)number_of_meassure;
}


//The function calculates the current average over number_of_meassure measurements.
double meassure_current(int number_of_meassure){
  
  double add=0;
  int  sensorValue_current=analogRead(pin_PV_cell_current_meassure);

  for (int i=0;i<number_of_meassure;i++){
    sensorValue_current=analogRead(pin_PV_cell_current_meassure);
    double Voltage=sensorValue_current/ 1023.0 * 5;  // V
    double ampere = -0.316832*Voltage+0.3517-0.15;  // -0.15 was derived by calibration against a three-digit ammeter
    add=add+ampere;
    delay(0.001);
  }
  return add/(double)number_of_meassure;
}



//The function calculates the temperature average over number_of_meassure measurements.
// reference voltage = 0 if amplifier board is short-circuit; otherwise the value should be around 1.25
//pin = Name of the pin, which is connected to the sensor (amplifier board)
double meassure_temperature(int number_of_meassure, double reference_voltage, int pin){
  double add=0;
  for (int i=0;i<number_of_meassure;i++){
    int  sensorValue_temperature=analogRead(pin);
    double temperature=((sensorValue_temperature*5)/(double)1024-reference_voltage)/(double)0.005;

    add+=temperature;
      delay(0.001);
  }
  return add/(double) number_of_meassure; 
}



  
void loop() {
 collect_data_solar_cell_temperature_current_voltage();


/*
double     temperature_spiral_middle_outlet= meassure_temperature(10,1.25,pin_waterflow_temperature_spiral_edge_outlet_meassure);
  Serial.print ("temperature edge outlet: " );
  Serial.print(temperature_spiral_middle_outlet);
  Serial.println (" Celsius" );

   temperature_spiral_middle_outlet= meassure_temperature(10,0,pin_waterflow_temperature_spiral_middle_outlet_meassure);
  Serial.print ("temperature middle outlet: " );
  Serial.print(temperature_spiral_middle_outlet);
  Serial.println (" Celsius" );

  
temperature_spiral_middle_outlet= meassure_temperature(10,0,pin_solar_cell_temperature_bottom);
  Serial.print ("temperature bottom: " );
  Serial.print(temperature_spiral_middle_outlet);
  Serial.println (" Celsius" );


  temperature_spiral_middle_outlet= meassure_temperature(10,0,pin_solar_cell_temperature_top);
  Serial.print ("temperature top: " );
  Serial.print(temperature_spiral_middle_outlet);
  Serial.println (" Celsius" );
*/
 


  

  


/*

int mantisse=35;
int exponent=0;

int test_1 =pump.set_flow_rate(&mantisse,&exponent);

pump.start();
delay(3000);
pump.stop();
delay(3000);


mantisse=20;
exponent=0;

int test_2 =pump.set_flow_rate(&mantisse,&exponent);

pump.start();
delay(3000);
pump.stop();
delay(3000);

mantisse=-1;
exponent=-1;
pump.get_flow_rate(&mantisse,&exponent);
Serial.println("get flow rate says: mantisse=" +String(mantisse)+" exponent="+String(exponent));


  double temperature_spiral_middle_outlet= meassure_temperature(10,0,pin_waterflow_temperature_spiral_middle_outlet_meassure);
  Serial.print ("temperature spiral middle in-/outlet: " );
  Serial.print(temperature_spiral_middle_outlet);
  Serial.println (" Celsius" );

  double temperature_spiral_edge_outlet= meassure_temperature(10,1.25,pin_waterflow_temperature_spiral_edge_outlet_meassure);
  Serial.print ("temperature spiral edge in-/outlet: " );
  Serial.print(temperature_spiral_edge_outlet);
  Serial.println (" Celsius" );


  double voltage= meassure_voltage(10);
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println(" Volt");

  double current= meassure_current(50);
  Serial.print("Current: ");
  Serial.print(current,5);
  Serial.println(" Ampere");


  delay(5000000000);
*/
}
