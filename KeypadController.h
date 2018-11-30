/*
  Design and created by Blascarr
  LiquidMVC
  Author    : BlZGZMakerSpaceascarr
  Description: LiquidMVC.h
  version : 1.0
 
    ZGZMakerSpace invests time and resources providing this open source code like some other libraries, please
    respect the job and support open-source software.
    
    Written by Adrian for Zaragoza Maker Space
*/
    
#ifndef KEYPADCONTROLLER_H
#define KEYPADCONTROLLER_H

#include "MenuController.h"

#ifdef KEYPAD_H

	class KeypadController: public MenuController{
		public:

			typedef void ( KeypadController::*_f_ptr )();
			_f_ptr f_hold_ptr  = &KeypadController::IFHold;
			_f_ptr f_idle_ptr  = &KeypadController::IFIdle;
			_f_ptr f_release_ptr  = &KeypadController::IFRelease;
			_f_ptr f_press_ptr  = &KeypadController::IFPress;

			KeypadController( Keypad& pad ): 
				MenuController( Type::KEYPAD ), _keypad(pad) {
					
			}

			//Configuration Keypad Definition
			void init(void) override {
				//Keypad keypad = Keypad( makeKeymap(keys), _rowPins, _colPins, _rows, _cols );
				//_keypad.addEventListener( KeypadController::_keypadEvent ); // Add an event listener for this keypad
			};

			uint8_t read(void){
				char key = _keypad.getKey();
				if (key) {
					Serial.println(key);
				}
				return ;
			};

			MenuController::Event readevent(void) override{
				return MenuController::Event::NONE;
			};

			void static _keypadEvent(KeypadEvent key, Keypad pad){

				switch ( pad.getState() ){
				case PRESSED:
					if (key == '#') {
					    //digitalWrite(ledPin,!digitalRead(ledPin));
					    //ledPin_state = digitalRead(ledPin);        // Remember LED state, lit or unlit.
					}
					break;

				case RELEASED:
					if (key == '*') {
					    //digitalWrite(ledPin,ledPin_state);    // Restore LED state from before it started blinking.
					    //blink = false;
					}
					break;

				case HOLD:
					if (key == '*') {
					    //blink = true;    // Blink the LED when holding the * key.
					}
					break;
				}
			}

			//FUNCIONES DE PRUEBA QUE HABRA QUE BORRAR MAS ADELANTE
			void ReadKey(){
				Serial.println("key");
			};
			void IFPress(){
				Serial.println("Press");
			};
			void IFRelease(){
				Serial.println("Release");
			};
			void IFHold(){
				Serial.println("Hold");
			};
			void IFIdle(){
				Serial.println("Idle");
			};
			void dump(){
				Serial.println("DUMP");
			};

			private:
				Keypad& _keypad;
	};

	#endif

#endif