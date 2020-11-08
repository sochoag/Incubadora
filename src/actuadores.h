#include <Arduino.h>
#include <variables.h>

#ifndef actuadores
#define actuadores

// Focos
#define ssr 15

// Ventiladores
#define ventEn 5
#define ventIn1 4
#define ventIn2 0

// Motor
#define motEn 12
#define motIn1 14
#define motIn2 2

void actuadoresSetup()
{
    pinMode(ssr,OUTPUT);

    pinMode(ventEn,OUTPUT);
    pinMode(ventIn1,OUTPUT);
    pinMode(ventIn2,OUTPUT);

    pinMode(motEn,OUTPUT);
    pinMode(motIn1,OUTPUT);
    pinMode(motIn2,OUTPUT);
}
void actuadoresUpdate()
{
    digitalWrite(ssr,focos);

    analogWrite(ventEn,1023);
    digitalWrite(ventIn1,ventilador);
    digitalWrite(ventIn2,LOW);

    analogWrite(motEn,1023);
    digitalWrite(motIn1,motor);
    digitalWrite(motIn2,LOW);
}

#endif