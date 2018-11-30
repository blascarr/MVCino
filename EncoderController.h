/*
  Design and created by Blascarr
  LiquidMVC
  Author    : ZGZMakerSpace
  Description: LiquidMVC.h
  version : 1.0
 
    ZGZMakerSpace invests time and resources providing this open source code like some other libraries, please
    respect the job and support open-source software.
    
    Written by Adrian for Zaragoza Maker Space
*/
    
#ifndef ENCODERCONTROLLER_H
#define ENCODERCONTROLLER_H

#include "MenuController.h"

#define ENC_DEBOUNCEDELAY    50  // check button every x milliseconds, also debouce time
#define ENC_DOUBLECLICKTIME  600  // second click within 600ms
#define ENC_HOLDTIME 1200 // report held button after 1.2s
#define INT_MIN 1000
#define INT_MAX 1000

void f_void(){

}

class EncoderController: public MenuController
{
	public:

		typedef void ( EncoderController::*_f_ptr )();
		_f_ptr f_C_ptr  = &EncoderController::IFC;
		_f_ptr f_CCW_ptr  = &EncoderController::IFCCW;
		_f_ptr f_CW_ptr  = &EncoderController::IFCW;
		_f_ptr f_press_ptr  = &EncoderController::IFPress;

		typedef void (*_func)();

		//Para ejecutarse en bucle deben estar vinculadas a funciones externas en el momento de crear el objeto o en el setup
		_func f_CW ;
		_func f_CCW ;
		_func _f ;
		_func f_press ;

		void (*_fp)();
		void (EncoderController::*_fr)();

		//Switch and counter Evaluation
		long _counter = 0, _lastValue = 0;
		uint8_t lastEncoded = 10;
		uint8_t sum = 0, encoder_steps = 3;
		int8_t _inc = 1;
		int _minL = INT_MIN, _maxL = INT_MAX;

		//Constructor Methods (Interruption MODE or NORMAL MODE)
		#if defined(ENCODER_USE_INTERRUPTS)

			EncoderController (uint8_t pinA, uint8_t pinB, uint8_t pinSW, bool clockwise = true, bool pullup = false)
				: MenuController( Type::ENCODER_INT ),
				_pinA(pinA), _pinB(pinB), _pinSW(pinSW), _state(LOW), _lastState(LOW),
					_lastMillis(0),
					_lastDebounceTime(0) {

				pinMode(_pinA, INPUT);
				pinMode(_pinB, INPUT);

				uint8_t configType = (pullup == true) ? INPUT_PULLUP : INPUT;
				pinMode(_pinSW, configType );

				_inc = (clockwise) ? 1 : -1;
				encoder_steps = 1;

				digitalWrite(_pinSW, HIGH);
				//Definir una funcion de ejecucion por defecto cuando detecta interrupcion
				//Las interrupciones funcionan más rápido que el sistema de actualizacion de la pantalla... REVISAR ESTA PARTE

			}

			EncoderController (uint8_t pinA, uint8_t pinB, uint8_t pinSW, void (*f)(), bool clockwise = true, bool pullup = false)
				: MenuController( Type::ENCODER_INT ),
					_pinA(pinA), _pinB(pinB), _pinSW(pinSW), _state(LOW), _lastState(LOW),
					_lastMillis(0),
					_lastDebounceTime(0) {

				EncoderController(pinA, pinB, pinSW, clockwise, pullup);

				attach( f );
			}

			void attach( void (*f)() ){
				attachInterrupt( digitalPinToInterrupt(_pinA), f, CHANGE);
				attachInterrupt( digitalPinToInterrupt(_pinB), f, CHANGE);
			}

		#else
			EncoderController(uint8_t pinA, uint8_t pinB, uint8_t pinSW, bool clockwise =false , bool pullup = true )
				: MenuController( Type::ENCODER ),
				_pinA( pinA ), _pinB( pinB ), _pinSW( pinSW ),
				_state(LOW), _lastState(LOW),
				_lastMillis(0),
				_lastDebounceTime(0) {

					pinMode(_pinA, INPUT);
					pinMode(_pinB, INPUT);

					uint8_t configType = (pullup == true) ? INPUT_PULLUP : INPUT;
					pinMode(_pinSW, configType );

					_inc = (clockwise) ? 1 : -1;
					encoder_steps = 1;

					digitalWrite(_pinSW, HIGH);
			}
  		#endif
		//Configuration Encoder Definition
		void init(void) override {

		};
		void setEncoderSteps( uint8_t steps){
			encoder_steps = steps;
		}

		void setLimits( int max_limit, int min_limit, bool circular = false){
			_limited = true;
			_maxL = max_limit;
			_minL = min_limit;
			_cLimits = circular;
		}

