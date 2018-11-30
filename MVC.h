/*
  Design and created by Blascarr
  MVC
  Author    : ZGZMakerSpace
  Description: MVC.h
  version : 1.0
 
    ZGZMakerSpace invests time and resources providing this open source code like some other libraries, please
    respect the job and support open-source software.
    
    Written by Adrian for Zaragoza Maker Space
*/

#ifndef MVC_H
#define MVC_H 

    #ifdef MEMWATCHDOG
    	#include <avr/wdt.h>

		//---------------- Dynamic MEMORY  Watchdog------------//
		#ifdef __arm__
			// should use uinstd.h to define sbrk but Due causes a conflict
			extern "C" char* sbrk(int incr);
		#else  // __ARM__
			extern char *__brkval;
		#endif  // __arm__

		int freeMemory() {
			char top;
			#ifdef __arm__
				return &top - reinterpret_cast<char*>(sbrk(0));
			#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
				return &top - __brkval;
			#else  // __arm__
				return __brkval ? &top - __brkval : &top - __malloc_heap_start;
			#endif 
		}
	#endif 

    #ifdef DEBUG
		#define  DUMP(s, v)  { Serial.print(F(s)); Serial.print(v); }
		#define  DUMPV(v)  { Serial.print(v); }
		#define  DUMPS(s)    { Serial.print(F(s));}
		#define  DUMPPRINTLN() { Serial.println();}
		#define  DUMPMEM() { Serial.println( freeMemory() ); }
	#else
		#define  DUMP(s, v)
		#define  DUMPV(v)
		#define  DUMPS(s)
		#define  DUMPPRINTLN() 
		#define  DUMPMEM()
	#endif
    
    #ifndef MENU_MAX_SIZE
		#define MENU_MAX_SIZE 20
	#endif 

	#if defined(ARDUINO) && ARDUINO >= 100
		#include "Arduino.h"
	#else
		#include "WProgram.h"
	#endif

	//#include <stdarg.h>
	#include "Vector.h"
	#include <MenuSystem/MenuSystem.h>
	#include "MenuOption.h"

	#include "EncoderController.h"
	#include "KeypadController.h"

	#include "LCDRenderer.h"
	#include "TFTRenderer.h"

	class MVC  {
		public:
			MenuRenderer& _renderer;

			typedef void ( MVC::*_f_MVC )();
				_f_MVC _f_read = &MVC::readEncoder;
				_f_MVC _f_C_MVC ;	//Puntero a funcion de cambio
				_f_MVC _f_CW_MVC = &MVC::pluspointer;
				_f_MVC _f_CCW_MVC = &MVC::minuspointer;

				_f_MVC _f_up_cursor = &MVC::update_upcursor;
				_f_MVC _f_down_cursor = &MVC::update_downcursor;
				_f_MVC _f_press_cursor = &MVC::dumpMVC;

			MVC(MenuRenderer& renderer, MenuController& controller, uint8_t firstline = 2, uint8_t secondline = 11):
					_renderer(renderer),
					_controller(controller)
					//_upline(firstline,0),	//Si la pantalla LCD es de 4 filas habria que introducir un condicional para meter 4 LiquidLines mas
					//_downline(firstline,1),
					//_upvline(secondline,0),
					//_downvline(secondline,1),
					//_sysMVC()
					//_screen(_upline, _downline, _upvline, _downvline),
					//_menu (static_cast<LCDRenderer&>(_renderer)._lcd )
					//_menu (static_cast<LCDRenderer&>(_renderer)._lcd , _screen) //Bueno. aqui ya nos hemos cargado la compatibilidad con pantallas TFT amenos que se pueda introducir una condicion en un constructor de miembros
				{
					
					if( _controller.getType() == MenuController::Type::ENCODER || _controller.getType() == MenuController::Type::ENCODER_INT){
						_f_read = &MVC::readEncoder;
					}
					#ifdef KEYPAD_H
						if( _controller.getType() == MenuController::Type::KEYPAD ){
							_f_read = &MVC::readKeypad;
						}
					#endif
					if( _renderer.getType() == MenuRenderer::Type::LCD ){
						_renderer = (LCDRenderer&) renderer;
					}
					if( _renderer.getType() == MenuRenderer::Type::TFT ){
						#ifdef _ADAFRUIT_GFX_H
							_renderer = (TFTRenderer&) renderer;
						#endif
					}

					_uptr = "Liquid";
					_dptr = "ZMS_MVC";
				};

			//----------------------SET MENU AND CONFIGURATION-------------//
			void init(){

				if( _renderer.getType() == MenuRenderer::Type::LCD ){
					LCDRenderer& rndr =  static_cast<LCDRenderer&>(_renderer);
					rndr._lcd.init(); 
					rndr._lcd.backlight();
				}else if( _renderer.getType() == MenuRenderer::Type::TFT ){
					
					#ifdef _ADAFRUIT_GFX_H

						TFTRenderer& rndr =  static_cast<TFTRenderer&>(_renderer);
						#ifdef _ADAFRUIT_ILI9341H_
							if( rndr.getTFType() == TFTRenderer::TFType::TFT_ILI9341 ){
								rndr._tft.begin();
								rndr._tft.fillScreen( rndr._tft.color565(0,255,0) );
							}
						#endif

						#ifdef _ADAFRUIT_ST7735H_
							if( rndr.getTFType() == TFTRenderer::TFType::TFT_ST7735 ){
								uint16_t tab = 1;
								rndr._tft.initR(tab);
								//rndr._tft.fillScreen( rndr._tft.color565(0,255,255) );
							}
						#endif

						#ifdef _ADAFRUIT_TFTLCD_H_
							if( rndr.getTFType() == TFTRenderer::TFType::TFT_ILI9341 ){
								rndr._tft.begin(rndr._tft.readID();
								//rndr._tft.fillScreen( rndr._tft.color565(0,255,255) );
							}
						#endif
					#endif
				}

				//_menu.set_focusSymbol(Position::LEFT, _rFocus);
				//_menu.set_focusPosition(Position::LEFT);
				//_upline.add_variable(_uptr);
				//_downline.add_variable(_dptr);
				//_upvline.add_variable(_upvalue);
				//_downvline.add_variable(_downvalue);

				//_upline.attach_function(1, f_void);
				//_downline.attach_function(1, f_void );

				//_sys.add_menu ( _menu );
				dumpMVCType();

			};
			
			void setMVCArray( _systemMVC MVC ){
				_sysMVC = &MVC;
				
				//Set current Menu initialized
				setPtrMenu(  _sysMVC->at(0)  );
				_uptr = _currentMenu->at(0)->getName();
				_dptr = _currentMenu->at(1)->getName();
				dumpMVC();
				//_sys.switch_focus(true);
				//_sys.update();
			}

			void setcurrentMenu(  _menuMVC sysMenu  ){
				_currentMenu = &sysMenu;
			}

			void setPtrMenu(  _menuMVC* sysMenu  ){
				_currentMenu = sysMenu;
			}

			//---------------------------------------------//
			//---------------DUMP MENU --------------------//
			
			void dumpMVC(){
				DUMP("MENU MODEL ", _sysMVC->size() );DUMPPRINTLN();

				//Reference to local object for read only
				_systemMVC _sys = _systemMVC ( *_sysMVC );
				for( int i = 0; i < _sys.size(); i++){
					DUMP("Menu - ",i);DUMP(" size: ", _sys[i]->size() );DUMPPRINTLN();
					_menuMVC _menu =  _menuMVC ( *_sys[i] );
					for( int j = 0; j < _menu.size() ; j++){
						
						DUMP("\tName: ", _menu[j]->getName() );
						if( _menu[j]->getType() == MenuOption::Type::INT_NUMBER ){
							//Oldvalue
							DUMP("\tVAL: ", static_cast<MenuOptionIntValue*>( _menu[j] )->getValue() );
							
							//Pointer to actual value
							int *_vp = static_cast<MenuOptionIntValue*>(_menu[j])->_value;

							DUMP("\tPTR_INT: ", int( _vp) );

						}
						DUMPPRINTLN();
					}
					DUMPPRINTLN();
				}
			}

			void dumpCurrentMenu(){
				DUMP("CURRENT MENU ", _currentMenu->size() );DUMPPRINTLN();
				for( int i = 0; i < _currentMenu->size() ; i++){
					DUMP("\tName: ", _currentMenu->at(i)->getName() );
					if( _currentMenu->at(i)->getType() == MenuOption::Type::INT_NUMBER ){
						int *_vp = static_cast<MenuOptionIntValue*>(_currentMenu->at(i))->_value;
						DUMP("\tPTR_INT: ", int( _vp) );
					}
					DUMPPRINTLN();
				}
				DUMPPRINTLN();
			}

			//---------------------------------------------//
			/*void set_screen( LiquidScreen screen ){
				//_menu->add_screen( screen );
			}*/

			void set_focus( byte* Focus ){
				
			}

			//Update values schedule in time in comparison with old values
			void update(){
				//_sys.update();
			}

			//Extension a modelos lcd de 4 columnas o TFT
			void updatevaluepointers( int pos ){
				if( _currentMenu->at(_npointer + pos)->getType() == MenuOption::Type::INT_NUMBER ){
					_upvalue = static_cast<MenuOptionIntValue*>(_currentMenu->at(_npointer+ pos) )->_value;
				}
				if( _currentMenu->at(_npointer+ pos+1)->getType() == MenuOption::Type::INT_NUMBER ){
					_downvalue = static_cast<MenuOptionIntValue*>(_currentMenu->at(_npointer + pos+1) )->_value;
				}
			}

			void pluspointer(){
				_systemMVC _sys = _systemMVC ( *_sysMVC );
				if( _npointer < _sys[ _menuID ]->size() -1){
					_npointer++;
				
					//Cambiar a current menu
					_menuMVC _menu =  _menuMVC ( *_sys[_menuID ] );
					//Cursor manipulation
					if( (_npointer == 1) ){
						(this->*_f_up_cursor)();
					}
					
					if ( (_npointer > 1) ){

						_uptr = _menu[_npointer-1]->getName();
						_dptr = _menu[_npointer]->getName();

						updatevaluepointers( -1);

					}

				}
				updatepointers( );

			}

			void minuspointer(){
				if( _npointer > 0){
					_npointer--;
				}
				
				if( (_npointer == 1) ){
					
					(this->*_f_down_cursor)();
				}

				if ( (_npointer <= 1) ){
					_uptr = _currentMenu->at (_npointer)->getName();
					_dptr = _currentMenu->at ( _npointer+1 )->getName();
					updatevaluepointers( 1 );
				}else{
					_uptr = _currentMenu->at ( _npointer-1 )->getName();
					_dptr = _currentMenu->at ( _npointer )->getName();
					updatevaluepointers( -1 );
				}
				updatepointers(  );
				
			}

			void updatepointers(  ){
				
				DUMP("POINTER: ",_npointer);
				if( _currentMenu->at ( _npointer )->getType() == MenuOption::Type::INT_NUMBER){
					DUMPS("INT_NUMBER");
					_upvalue = static_cast<MenuOptionIntValue*>(_currentMenu->at ( _npointer -1 ))->getValuePtr();
					_downvalue = static_cast<MenuOptionIntValue*>(_currentMenu->at ( _npointer ))->getValuePtr();
					//String val = "HOH";
					//val.c_str();
					set_press_cursor(&MVC::dumpMVC);
				}else{
					
				}

				if( _currentMenu->at ( _npointer )->getType() == MenuOption::Type::CALLBACK){
					DUMPS("CALLBACK");
					set_press_cursor(&MVC::executecallback);
				}
				
				if( _currentMenu->at ( _npointer )->getType() == MenuOption::Type::LINK){
					DUMPS("LINK");
					//static_cast<MenuOption*>(_currentMenu[_npointer])->getName();
					_nextMenu = static_cast<MenuOptionLink*>(_currentMenu->at (_npointer))->_link;

					//Asociar el click del encoder a esta funcion....
					//_controller.set_press();
					//menuvalue->dumplink();
					set_press_cursor(&MVC::changeMenu);
				}

			}

			//----------------------------------------------------//
			//---------------- CURSOR MANAGEMENT -----------------//
			//---------------------------------------------------//
			
			void update_upcursor(  ){
				//_sys.switch_focus(true);
				set_up_cursor(&MVC::nocursor);
				set_down_cursor(&MVC::update_downcursor);
			}

			void update_downcursor(  ){
				//_sys.switch_focus(false);
				DUMPS("DOWN CURSOR");DUMPPRINTLN();
				set_up_cursor(&MVC::update_upcursor);
				set_down_cursor(&MVC::nocursor);
			}

			void nocursor(){

			}

			//----------------------------------------------------//
			//---------------- PRESS MANAGEMENT ------------------//
			//---------------------------------------------------//
			void changeMenu(  ){
				DUMPS("CAMBIAME EL MENU");
				//_currentMenu = _nextMenu;

				//_uptr = _currentMenu[ 0 ]->getName();
				//_dptr = _currentMenu[ 1 ]->getName();
				_npointer = 0;
				//_sys.switch_focus();
				//_sys.update();
			}

			void executecallback(  ){
				DUMPS("Ejecuta funcion externa");

			}

			void counterMode(  ){
				DUMPS("Ejecuta funcion contador");
				//set_up_cursor(&MVC::update_upcursor);
				//set_down_cursor(&MVC::nocursor);
				set_press_cursor(&MVC::normalMode);
			}

			void normalMode(){
				DUMPS("Modo normal del menu");
				set_up_cursor(&MVC::update_upcursor);
				set_down_cursor(&MVC::update_downcursor);
			}
			//-----------------------------------------------//
			//-----------------------------------------------//
			void dumpMVCType(){
				
				DUMP(" Is LCD: ", _renderer.getType() == MenuRenderer::Type::LCD );DUMPPRINTLN();
				DUMP(" Is TFT: ", _renderer.getType() == MenuRenderer::Type::TFT );DUMPPRINTLN();
				DUMP(" Is Unknown: ", _renderer.getType() == MenuRenderer::Type::UNKNOWN );DUMPPRINTLN();
				DUMPPRINTLN();
				DUMP(" Is Encoder: ", _controller.getType() == MenuController::Type::ENCODER );DUMPPRINTLN();
				DUMP(" Is Encoder Interruption : ", _controller.getType() == MenuController::Type::ENCODER_INT );DUMPPRINTLN();
				DUMP(" Is Keypad: ", _controller.getType() == MenuController::Type::KEYPAD );DUMPPRINTLN();
				DUMP(" Is Unknown: ", _controller.getType() == MenuController::Type::UNKNOWN );DUMPPRINTLN();
			}

			//Declaration for read in MVC after EncoderMenuController definition
			void read();
			void readEncoder();
			void readKeypad();

			void update_menu(  ){
				//_sys.switch_focus(true);
				//set_up_cursor(&MVC::nocursor);
				//set_down_cursor(&MVC::update_downcursor);
			}

			void menuUpdate(){
				DUMPS("MENU UPDATE");
			}

			void IFC(){
				DUMPS("MENU C");
			}

			void IFCWS(){
				DUMPS("MENU CWS");
			}

			void IFCCWS(){
				DUMPS("MENU CCWS");
			}

			void IFPress(){
				DUMPS("MENU Press");

			}

			/*void add_menus( int n, ...) {
				va_list ap;
				
				DUMP("Num Args: ", n);
				va_start(ap, n);
				
				while (n--){
					//int total;
					//total += va_arg(ap, LiquidMenu );

				}
				va_end(ap);
			}*/

			/*template <class... LiquidMenu>
			void add_menus( LiquidMenu ...) {
				//DUMP("SizeOfLM: ",sizeof...(LiquidMenu));
				//DUMP("SizeOf Args: ",sizeof...(args));
				va_list ap;
				int n = sizeof...(LiquidMenu);
				DUMP("Num Args: ", n);
				va_start(ap, n);
				
				
				while (n--){
					//LiquidMenu* _menu;
					//_menu = va_arg(ap, LiquidMenu );
					//_sys.add_menu();

				}
				va_end(ap);
			}*/

			private:
				//Menu system private member class
				//LiquidSystem _sys;
				//LiquidMenu _menu;
				//LiquidScreen _screen;

				//LiquidLine _upline;
				//LiquidLine _downline;
				//LiquidLine _upvline;
				//LiquidLine _downvline;

				int _npointer = 0;	//Menu position

				//Menu dynamic variables
				char *_uptr ;
				char *_dptr ;
				char *_upchar ;
				char *_downchar ;
				int *_upvalue ;
				int *_downvalue ;

				//Vector Model for options
				int _menuID = 0;
				_systemMVC* _sysMVC;

				_menuMVC* _currentMenu;
				_menuMVC* _nextMenu;

				bool _editMode;
				
				byte _rFocus[8] = {
					0b11000,
					0b11100,
					0b00110,
					0b00011,
					0b00011,
					0b00110,
					0b11100,
					0b11000
				};
				//Controller - Encoder or Keypad
				MenuController& _controller;

			//Internal Function Pointers
			void set_CW( _f_MVC _f ){
				_f_CW_MVC = static_cast<void(MVC::*)()>( _f );
			}

			void set_CCW( _f_MVC _f  ){
				_f_CCW_MVC = static_cast<void(MVC::*)()>( _f );
			}

			void set_C( _f_MVC _f  ){
				_f_C_MVC = static_cast<void(MVC::*)()>( _f );
			}

			void set_press_cursor( _f_MVC _f  ){
				_f_press_cursor = static_cast<void(MVC::*)()>( _f );
			}
			void set_up_cursor( _f_MVC _f  ){
				_f_up_cursor = static_cast<void(MVC::*)()>( _f );
			}
			void set_down_cursor( _f_MVC _f  ){
				_f_down_cursor = static_cast<void(MVC::*)()>( _f );
			}


	};

	class EncoderMenuController: public EncoderController {
		public:

			typedef void ( MVC::*_f_MVC )();
				_f_MVC f_C_MVC  = &MVC::IFC;
				_f_MVC f_CCW_MVC  = &MVC::IFCCWS;
				_f_MVC f_CW_MVC  = &MVC::IFCWS;
				_f_MVC f_press_MVC  = &MVC::IFPress;

			EncoderMenuController(uint8_t pinA, uint8_t pinB, uint8_t pinSW, bool clockwise =false , bool pullup = true )
				: EncoderController( pinA, pinB, pinSW, clockwise, pullup ){
						
			}

			//EncoderMenuController Update-Pressed and readCounter redefinition
			//Function Pointer inner Behavior
			
			void update( ) override {
				this->pressed();
				//this->pressed( MVC );
				this->readCounter();
				//this->readCounter( MVC );
			}

			bool pressed( bool triggerState = LOW ) override {
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


			//Execution Dynamic Function Pointers fired in read Method
			long readCounter(  ) override {
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
						//(this->*f_CW_MVC)();
					}else{
					  	//this->f_CCW();
					  	(this->*f_CCW_ptr)();
						//(this->*f_CCW_MVC)();
					}
					_lastValue = value;
					//this->_f();
					(this->*f_C_ptr)();
					//(this->*f_C_MVC)();
				}
				return (value);
			}

			//Internal Function Pointers
			void set_CW( _f_MVC _f ){
				f_CW_MVC = static_cast<void(MVC::*)()>( _f );
			}

			void set_CCW( _f_MVC _f  ){
				f_CCW_MVC = static_cast<void(MVC::*)()>( _f );
			}

			void set_C( _f_MVC _f  ){
				f_C_MVC = static_cast<void(MVC::*)()>( _f );
			}

			void set_press( _f_MVC _f ){
				f_press_MVC = static_cast<void(MVC::*)()>( _f );
			}
	};

	#ifdef KEYPAD_H
		class KeypadMenuController: public KeypadController {
			public:
				KeypadMenuController(Keypad& pad):
					KeypadController(pad){

				}
		};
	#endif
	//--------   Read Controller Definition from MVC  -----------------//
	void MVC::read(){
		//Determinar si esta usando un ENCODER o UN KEYPAD.... Modificar el puntero de esta funcion internamente en funcion del controlador
		(this->*_f_read)();
	}
	#ifdef KEYPAD_H
		void MVC::readKeypad(){
			//DUMPS("IS KEYPAD");
			KeypadMenuController& ctrlr =  static_cast<KeypadMenuController&>(_controller);
			ctrlr.read();
		}
	#endif
	void MVC::readEncoder(){
		EncoderMenuController& ctrlr =  static_cast<EncoderMenuController&>(_controller);
		uint8_t encoderValue = ctrlr.read();
		
		switch(encoderValue){
			case 0:

			break;
			case 1:
				(this->*_f_CCW_MVC)();
				DUMP("POINTER CCW",_npointer);
				//MVC::_sys.update();
				
			break;
			case 2:
				(this->*_f_CW_MVC)();
				DUMP("POINTER CW",_npointer);
				//MVC::_sys.update();
				
			break;
		};
		
		if( ctrlr.pressed() ){
			(this->*_f_press_cursor)();
		}
	}

#endif