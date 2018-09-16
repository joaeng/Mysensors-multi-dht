#define MY_DEBUG
#define MY_RADIO_NRF24
#define DHTTYPE DHT21 
#include <SPI.h>
#include <MySensors.h>  
#include <DHT.h>

#define CHILD_ID_HUM1 1
#define CHILD_ID_TEMP1 2
#define DHT1_DATA_PIN 3

#define SENSOR_TEMP1_OFFSET 0

#define CHILD_ID_HUM2 3
#define CHILD_ID_TEMP2 4
#define DHT2_DATA_PIN 4

#define SENSOR_TEMP2_OFFSET 0

#define CHILD_ID_HUM3 5
#define CHILD_ID_TEMP3 6
#define DHT3_DATA_PIN 5

#define SENSOR_TEMP3_OFFSET 0

// Mina egna tillägg

#define CHILD_ID_HUM4 7
#define CHILD_ID_TEMP4 8
#define DHT4_DATA_PIN 6

#define SENSOR_TEMP4_OFFSET 0

#define CHILD_ID_HUM5 9
#define CHILD_ID_TEMP5 10
#define DHT4_DATA_PIN 7

#define SENSOR_TEMP5_OFFSET 0

// Force sending an update of the temperature after n sensor reads, so a controller showing the
static const uint8_t FORCE_UPDATE_N_READS = 2;

// Must be >1000ms for DHT22 and >2000ms for DHT11
static const uint64_t UPDATE_INTERVAL = 30000;

DHT dht1;
DHT dht2;
DHT dht3;
// eget 
DHT dht4;
DHT dht5;

float lastTemp1;
float lastHum1;
float lastTemp2;
float lastHum2;
float lastTemp3;
float lastHum3;
// eget 
float lastTemp4;
float lastHum4;
float lastTemp5;
float lastHum5;

uint8_t nNoUpdatesTemp1;
uint8_t nNoUpdatesHum1;
uint8_t nNoUpdatesTemp2;
uint8_t nNoUpdatesHum2;
uint8_t nNoUpdatesTemp3;
uint8_t nNoUpdatesHum3;
// eget 
uint8_t nNoUpdatesTemp4;
uint8_t nNoUpdatesHum4;
uint8_t nNoUpdatesTemp5;
uint8_t nNoUpdatesHum5;
bool metric = true;

MyMessage msgHum1(CHILD_ID_HUM1, V_HUM);
MyMessage msgTemp1(CHILD_ID_TEMP1, V_TEMP);
MyMessage msgHum2(CHILD_ID_HUM2, V_HUM);
MyMessage msgTemp2(CHILD_ID_TEMP2, V_TEMP);
MyMessage msgHum3(CHILD_ID_HUM3, V_HUM);
MyMessage msgTemp3(CHILD_ID_TEMP3, V_TEMP);
// eget 
MyMessage msgHum4(CHILD_ID_HUM4, V_HUM);
MyMessage msgTemp4(CHILD_ID_TEMP4, V_TEMP);
MyMessage msgHum5(CHILD_ID_HUM5, V_HUM);
MyMessage msgTemp5(CHILD_ID_TEMP5, V_TEMP);


void presentation()  
{ 
  // Send the sketch version information to the gateway
  sendSketchInfo("IncubatorTempAndHum", "1.1");
  delay(15000); 
  // Register all sensors to gw (they will be created as child devices)
  present(CHILD_ID_HUM1, S_HUM);
  present(CHILD_ID_TEMP1, S_TEMP);
  delay(15000); 
  present(CHILD_ID_HUM2, S_HUM);
  present(CHILD_ID_TEMP2, S_TEMP);
  delay(15000);  
  present(CHILD_ID_HUM3, S_HUM);
  present(CHILD_ID_TEMP3, S_TEMP);
// eget 
  delay (15000);
  present(CHILD_ID_HUM4, S_HUM);
  present(CHILD_ID_TEMP4, S_TEMP);
    delay (15000);
  present(CHILD_ID_HUM5, S_HUM);
  present(CHILD_ID_TEMP5, S_TEMP);
  metric = getControllerConfig().isMetric;
}


void setup()
{
  dht1.setup(DHT1_DATA_PIN);
  dht2.setup(DHT2_DATA_PIN);
  dht3.setup(DHT3_DATA_PIN);
  // eget
  dht4.setup(DHT3_DATA_PIN);
  dht5.setup(DHT3_DATA_PIN);
  // Sleep for the time of the minimum sampling period to give the sensor time to power up
  // (otherwise, timeout errors might occure for the first reading)
  sleep(dht1.getMinimumSamplingPeriod());
  sleep(dht2.getMinimumSamplingPeriod());
  sleep(dht3.getMinimumSamplingPeriod());
  // eget
  sleep(dht4.getMinimumSamplingPeriod());
  sleep(dht5.getMinimumSamplingPeriod());
}


