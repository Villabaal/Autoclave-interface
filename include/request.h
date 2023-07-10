#include <Arduino.h>


//funciona dentro de un ciclo infinito
struct millisSampler{
    uint32_t lastTimer;
    void operator()(uint16_t dly, const auto& onEnd){
      if ( (millis() - lastTimer) >= dly ){ onEnd(); lastTimer = millis(); }
    }
};

static millisSampler Sampler;

class Fetch{
  void (*defaultErrorFuncion) ( const char& ); void (*defaultTimeOutFunc) ( const char& );
  void unsuccesHandler( const auto& handle ){ while ( PORT.available()==0 ); handle(  PORT.read() );  }

public:
  HardwareSerial& PORT;
  uint32_t timeout = 3000;  //200ms de timeout
  Fetch( HardwareSerial& port, const auto& errorFuncion, const auto& timeoutFuncion )
    :defaultErrorFuncion(errorFuncion),defaultTimeOutFunc(timeoutFuncion),PORT(port) {}
  void fetch( const char& req , const auto& success, const auto& denied , const auto& error ){
    PORT.write( req );  Sampler.lastTimer = millis();  
    while ( PORT.available()==0 ){
      Sampler( timeout , [this,&req](){ this->defaultTimeOutFunc(req); while(true); });
    }
    char res = PORT.read();
    if(res == 'k'){ success(req); return; }
    if(res == 'n'){ unsuccesHandler(denied); return; }
    if(res == 'e'){ unsuccesHandler(error); while(true);}    
  }  
  void fetch( const char& req , const auto& success, const auto& denied ){
    fetch( req, success, denied , defaultErrorFuncion );
  }    
  void fetch( const char& req , const auto& success){ fetch( req, success, [](const char&){} ); }
  void fetch( const char& req ){ fetch( req, [](const char&){} ); }

};



