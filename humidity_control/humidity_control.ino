#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <SimpleTimer.h>


#define DHTPIN D1     // PIN DHT22
#define DHTTYPE DHT22   // tipe DHT
#define EXFAN_PIN D2   // relay exhaust fan
#define WLFAN_PIN D3 // relay wallfan
WidgetLED FAN(V0);
SimpleTimer timer;

char auth[] = ""; //token blynk
char ssid[] = ""; //ssid wifi
char pass[] = ""; //pass wifi

float humDHT = 0; //inisialisasi nilai kelembaban
float tempDHT = 0; //inisialisasi nilai temperature
int ValH=0; //inisialisasi value kelembaban
int ValT=0;
int mode = 0; //mode ON, OFF, AUTO
// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

void sendSensor()
{
  delay(2000);
  humDHT = dht.readHumidity();
  tempDHT = dht.readTemperature();

  if (isnan(humDHT) || isnan(tempDHT)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
 
}

void setup() {
  Serial.begin(115200);
  pinMode(EXFAN_PIN, OUTPUT); //output fan 1
  digitalWrite(EXFAN_PIN, LOW); //inisialisasi value fan1
  pinMode (WLFAN_PIN, OUTPUT); //output fan 2
  digitalWrite (WLFAN_PIN, LOW); // inisialisasi value fan2
  Serial.println(F("DHT22 test!"));
  dht.begin();
  humDHT = dht.readHumidity(); //inisialisasi nilai kelembaban
  tempDHT = dht.readTemperature();
  timer.setInterval(1000L, sendSensor);
  Blynk.begin( auth, ssid , pass, "", 8080 );
}

BLYNK_WRITE(V3)
{
  ValH = param.asInt(); // mengisi parameter kelembaban
  Serial.print(" The Threshhold  Humidity value is: ");
  Serial.println(ValH);
  Serial.println();
}

BLYNK_WRITE(V4)
{
  if (humDHT < ValH){
    mode = param.asInt();
    Serial.print("Mode sekarang: ");
    Serial.println(mode);
    Serial.println();
  }
  else if (humDHT > ValH){
    mode = 1;
    Serial.print("Mode sekarang: ");
    Serial.println(mode);
    Serial.println();
  }
  else{
    mode = param.asInt();
    Serial.print("Mode sekarang: ");
    Serial.println(mode);
    Serial.println();
  }

}


void loop() {
  
  Blynk.run();
  timer.run();


  Serial.print(F("Temperature: "));
  Serial.print(tempDHT);
  Serial.print(F("°C "));
  Serial.println();
  Serial.print(F("Humidity: "));
  Serial.print(humDHT);
  Serial.print(F("%"));
  Serial.println();
  
  Serial.println("***********************");
  Serial.println();

  // Compare Threshold value from Blynk and DHT Temperature value.
  
 /* if (ValH > humDHT)
  {
    digitalWrite(EXFAN_PIN, HIGH);
    FAN.off();
    Blynk.notify("Exhaust Fan ON !");
    
}
  else {
    digitalWrite(EXFAN_PIN, LOW);
    FAN.on();
    Blynk.notify("Exhaust Fan OFF !");
    
  } 
*/
if (mode == 2){
  digitalWrite(EXFAN_PIN, HIGH);
  FAN.off();
  Blynk.notify("Exhaust Fan ON !");
}
else if(mode == 3){
  digitalWrite(EXFAN_PIN, LOW);
  FAN.on();
  Blynk.notify("Exhaust Fan OFF !");
}
else{
  if (ValH > humDHT)
  {
    digitalWrite(EXFAN_PIN, HIGH);
    FAN.off();
    Blynk.notify("Exhaust Fan ON !");
    
}
  else {
    digitalWrite(EXFAN_PIN, LOW);
    FAN.on();
    Blynk.notify("Exhaust Fan OFF !");
    
  } 
}
  Blynk.virtualWrite(V1, tempDHT);
  Blynk.virtualWrite(V2, humDHT);
}
