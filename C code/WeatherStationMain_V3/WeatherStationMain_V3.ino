//Shady Ammar
//Date: 1-12-2019
//Weather Station Main Program
//======================================
#include <Wire.h>   //I2C for airquality sensor (CCS811)
#include <SPI.h>    //SPI for BME sensor

/*GPS*/
#include <SoftwareSerial.h>
SoftwareSerial gpsSerial(10,8); // Rx, Tx
char Massege_id[6];
char gps_buffer[50];
void Print(char Array[],unsigned char len){
  int i =0;
      while(i < len){
            Serial.print(Array[i]);
            i++;
      }
      Serial.write("\n");  
}

/*Airquality sensor (CCS811) */
#include "SparkFunCCS811.h"   
#define CCS811_ADDR 0x5B //Default I2C Address
CCS811 airquality(CCS811_ADDR);

/*BME sensor*/
#include "SparkFunBME280.h"
BME280 BME;
//#define CS_BME_SPI 10   //Chip select for BME sensor in SPI mode

/*Wind Speed and Direction Sensors*/
#include "WindAnemRain.h"
WindAnemRain windSensors(A0,3,2);

unsigned char temp;
unsigned char len;

//LEDs
#define ON_OFFpin 3
#define CONNECTEDpin 4
void setup() {
  //ON/OFF LED
  pinMode(ON_OFFpin,OUTPUT);
  digitalWrite(ON_OFFpin,HIGH);

  //Connected LED
  pinMode(CONNECTEDpin,OUTPUT);
  
  /*Communication Modules*/
  Serial.begin(57600);  //Initialize the serial with baud rate required for telemetry
  gpsSerial.begin(9600);
  Wire.begin();         //Initialize I2C

  /*Airquality sensor setup*/
  CCS811Core::status returnCode = airquality.begin();
  if (returnCode != CCS811Core::SENSOR_SUCCESS){
    Serial.println("error:airquality");
    while (1); //Freeze
  }

  /*BME setup*/
  if (BME.beginI2C() == false){ //Begin communication over I2C
    Serial.println("error:BME");
    while(1); //Freeze
  }
  
}

int count;
bool LED;

void loop() {
  /*Airquality data*/  
  if(airquality.dataAvailable()){
    airquality.readAlgorithmResults();
    Serial.print("AQ:");
    Serial.print("CO2[");
    //Returns calculated CO2 reading
    Serial.print(airquality.getCO2());
    Serial.print("],tVOC[");
    //Returns calculated TVOC reading
    Serial.print(airquality.getTVOC());
    Serial.print("]");
    Serial.println();
  }
  
  /*BME data*/
  Serial.print("BME:");
  Serial.print("Humidity[");
  Serial.print(BME.readFloatHumidity(), 0);
  Serial.print("],Pressure[");
  Serial.print(BME.readFloatPressure(), 0);
  Serial.print("],Alt[");
  Serial.print(BME.readFloatAltitudeMeters(), 1);
  //Serial.print(BME.readFloatAltitudeFeet(), 1);
  Serial.print("],Temp[");
  Serial.print(BME.readTempC(), 2);
  //Serial.print(BME.readTempF(), 2);
  Serial.print("]");
  Serial.println();
  
  /*Wind Data*/
  Serial.print("WIND:");
  Serial.print("Direction[");
  Serial.print(windSensors.getWindDirection());
  Serial.print("],Wind Speed[");
  Serial.print(windSensors.getWindSpeed());
  Serial.print("],Rain[");
  Serial.print(windSensors.getRain());
  Serial.println("]");

  /*GPS*/
  
  /*while(gpsSerial.available()>0){   
    if(gpsSerial.read() == '$')
    {
      //Serial.println("$ readed");
      gpsSerial.readBytesUntil(',',Massege_id,'7');
      if(Massege_id[2]=='G' && Massege_id[3]=='G' && Massege_id[4]=='A')
      {
        len = gpsSerial.readBytesUntil('$',gps_buffer,'30');
        Serial.print("GGA:");
        Print(gps_buffer,len);
      }
      else if(Massege_id[2]=='R' && Massege_id[3]=='M' && Massege_id[4]=='C'){
          len = gpsSerial.readBytesUntil('$',gps_buffer,'30');   
          Serial.print("RMC:");
          Print(gps_buffer,len);
      }
      
    }
    /*temp = gpsSerial.read();
    if(temp != '$'){
        Serial.write(gpsSerial.read());
    }else{
      Serial.println();
    }*/
  //}

  while(gpsSerial.available()>0){
    if(gpsSerial.read() == '$')
    {
      gpsSerial.readBytesUntil(',',Massege_id,'7');
      if(Massege_id[2]=='R' && Massege_id[3]=='M' && Massege_id[4]=='C'){
          len = gpsSerial.readBytesUntil('$',gps_buffer,'30');   
          Serial.print("RMC:");
          Print(gps_buffer,len);
      }
    }
  }

  if(count > 10){
      count = 0;
      LED = !LED;
      digitalWrite(CONNECTEDpin,LED);
    }else{
        count++;
      }
  //Serial.println();
  //delay(100); //not needed due to the 1 second delay in the WindSpeed
}
