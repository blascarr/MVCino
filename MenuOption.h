/*
  Design and created by Victor
  LiquidMVC
  Author    : ZGZMakerSpace
  Description: LiquidMVC.h
  version : 1.0
 
    ZGZMakerSpace invests time and resources providing this open source code like some other libraries, please
    respect the job and support open-source software.
    
    Written by Victor for Zaragoza Maker Space
*/

#ifndef MENUOPTION_H
#define MENUOPTION_H  

#include <limits.h>

typedef void (*CallbackFunction)(void);

class MenuOption{
  public:
   
    enum class Type
    {
      NONE,
      INT_NUMBER,
      CALLBACK,
      LINK,
      TIMER
    };

    MenuOption(char* name, Type type = Type::NONE):
      _name(name),
      _type(type){
    }    

    char* getName(void){
      return _name;
    }

    Type getType(void){
      return _type;
    }
    
    String getTypeName(void){
      switch(_type)
      {
        case Type::INT_NUMBER:
          return "INT_VALUE";
          break;
                
        case Type::CALLBACK:
          return "CALLBACK";
          break;
                
        case Type::LINK:
          return "LINK";
          break;
          
        case Type::NONE:
        default:
          return "NONE";
          break;      
      }
    }

  private:
    //const String  _name;
    const char*  _name;
    const Type _type;
};

//--------------------------------------------------------//
//-------------Definition Type for MenuMVC----------------//
//--------------------------------------------------------//
typedef Vector<MenuOption* > _menuMVC;
typedef Vector<_menuMVC* > _systemMVC;

//--------------------------------------------------------//
//--------------Option Models for Menu--------------------//
//--------------------------------------------------------//

class MenuOptionIntValue: public MenuOption{
  public:

    int _minValue;
    int _maxValue;
    int& _value;
    MenuOptionIntValue(char* name, int &value, int min = INT_MIN, int max = INT_MAX):
      MenuOption(name, Type::INT_NUMBER),
      _value(value),
      _minValue(min),
      _maxValue(max){
        _v = value;
    }

    int* getValuePtr(void){
      return _value;
    }

    int getValue(void){
      return _v;
    }
    void dumpvalue(){
      Serial.println("DUMP INT VALUES");
      Serial.print("Intern VAL: ");
      Serial.println( _value );
      Serial.print("\tReal VAL: ");
      Serial.println(_v);
      Serial.print("MIN VALUE ");
      Serial.println(_minValue);
      Serial.print("MAX VALUE ");
      Serial.println(_maxValue);
      Serial.println();
    }

    private:
      
      int _v;
      
};

class MenuOptionAction: public MenuOption{
  public:
    MenuOptionAction(char* name, CallbackFunction function):
    MenuOption(name, Type::CALLBACK),
    _function(function){
    }

    void ExecuteCallback(void){
      if(_function != NULL)
      {
        _function();
      }
    }

  private:
    CallbackFunction _function;
};


// TODO: implement behaviour
class MenuOptionLink: public MenuOption
{
  public:
    _menuMVC* _link;
    MenuOptionLink(char* name):
    MenuOption(name, Type::LINK)
    {
      
    }  

    MenuOptionLink(char* name, _menuMVC lnk ):
    MenuOption(name, Type::LINK), _link( &lnk )
    {
      
    }  
    void dumplink(){
        Serial.println("Hola, soy un link a otro submenu");
      }

    private:
      

};


#endif