		void nolimits( ){
			_limited = false;
		}

		int counter( ){
			return this->_lastValue;
		}

		//Reading Methods
		#if defined(ENCODER_USE_INTERRUPTS)
			uint8_t read(  ) override{
				int MSB = digitalRead(_pinA); //MSB = most significant bit
				int LSB = digitalRead(_pinB); //LSB = least significant bit
				int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
				uint8_t dir= 0;

				sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value  
				lastEncoded = encoded; //store this value for next time

				if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {_counter = _counter - _inc ; dir = 1; }
				if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {_counter = _counter + _inc ; dir = 2; }
				this->update();
				return dir;
			}
    	#else
			uint8_t read(  ) override{
				byte DialPos = (digitalRead(_pinA) << 1) | digitalRead(_pinB); uint8_t dir;
				dir = 0;
				if (DialPos == 3 && _Last_DialPos == 1) {
					_counter = _counter + _inc;
					dir = 1;
				}
				if (DialPos == 3 && _Last_DialPos == 2) {
					_counter= _counter - _inc;
					dir = 2;
					
				}
				_Last_DialPos = DialPos; 
				this->update();
				//No funciona bien la devolucion del valor
				return dir;
			} // Debounces the button and returns the state if it was just changed.
    	#endif

		MenuController::Event readevent(void) override{
			return MenuController::Event::NONE;
		};

		//Function Pointer inner Behavior
		virtual void update( ){
			this->pressed();
			this->readCounter();
		}

		virtual bool pressed( bool triggerState = LOW ) {
			_state = digitalRead(_pinSW); // Checks if the buttons has changed state      
			if (_state != _lastState) {

				_lastState = _state;
				if (_lastState == triggerState){
					(this->*f_press_ptr)();
				}
				return true;
			}
			return false;
		}

		//External Function Pointers

		void set_f_CW( void (*fCW)() ){
			f_CW = fCW;
		}

		void set_f_CCW( void (*fCCW)()  ){
			f_CCW = fCCW;
		}

		void set_f_C( void (*fC)()  ){
			_f = fC;
		}

		void set_f_press( void (*f)() ){
			f_press = f;
		}

		//Set Function Pointers on private member functions
		void set_f_rotary( void (*f)() = NULL, void (*g)() = NULL, void (*h)() = NULL ){
			this->set_f_C(f);
			this->set_f_CW(g);
			this->set_f_CCW(h);
		}

		//Execution Dynamic Function Pointers fired in read Method
		virtual long readCounter(  ){
			int value = _counter / encoder_steps;
			if(_limited){
				if (_cLimits){
					_counter = (value > _maxL) ? _minL*encoder_steps : _counter;
					_counter = (value < _minL) ? _maxL*encoder_steps : _counter;
				}else{
					_counter = (value > _maxL) ? _maxL*encoder_steps : _counter;
					_counter = (value < _minL) ? _minL*encoder_steps : _counter;
				}
			}

			if ( value != _lastValue) {
				if(_lastValue < value){
					//(this->f_CW)();	//Para ejecutarse deben estar definidas
					(this->*f_CW_ptr)();
				}else{
				  	//this->f_CCW();
					(this->*f_CCW_ptr)();
				}
				_lastValue = value;
				//this->_f();
				(this->*f_C_ptr)();
			}
			return (value);
		}

		//FUNCIONES DE PRUEBA QUE HABRA QUE BORRAR MAS ADELANTE
		void IFCW(){
			Serial.println("CW");
		};
		void IFCCW(){
			Serial.println("CCW");
		};
		void IFC(){
			Serial.println("M");
		};
		void IFPress(){
			Serial.println("Press");
			//f_CW_ptr = static_cast<void(EncoderController::*)()>(&this->dump);
			set_CW( &EncoderController::dump );
		};

		void dump(){
			Serial.println("DUMP");
		};

    protected:
		const uint8_t _pinA, _pinB, _pinSW;
		bool _state, _lastState;
		bool _cLimits = false, _limited = false ;
		byte _Last_DialPos;
		uint32_t _lastMillis;
		uint16_t _debounceDelay = ENC_DEBOUNCEDELAY;
		uint32_t _lastDebounceTime;

		//Internal Function Pointers

		void set_CW( _f_ptr _f ){
			f_CW_ptr = static_cast<void(EncoderController::*)()>( _f );
		}

		void set_CCW( _f_ptr _f  ){
			f_CCW_ptr = static_cast<void(EncoderController::*)()>( _f );
		}

		void set_C( _f_ptr _f  ){
			f_C_ptr = static_cast<void(EncoderController::*)()>( _f );
		}

		void set_press( _f_ptr _f ){
			f_press_ptr = static_cast<void(EncoderController::*)()>( _f );
		}

};


#endif