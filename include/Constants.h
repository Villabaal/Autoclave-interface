
#include <Arduino.h>

#pragma once
#ifndef _CONST_H_
#define _CONST_H_  
//codigo del nivel
static constexpr uint8_t full       {2};
static constexpr uint8_t medium     {3};
static constexpr uint8_t empty      {1};
static constexpr uint8_t broken     {0};


//codigos de peticiones y denegaciones
//peticiones de puerta    
static constexpr uint8_t abrirPuerta   {0};
static constexpr uint8_t cerrarPuerta  {1};
static constexpr uint8_t detenerPuerta {2};
//peticiones de medidas
static constexpr uint8_t MedidaP_In           {10};
static constexpr uint8_t MedidaP_Ex           {11};
static constexpr uint8_t MedidaTemp           {12};
//peticiones de alarma
static constexpr uint8_t buzzerIO      {20};
//peticiones de generador
static constexpr uint8_t generadorIO        {30};
static constexpr uint8_t MedidaNivel        {31};
static constexpr uint8_t ResistenciasManual {32};
static constexpr uint8_t BombaManual        {33};
//peticiones del proceso 
static constexpr uint8_t Process121           {40};
static constexpr uint8_t Process134           {41};
static constexpr uint8_t ProcessCorto         {42};
static constexpr uint8_t Resumen              {43};
static constexpr uint8_t AireManual           {44};
static constexpr uint8_t SuccionManual        {45};
static constexpr uint8_t EscapeLentoManual    {46};
static constexpr uint8_t EscapeRapidoManual   {47};
static constexpr uint8_t CalentManual         {48};
static constexpr uint8_t SaltarFase           {49};
//denegacion de la peticion de generador
static constexpr uint8_t tanqueLleno          {2};
static constexpr uint8_t tanqueVacio          {1};
static constexpr uint8_t errorSensorNivel     {0};
//denegacion del proceso 
static constexpr uint8_t puertaAbierta         {0};
static constexpr uint8_t presionInsuficiente   {1};

//etapas del proceso
static constexpr uint8_t Prewarm              { 0 };
static constexpr uint8_t Warm                 { 1 };
static constexpr uint8_t Sterelization        { 2 };
static constexpr uint8_t Expulsion            { 3 };
static constexpr uint8_t Refill               { 4 };
static constexpr uint8_t Drying               { 5 };
static constexpr uint8_t Finish               { 6 };
static constexpr uint8_t Free                 { 7 };

//cadenas repetidaas
static constexpr char abrir[] {"Abrir"};
static constexpr char cerrar[] {"Cerrar"};

static constexpr char bajo[] {"bajo"};
static constexpr char medio[] {"medio"};
static constexpr char alto[] {"alto"};
static constexpr char falla[] {"falla"};

static constexpr char nivel[] {"Nivel"};
static constexpr char presInt[] {"Presion in."};
static constexpr char presExt[] {"Presion ex."};
static constexpr char tempInt[] {"Temp. interior"};

#endif