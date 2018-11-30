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
    
#ifndef MENUCONTROLLER_H
#define MENUCONTROLLER_H

class MenuController
{
  public:
    
    enum class Event
    {
      NONE,
      SELECT,
      PREV,
      NEXT
    };

    enum class Type
    {
      UNKNOWN,
      ENCODER,
      ENCODER_INT,
      KEYPAD
    };

    Type _type = MenuController::Type::UNKNOWN ;
    MenuController( Type type = Type::UNKNOWN ): _type( type ){

    };

    virtual void init(void){};
    virtual uint8_t read(void){};
    virtual MenuController::Event readevent(void){ return MenuController::Event::NONE; };

    Type getType(void){
      return _type;
    }

};

#endif
