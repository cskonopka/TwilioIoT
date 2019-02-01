#include <DHT.h>
#include <SD.h>
const int chipSelect = 43;

#define SENSOR_PIN (D38)
#define INTERVAL   (10000)
#define DHTTYPE DHT11   // DHT 11 

DHT dht(SENSOR_PIN, DHTTYPE);

File dataFile;

void setup() {
  dht.begin();
  SerialUSB.print("Initializing SD card...");
  pinMode(SS, OUTPUT);
  if (!SD.begin(chipSelect)) {
    SerialUSB.println("Card failed, or not present");
    while (1) ;
  }
  SerialUSB.println("card initialized.");

  dataFile = SD.open("templog.txt", FILE_WRITE);
  if (! dataFile) {
    SerialUSB.println("error opening temp.txt");
    while (1) ;
  }
}

void loop()
{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  String dataString = "";
  for (int analogPin = 0; analogPin < 1; analogPin++) {
    dataString += String(temperature) + ", ";
    dataString += String(humidity);
  }
  dataFile.println(dataString);
  SerialUSB.println(dataString);
  dataFile.flush();
  delay(INTERVAL);
}
