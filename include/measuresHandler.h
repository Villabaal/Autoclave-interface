#include "Menu.h"
//peticion de medidas, si la peticion es exitosa crea un bluce 
// e imprime los valores de las mediciones hasta presional el boton
void measuresAcepted(const char&);

static constexpr char psi[] {"Psi"};

void requestMeasures( const char& req ){
  lcd.clear();
  if ( req == MedidaNivel ) { 
    lcd.print(nivel); Port0.fetch( req , measuresAcepted ); return;
    }   
  if ( req == MedidaP_In ){
    lcd.print(presInt);  lcd.setCursor(7,1);   lcd.print( psi );
    Port0.fetch( req , measuresAcepted ); return; 
  }
  if ( req == MedidaP_Ex ){
    lcd.print(presExt); lcd.setCursor(7,1); lcd.print( psi ); 
    Port0.fetch( req , measuresAcepted ); return;
  }  
  if ( req == MedidaTemp ){
    lcd.print(tempInt); lcd.setCursor(7,1); lcd.print("C");  
    Port0.fetch( req , measuresAcepted );
  }  
}   

void measuresPrint(const char& req){
  if (req == MedidaNivel){ //mediciones analogicas
    while ( Port0.PORT.available()==0 );
    char nivel = Port0.PORT.read();   
    lcd.setCursor(0,1); lcd.print("     "); lcd.setCursor(0,1);
    if (nivel == empty){ lcd.print( bajo ); return; }
    if (nivel == medium){ lcd.print( medio ); return; }  
    if (nivel == full){ lcd.print( alto ); return; }  
    lcd.print( falla );   return;
  }
  float medicion = Port0.PORT.parseFloat();
  lcd.setCursor(0,1); lcd.print("       "); lcd.setCursor(0,1);
  lcd.print( medicion ); 
}

void measuresAcepted(const char& req){
  measuresPrint(req);
  Sampler.lastTimer = millis(); bool hold=true;
  do{
    Sampler(  2000 ,  [&req](){  Port0.fetch( req , measuresPrint ); } );
    modo( [&hold](){hold=false;} );
  }while( hold );
  menuLcd.update();
}