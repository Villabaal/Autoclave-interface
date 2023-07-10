#include <LiquidMenu.h>
#include "Button.h"
#include "request.h"
#include "Constants.h"

#pragma once
#ifndef _MENU_H_
#define _MENU_H_

#define RS 2
#define EN 3
#define D4 4
#define mode 7
#define down 8
#define D5 19
#define D6 18
#define D7 17
void menuSetup();
void Navega(uint16_t);
extern LiquidMenu menuLcd;
extern LiquidCrystal lcd;
extern Button modo;
extern bool manual;
extern Fetch Port0;

#endif