#ifndef ServerConnect
#define ServerConnect

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <Losant.h>

// WiFi credentials.
const char* WIFI_SSID = "Familia Ochoa";
const char* WIFI_PASS = "0301072468";

// Losant credentials.
const char* LOSANT_DEVICE_ID = "5f9c7dc3106a3a00074606c9";
const char* LOSANT_ACCESS_KEY = "8bb2306b-e8f8-45a6-9918-521615881535";
const char* LOSANT_ACCESS_SECRET = "ee27a76c29a4a1f6cccd51c47c62fcfe1dd8d95f1cef923c5d79677a8640fc3e";


static const char digicert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3
d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD
QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT
MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j
b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG
9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB
CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97
nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt
43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P
T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4
gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO
BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR
TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw
DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr
hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg
06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF
PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls
YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk
CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=
-----END CERTIFICATE-----
)EOF";

BearSSL::WiFiClientSecure wifiClient;

LosantDevice device(LOSANT_DEVICE_ID);

void setClock() 
{
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    now = time(nullptr);
  }
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
}

void connect() 
{
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("Conectando a Wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(".");
  Serial.println("Conectado!");
  Serial.println("Conectando a Losant...");
  BearSSL::X509List cert(digicert);
  wifiClient.setTrustAnchors(&cert);
  setClock();
  // Connect to Losant.
  device.connectSecure(wifiClient, LOSANT_ACCESS_KEY, LOSANT_ACCESS_SECRET);
  while(!device.connected()) 
  {
    delay(500);
    Serial.print("*");
  }
  Serial.println("Conectado");
}

void report(float temperatura, float humedad, bool ventiladores, bool focos, bool motor)
{
  StaticJsonDocument<500> jsonBuffer;
  JsonObject root = jsonBuffer.to<JsonObject>();
  root["Temperatura"] = temperatura;
  root["Humedad"] = humedad;
  root["Ventiladores"] = ventiladores;
  root["Focos"] = focos;
  root["Motor"] = motor;
  device.sendState(root);
  Serial.println("Reported!");
}

#endif