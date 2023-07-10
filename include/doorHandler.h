#include <Menu.h>
// Wraper para los metodos de abrir/cerrar y request
// comunica con el cerebroprincipal para abrir o cerrar puertas

void requestDoor(const char&& req){
  auto acepted = [](const char& req){ 
    lcd.setCursor(9,1);
    if ( req == abrirPuerta) lcd.print( abrir );
    else if ( req == cerrarPuerta ) lcd.print( cerrar );
    modo( [](){} ,[](){} ,[](){
      Port0.fetch( detenerPuerta );  lcd.setCursor(9,1); lcd.print("      ");      
    }); 
  };
  auto denied = [](const char&){
    lcd.setCursor(9,1); lcd.print("No");
    delay(2000);
    lcd.setCursor(9,1); lcd.print("  ");   
  };
  Port0.fetch(req, acepted , denied );
}
