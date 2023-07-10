#include "Menu.h"
#include "doorHandler.h"
#include "manualHandler.h"
#include "processHandler.h"


//botones
Button abajo(down);
Button modo(mode);
bool manual = false;

#include <Arduino.h>

//pantalla de la puerta
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
//pantalla de abrir/cerrar
LiquidLine AC(0, 0, "Puerta:");
LiquidLine Abrir(0, 1, abrir);
LiquidLine Cerrar(0, 1, cerrar);
LiquidScreen ACscreen(AC,Abrir,Cerrar);
//pantalladelas mediciones
LiquidLine MedicionesTitle(0,0, "Mediciones:");
LiquidLine TempInterior(0,1, tempInt);
LiquidLine PresionInterior(0,1, presInt);
LiquidLine PresionExterior(0,1, presExt);
LiquidLine Nivel(0,1, nivel);
LiquidLine Next(0,1, "delante");
LiquidLine Back(0,1, "atras");
LiquidScreen Mediciones(MedicionesTitle,TempInterior,PresionExterior,PresionInterior);
// //pantallade procesos
LiquidLine ProcesosTitle(0,0, "Procesos:");
LiquidLine Proceso1(0,1, "Proceso 121C");
LiquidLine Proceso2(0,1, "Proceso 134C");
LiquidLine Proceso3(0,1, "Prueba");
LiquidLine Generador(0,1, "Generador");
LiquidScreen Procesos(ProcesosTitle);
// //pantalla del modo manual
LiquidLine ManualTitle(0,0, "Manual:");
LiquidLine Bomba(0,1, "Bomba");
LiquidLine Resistencias(0,1, "Resistencias");
LiquidLine Calentar(0,1, "V. cmr. int.");
LiquidLine ESL(0,1, "V. Esc. lnto.");
LiquidLine ESR(0,1, "V. Esc. rpd.");
LiquidLine SEC(0,1, "V. succ.");
LiquidLine ENA(0,1, "V. Ent. Aire");
LiquidLine Buzz(0,1, "Alarma");
LiquidScreen Manual(ManualTitle,Bomba,Resistencias,Calentar);
//menu
LiquidMenu menuLcd(lcd,ACscreen,Mediciones);


//puerto

Fetch Port0( Serial, 
  [](const char& code){ //Imprime las banderas de error ejemp... (E0, E4,... etc)
    lcd.clear();
    char nbits = 0; 
    for (char i = 0; i<8; i++){
      if( bitRead(code,i) == 1 ){    
        nbits++;
        if (nbits == 5) lcd.setCursor(0,1);
        else if (nbits != 1)  lcd.print(", "); 
        lcd.write('E'); lcd.write(i+'0');
      }
    }
  },
  [](const char& req){
    lcd.clear(); lcd.print("TimeOut req: "); lcd.write(req); 
  }
);

auto setScreen = [](){ menuLcd.set_focusedLine(1); menuLcd.update(); };
void menuSetup(){
    //inicializacion del modo manual o automatico
    unsigned long Timer = millis();
    lcd.begin(16, 2);
    Port0.PORT.begin(4800);  Port0.PORT.setTimeout(200);
    while ( millis()-Timer < 3000 ) modo( [](){ manual = true; } );
    //conexion con autoclave
    if (manual){ Port0.fetch('M',[](const char&){ lcd.print("Manual"); delay(2000); }); }
    else Port0.fetch('A');
    // //wrapers para usarse con el menu
    Next.attach_function(1, [](){menuLcd.next_screen(); setScreen();} );
    Back.attach_function(1,  [](){menuLcd.previous_screen(); setScreen();}); 

    Abrir.attach_function(1, [](){ requestDoor(abrirPuerta); } );
    Cerrar.attach_function(1, [](){ requestDoor(cerrarPuerta); } );
    ACscreen.add_line(Next);
    ACscreen.add_line(Back);    
    ACscreen.set_displayLineCount(2);   

    Mediciones.add_line(Nivel);
    Mediciones.add_line(Next);
    Mediciones.add_line(Back);
    PresionInterior.attach_function(1, [](){requestMeasures(MedidaP_In);});
    PresionExterior.attach_function(1, [](){requestMeasures(MedidaP_Ex);} ); 
    TempInterior.attach_function(1, [](){requestMeasures(MedidaTemp);} );
    Nivel.attach_function(1, [](){requestMeasures(MedidaNivel);} );
    Mediciones.set_displayLineCount(2);
    if (manual){
      Manual.add_line(ESL);
      Manual.add_line(ESR);
      Manual.add_line(SEC);
      Manual.add_line(ENA);
      Manual.add_line(Buzz);
      Manual.add_line(Next);   
      Manual.add_line(Back);
      //funciones
      Buzz.attach_function(1, [](){ Port0.fetch(buzzerIO); } );
      Resistencias.attach_function(1, [](){requestManual(ResistenciasManual);} );
      Bomba.attach_function(1, [](){requestManual(BombaManual);} );
      Calentar.attach_function(1, [](){requestManual(CalentManual);} );
      ESL.attach_function(1, [](){requestManual(EscapeLentoManual);} );
      ESR.attach_function(1, [](){requestManual(EscapeRapidoManual);} );
      SEC.attach_function(1, [](){requestManual(SuccionManual);} );
      ENA.attach_function(1, [](){requestManual(AireManual);} );
      //setting
      Manual.set_displayLineCount(2);
      menuLcd.add_screen(Manual);    

      Procesos.add_line(Generador);
      Procesos.add_line(Proceso3);
      Proceso3.attach_function(1, [](){requestProcess(ProcessCorto);} );
      Generador.attach_function( 1,[](){requestManual(generadorIO);} );
    }
    else{
      Procesos.add_line(Proceso1);
      Procesos.add_line(Proceso2);      
      //funciones
      Proceso1.attach_function(1, [](){requestProcess(Process121);} );
      Proceso2.attach_function(1, [](){requestProcess(Process134);} );
    }
    Procesos.add_line(Next);   
    Procesos.add_line(Back);
    Procesos.set_displayLineCount(2);
    menuLcd.add_screen(Procesos);
    menuLcd.set_focusedLine(1);   
    menuLcd.update(); 
}

void Navega(uint16_t dly){
  Sampler.lastTimer = millis();
  abajo( [](){ menuLcd.switch_focus(true); }, [dly](){
    Sampler( dly , [](){ menuLcd.switch_focus(true); } );
  });
  modo( [](){menuLcd.call_function(1);} );
}
