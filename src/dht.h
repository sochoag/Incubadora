#include "DHTesp.h"
#include <variables.h>

DHTesp dht;

void dhtSetup()
{
    dht.setup(13, DHTesp::DHT11);
}

void dhtGetVals()
{
    temperatura = dht.getTemperature();
    humedad = dht.getHumidity();
}
