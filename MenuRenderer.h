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

#ifndef MENURENDERER_H
#define MENURENDERER_H

class MenuRenderer
{
  public:

    enum class Type
    {
      UNKNOWN,
      LCD,
      TFT
    };

    Type _type = MenuRenderer::Type::UNKNOWN ;
    MenuRenderer( Type type = Type::UNKNOWN ): _type( type ){

    };

    virtual void init(void){};
    virtual void update(void){};

    Type getType(void){
      return _type;
    }

};

#endif