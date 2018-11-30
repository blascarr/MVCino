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

#ifndef TFTRENDERER_H
#define TFTRENDERER_H

#include "MenuRenderer.h"

	#ifdef _ADAFRUIT_ILI9341H_
		//PinOut for Arduino UNO
		#define TFT_CS   7 		//Chip Select
		#define TFT_DC   6  	//Data/Command 
	 	#define TFT_SD_CS 8 	//SD Pin

		#define BLACK   0x0001
		#define BLUE    0x001F
		#define RED     0xF800
		#define GREEN   0x07E0
		#define CYAN    0x07FF
		#define MAGENTA 0xF81F
		#define YELLOW  0xFFE0
		#define ROSE 	0xF81F
		#define WHITE   0xFFFF
	#endif	

	#ifdef _ADAFRUIT_TFTLCD_H_

		//PinOut for Arduino UNO
		#define LCD_CS A3 // Chip Select goes to Analog 3
		#define LCD_CD A2 // Command/Data goes to Analog 2
		#define LCD_WR A1 // LCD Write goes to Analog 1
		#define LCD_RD A0 // LCD Read goes to Analog 0

		#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin
		#define TFT_SD_CS 10
		
		#define BLACK   0x0000
		#define BLUE    0x001F
		#define RED     0xF800
		#define GREEN   0x07E0
		#define CYAN    0x07FF
		#define MAGENTA 0xF81F
		#define YELLOW  0xFFE0
		#define ROSE 	0xF81F
		#define WHITE   0xFFFF
	#endif

    #ifdef _ADAFRUIT_ST7735H_
		#define BLACK 	0x000001
		#define BLUE 	0x0000FF
		#define RED     0xF800
		#define GREEN   0x07E0
		#define CYAN    0x07FF
		#define MAGENTA 0xF81F
		#define WHITE 	0xFFFFFF
		#define YELLOW 	0x00FF00
		#define ROSE 	0x99AA66
		#define WHITE 	0xFFFFFF

	#endif

	#ifdef _ADAFRUIT_GFX_H
		class TFTRenderer: public MenuRenderer{
			public:

				enum class TFType
					{
						UNKNOWN,
						TFT_ST7735,
						TFT_ILI9341,
						TFT_SPFD548,
						TFT_PDQST7735
				};

				TFType _tftype = TFType::UNKNOWN ;

				#ifdef _ADAFRUIT_ST7735H_
					TFTRenderer( Adafruit_ST7735& tft ): 
						MenuRenderer( Type::TFT ), _tft(tft), _tftype(TFType::TFT_ST7735) {
							
					}
				#endif

				#ifdef _PDQ_ST7735H_
					TFTRenderer( PDQ_ST7735& tft ): 
						MenuRenderer( Type::TFT ), _tft(tft), _tftype(TFType::TFT_PDQST7735) {
							
					}
				#endif

				#ifdef _ADAFRUIT_ILI9341H_
					TFTRenderer( Adafruit_ILI9341& tft ): 
						MenuRenderer( Type::TFT ), _tft(tft), _tftype(TFType::TFT_ILI9341) {
							
					}
				#endif

				#ifdef _ADAFRUIT_TFTLCD_H_
					TFTRenderer( Adafruit_TFTLCD& tft ): 
						MenuRenderer( Type::TFT ), _tft(tft), _tftype(TFType::TFT_SPFD548) {
							
					}
				#endif

			
				#ifdef _ADAFRUIT_ST7735H_ 
					Adafruit_ST7735& _tft;
				#endif

				#ifdef _PDQ_ST7735H_
					PDQ_ST7735& _tft;
				#endif

				#ifdef _ADAFRUIT_ILI9341H_
					Adafruit_ILI9341& _tft;
				#endif

				#ifdef _ADAFRUIT_TFTLCD_H_
					Adafruit_TFTLCD& _tft;
				#endif

				TFType getTFType(void){
					return _tftype;
				}
		};
	#endif

#endif