void loop()      
{  
  // Force reading sensor, so it works also after sleep()
  dht1.readSensor(true);
  dht2.readSensor(true);
  dht3.readSensor(true);
  // eget 
  dht4.readSensor(true);
  dht5.readSensor(true);

  // Get temperature from DHT library
  float temperature1 = dht1.getTemperature();
  if (isnan(temperature1)) {
    Serial.println("Failed reading temperature from DHT1!");
  } else if (temperature1 != lastTemp1 || nNoUpdatesTemp1 == FORCE_UPDATE_N_READS) {
    // Only send temperature if it changed since the last measurement or if we didn't send an update for n times
    lastTemp1 = temperature1;
    if (!metric) {
      temperature1 = dht1.toFahrenheit(temperature1);
    }
    // Reset no updates counter
    nNoUpdatesTemp1 = 0;
    temperature1 += SENSOR_TEMP1_OFFSET;
    send(msgTemp1.set(temperature1, 1));

    #ifdef MY_DEBUG
    Serial.print("T: ");
    Serial.println(temperature1);
    #endif
    delay(10000); 
  } else {
    // Increase no update counter if the temperature stayed the same
    nNoUpdatesTemp1++;
  }

  // Get humidity from DHT library
  float humidity1 = dht1.getHumidity();
  if (isnan(humidity1)) {
    Serial.println("Failed reading humidity from DHT1");
  } else if (humidity1 != lastHum1 || nNoUpdatesHum1 == FORCE_UPDATE_N_READS) {
    // Only send humidity if it changed since the last measurement or if we didn't send an update for n times
    lastHum1 = humidity1;
    // Reset no updates counter
    nNoUpdatesHum1 = 0;
    send(msgHum1.set(humidity1, 1));

    #ifdef MY_DEBUG
    Serial.print("H: ");
    Serial.println(humidity1);
    #endif
    delay(10000); 
  } else {
    // Increase no update counter if the humidity stayed the same
    nNoUpdatesHum1++;
  }
  float temperature2 = dht2.getTemperature();
  if (isnan(temperature2)) {
    Serial.println("Failed reading temperature from DHT2!");
  } else if (temperature2 != lastTemp2 || nNoUpdatesTemp2 == FORCE_UPDATE_N_READS) {
    // Only send temperature if it changed since the last measurement or if we didn't send an update for n times
    lastTemp2 = temperature2;
    if (!metric) {
      temperature2 = dht2.toFahrenheit(temperature2);
    }
    // Reset no updates counter
    nNoUpdatesTemp2 = 0;
    temperature2 += SENSOR_TEMP2_OFFSET;
    send(msgTemp2.set(temperature2, 1));

    #ifdef MY_DEBUG
    Serial.print("T: ");
    Serial.println(temperature2);
    #endif
    delay(10000); 
  } else {
    // Increase no update counter if the temperature stayed the same
    nNoUpdatesTemp2++;
  }

  // Get humidity from DHT library
  float humidity2 = dht2.getHumidity();
  if (isnan(humidity2)) {
    Serial.println("Failed reading humidity from DHT2");
  } else if (humidity2 != lastHum2 || nNoUpdatesHum2 == FORCE_UPDATE_N_READS) {
    // Only send humidity if it changed since the last measurement or if we didn't send an update for n times
    lastHum2 = humidity2;
    // Reset no updates counter
    nNoUpdatesHum2 = 0;
    send(msgHum2.set(humidity2, 1));

    #ifdef MY_DEBUG
    Serial.print("H: ");
    Serial.println(humidity2);
    #endif
    delay(10000); 
  } else {
    // Increase no update counter if the humidity stayed the same
    nNoUpdatesHum2++;
  }
  float temperature3 = dht3.getTemperature();
  if (isnan(temperature3)) {
    Serial.println("Failed reading temperature from DHT3!");
  } else if (temperature3 != lastTemp3 || nNoUpdatesTemp3 == FORCE_UPDATE_N_READS) {
    // Only send temperature if it changed since the last measurement or if we didn't send an update for n times
    lastTemp3 = temperature3;
    if (!metric) {
      temperature3 = dht3.toFahrenheit(temperature3);
    }
    // Reset no updates counter
    nNoUpdatesTemp3 = 0;
    temperature3 += SENSOR_TEMP3_OFFSET;
    send(msgTemp3.set(temperature3, 1));

    #ifdef MY_DEBUG
    Serial.print("T: ");
    Serial.println(temperature3);
    #endif
    delay(10000); 
  } else {
    // Increase no update counter if the temperature stayed the same
    nNoUpdatesTemp3++;
  }

  // Get humidity from DHT library
  float humidity3 = dht3.getHumidity();
  if (isnan(humidity3)) {
    Serial.println("Failed reading humidity from DHT3");
  } else if (humidity3 != lastHum3 || nNoUpdatesHum3 == FORCE_UPDATE_N_READS) {
    // Only send humidity if it changed since the last measurement or if we didn't send an update for n times
    lastHum3 = humidity3;
    // Reset no updates counter
    nNoUpdatesHum3 = 0;
    send(msgHum3.set(humidity3, 1));

    #ifdef MY_DEBUG
    Serial.print("H: ");
    Serial.println(humidity3);
    #endif
    delay(10000); 
  } else {
    // Increase no update counter if the humidity stayed the same
    nNoUpdatesHum3++;
  }

  // eget 

    float temperature4 = dht4.getTemperature();
  if (isnan(temperature4)) {
    Serial.println("Failed reading temperature from DHT4!");
  } else if (temperature4 != lastTemp4 || nNoUpdatesTemp4 == FORCE_UPDATE_N_READS) {
    // Only send temperature if it changed since the last measurement or if we didn't send an update for n times
    lastTemp4 = temperature4;
    if (!metric) {
      temperature4 = dht4.toFahrenheit(temperature4);
    }
    // Reset no updates counter
    nNoUpdatesTemp4 = 0;
    temperature4 += SENSOR_TEMP4_OFFSET;
    send(msgTemp4.set(temperature4, 1));

    #ifdef MY_DEBUG
    Serial.print("T: ");
    Serial.println(temperature4);
    #endif
    delay(10000); 
  } else {
    // Increase no update counter if the temperature stayed the same
    nNoUpdatesTemp4++;
  }

  // Get humidity from DHT library
  float humidity4 = dht4.getHumidity();
  if (isnan(humidity4)) {
    Serial.println("Failed reading humidity from DHT4");
  } else if (humidity4 != lastHum4 || nNoUpdatesHum4 == FORCE_UPDATE_N_READS) {
    // Only send humidity if it changed since the last measurement or if we didn't send an update for n times
    lastHum4 = humidity4;
    // Reset no updates counter
    nNoUpdatesHum4 = 0;
    send(msgHum4.set(humidity4, 1));

    #ifdef MY_DEBUG
    Serial.print("H: ");
    Serial.println(humidity4);
    #endif
    delay(10000); 
  } else {
    // Increase no update counter if the humidity stayed the same
    nNoUpdatesHum4++;
  }
      float temperature5 = dht5.getTemperature();
  if (isnan(temperature5)) {
    Serial.println("Failed reading temperature from DHT5!");
  } else if (temperature5 != lastTemp5 || nNoUpdatesTemp5 == FORCE_UPDATE_N_READS) {
    // Only send temperature if it changed since the last measurement or if we didn't send an update for n times
    lastTemp5 = temperature5;
    if (!metric) {
      temperature5 = dht5.toFahrenheit(temperature5);
    }
    // Reset no updates counter
    nNoUpdatesTemp5 = 0;
    temperature5 += SENSOR_TEMP5_OFFSET;
    send(msgTemp5.set(temperature5, 1));

    #ifdef MY_DEBUG
    Serial.print("T: ");
    Serial.println(temperature5);
    #endif
    delay(10000); 
  } else {
    // Increase no update counter if the temperature stayed the same
    nNoUpdatesTemp5++;
  }

  // Get humidity from DHT library
  float humidity5 = dht5.getHumidity();
  if (isnan(humidity5)) {
    Serial.println("Failed reading humidity from DHT5");
  } else if (humidity5 != lastHum5 || nNoUpdatesHum5 == FORCE_UPDATE_N_READS) {
    // Only send humidity if it changed since the last measurement or if we didn't send an update for n times
    lastHum5 = humidity5;
    // Reset no updates counter
    nNoUpdatesHum5 = 0;
    send(msgHum5.set(humidity5, 1));

    #ifdef MY_DEBUG
    Serial.print("H: ");
    Serial.println(humidity5);
    #endif
    delay(10000); 
  } else {
    // Increase no update counter if the humidity stayed the same
    nNoUpdatesHum5++;
  }
  // Sleep for a while to save energy
  sleep(UPDATE_INTERVAL); 
}
