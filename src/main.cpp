#include <Arduino.h>
#include <variables.h>
#include <ServerConnect.h>
#include <dht.h>
#include <actuadores.h>

void setup() 
{
  Serial.begin(115200);
  connect();
  dhtSetup();
  actuadoresSetup();
}

void loop() 
{
  bool toReconnect = false;

  if(WiFi.status() != WL_CONNECTED) 
  {
    Serial.println("Disconnected from WiFi");
    toReconnect = true;
  }

  if(!device.connected()) 
  {
    Serial.println("Disconnected from Losant");
    toReconnect = true;
  }

  if(toReconnect) 
  {
    connect();
  }

  device.loop();

  long t = millis();
  
  if (temperatura > 20)
  {
      ventilador = true;
      focos = false;
  }
  else
  {
      ventilador = false;
      focos = true;
  }

  if (t - t1 > t_espera)
  {
    t1 = t;
    t_espera = random(10000, 60000);
    motor = !motor;
  }

  // Publicaciones

  if (t - t0 > 5000)
  {
    t0 = t;
    dhtGetVals();
    Serial.println("Publicando valores de temperatura y humedad");
    Serial.println("Temperatura: " + String(temperatura) + "\t\t Humedad: " + String(humedad));
    report(temperatura, humedad, ventilador, focos, motor);
  }

  actuadoresUpdate();
}