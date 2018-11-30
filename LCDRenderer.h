/*
  Design and created by Blascarr
  LiquidMVC
  Author    : ZGZMakerSpace
  Description: MVC.h
  version : 1.0
 
    ZGZMakerSpace invests time and resources providing this open source code like some other libraries, please
    respect the job and support open-source software.
    
    Written by Adrian for Zaragoza Maker Space
*/

#ifndef LCDRENDERER_H
#define LCDRENDERER_H

#include <Wire.h>  
#include "MenuRenderer.h"

	class LCDRenderer: public MenuRenderer{
		public:
			#ifdef LiquidCrystal_I2C_h

				LCDRenderer( LiquidCrystal_I2C& lcd ): 
					MenuRenderer( Type::LCD ), _lcd(lcd) {
						
				}
			#endif

			#ifdef LiquidCrystal_h
				LCDRenderer( LiquidCrystal& lcd ): 
					MenuRenderer( Type::LCD ), _lcd(lcd) {
						
				}
			#endif

			void update(){

			}
		
			#ifdef LiquidCrystal_I2C_h
				LiquidCrystal_I2C& _lcd;
			#endif

			#ifdef LiquidCrystal_h
				LiquidCrystal& _lcd;
			#endif
	};

#endif