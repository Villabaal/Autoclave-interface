
#include <Arduino.h>

class Button {
public:
	Button (uint8_t pin, bool pullup = true, uint16_t debounceDelay = 50)
		: _pin(pin), _state(HIGH), _lastState(HIGH),
		  _debounceDelay(debounceDelay),_lastDebounceTime(0) {
		if (pullup == true) pinMode(_pin, INPUT_PULLUP); 
		else pinMode(_pin, INPUT);
	}
	//callbakc de cada evento, (funciona dentro de un ciclo infinito)
	void operator()(  const auto& onPress, const auto& onHold, const auto& onRelease ) 
		{ debounce( onPress ); while(!_state){ onHold(); debounce( onRelease,true ); } }		
	void operator()( const auto& onPress, const auto& onHold) {
		(*this)( onPress, onHold, [](){}); 
	}
	void operator()( const auto& onPress ) { debounce( onPress ); }

private:
	const uint8_t _pin;
	bool _state,_lastState;  //presente y ultimo estado filtrado
	uint16_t _debounceDelay; uint32_t _lastDebounceTime;
 	// filtra el rebote y llama la funcion si se detecta un flanco en el boton
	// especificado por _trigger.
	void debounce( const auto& onEdge, bool edgeDirection = false ) {
		bool reading = digitalRead(_pin);
		// checa si el boton acaba de cambiar de estado
		if (reading != _lastState) _lastDebounceTime = millis();
		//debounce y detecta los flancos
		if ( ((millis() - _lastDebounceTime) > _debounceDelay) && (reading != _state)) {
			_state = reading; if(reading == edgeDirection) onEdge(); return;
		}
		_lastState = reading;
	}

};