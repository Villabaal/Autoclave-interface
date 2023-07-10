#include "measuresHandler.h"
#include "Menu.h"

void manualAcepted(const char&);
void manualDenied(const char&);

void requestManual(const char& req){
  Port0.fetch(req, manualAcepted, manualDenied );
}

void manualAcepted(const char& req){
  if ( (req==generadorIO) || (req==ResistenciasManual) || (req==BombaManual) ){
    requestMeasures(MedidaNivel); if  (req!=generadorIO) Port0.fetch(req); return;
  }
  if ( req==CalentManual ){
    requestMeasures(MedidaTemp); Port0.fetch(req); return;
  }  
  if ( (req==EscapeLentoManual) || (req==EscapeRapidoManual) || (req==AireManual) ){
    requestMeasures(MedidaP_In); Port0.fetch(req); return;
  }  
  if ( req==SuccionManual ){
    requestMeasures(MedidaP_Ex); Port0.fetch(req);
  }    
}

void manualDenied(const char& code){
  lcd.clear();
  if (code == tanqueVacio) lcd.print( bajo );
  else if (code == tanqueLleno) lcd.print( alto );
  else if (code == errorSensorNivel) lcd.print( falla );
  delay(2000);
  menuLcd.update();   
}

