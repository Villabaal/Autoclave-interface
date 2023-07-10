#include "Menu.h"

void processAcepted(const char&);
void processDenied(const char&);


void requestProcess(const char& req){
  Port0.fetch(req,processAcepted,processDenied);
}

void processAcepted(const char&  req){
  bool bussy = true; char laststate=Free;
  auto processState = [&](const char&){
    //parse el reloj
    char state; uint8_t mins, segs;  
    while ( Port0.PORT.available()==0 ); state = Port0.PORT.read();
    if( (state==Sterelization) || (state==Drying) || (state==Finish) ){
      while ( Port0.PORT.available()==0 ); mins = Port0.PORT.read();
      while ( Port0.PORT.available()==0 ); segs = Port0.PORT.read();   
    }
    if (state != laststate){  //cambio la fase ??
      lcd.clear();
      if (state==Prewarm) lcd.print("Precalent.");
      else if (state==Warm) lcd.print("Calentamiento");
      else if (state==Sterelization) lcd.print("Esterilizacion");
      else if (state==Expulsion) lcd.print("Expulsion");
      else if (state==Refill) lcd.print("Cargando Vapor");
      else if (state==Drying) lcd.print("Secado");
      else if (state==Finish) lcd.print("Terminando");
      else bussy = false;                                   
      laststate = state;
    }    
    //imprimir el relog
   if ( (state==Sterelization) || (state==Drying) || (state==Finish) ){
      lcd.setCursor(0,1); lcd.print("       "); lcd.setCursor(0,1);    
      lcd.print(mins);lcd.write(':');lcd.print(segs);
    }    
  };
  //muestreo para pedir el estado del proceso
  Sampler.lastTimer = millis()-2000;
  do{
    Sampler( 2000 , [processState](){ Port0.fetch( Resumen, processState ); } );
    //si presionamos el boton modo saltamos una etapa del proceso
    //si estamos en modo manual
    modo( [](void){ if (manual) Port0.fetch( SaltarFase ); } );
  } while (bussy);
  menuLcd.next_screen(); menuLcd.set_focusedLine(1); menuLcd.update();
}

void processDenied(const char&  code){
  lcd.clear();  
  if ( code == puertaAbierta ) lcd.print("Puerta abierta");
  else if ( code == presionInsuficiente ) lcd.print("Falta presion");
  delay(2000);  
  menuLcd.update();
}

