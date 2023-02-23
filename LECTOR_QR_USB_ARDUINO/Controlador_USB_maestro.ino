#include <avr/wdt.h>
#include <Base64.h>
#include <ArduinoJson.hpp>
#include <ArduinoJson.h>
#include <hidboot.h>
#include <usbhub.h>
#include <hiduniversal.h> // no esta
#include <usbhid.h>  // no esta
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>


String cadenaSerial="";
//String cadenaDeco="eyJ2ZXIiOjEsImZlY2hhIjoiMjAyMi0xMS0wNCIsImN1aXQiOjMwNjEyOTI5NDU1LCJwdG9WdGEiOjIxMDYsInRpcG9DbXAiOjYsIm5yb0NtcCI6MTY1MzE0LCJpbXBvcnRlIjoyNTgwNjU5LCJtb25lZGEiOiJQRVMiLCJjdHoiOjEsInRpcG9Eb2NSZWMiOjk5LCJucm9Eb2NSZWMiOjAsInRpcG9Db2RBdXQiOiJFIiwiY29kQXV0Ijo3MjQ0NTk1MTk5MDgyNn0=";
bool leer=false,leer2=false,leer3=false,leer4=false;
char* base64_deco;
//unsigned char* decoded_text;
unsigned int len;

class KbdRptParser : public KeyboardReportParser
{
    void PrintKey(uint8_t mod, uint8_t key);
protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);
    void OnKeyDown  (uint8_t mod, uint8_t key);
    void OnKeyUp  (uint8_t mod, uint8_t key);
    void OnKeyPressed(uint8_t key);
    void decodificar();
};
void KbdRptParser::decodificar(){
   //len = cadenaSerial.length()+1;
   char buf[len];
   cadenaSerial.toCharArray(buf,len); //Pasa el String a Char*
   base64_deco= buf; // Guarda en el puntero
   //Serial.println(base64_deco);
   //Serial.println("abajo");
   //int stringTam = strlen(base64_deco); se harcodeo el valor 138
   int decodedLength = Base64.decodedLength(base64_deco, 138);
  char decodedString[decodedLength + 1];
  Base64.decode(decodedString, base64_deco, 138);
  //Serial.print("Cadena decodificada: ");
  //Serial.println(decodedString);

  //Examinar cadena Json Decodificada
    StaticJsonDocument<138> doc;
    DeserializationError error = deserializeJson(doc, decodedString);
    String texto = doc["nroCmp"];
    cadenaSerial="#";
    cadenaSerial += texto;
    //Serial.println(cadenaSerial); MOSTRAR POr SERIAL
    Serial2.println(cadenaSerial);  
    if (error) { return; }
  }

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
  uint8_t c = OemToAscii(mod, key);
  if((char)c== '\r'){
    if(cadenaSerial != ""){
      len = cadenaSerial.length()+1;
      //Serial.print("Tamaño de cadenaSerial: ");
      //Serial.println(len);
      //Serial.println(cadenaSerial);
      // Si sobrepasa los 25 caracteres lee el JSON de Afip
      if(len > 28){
        decodificar();
        }else{
          cadenaSerial="#"+cadenaSerial;
          //Serial.println(cadenaSerial); MOSTRAR POr SERIAL
          Serial2.println(cadenaSerial);          
        }     
      
      }
    cadenaSerial="";
    leer=false;
    leer2=false;
    leer3=false;
    leer4=false;
    }
         
    if (c)
    {
      
        OnKeyPressed(c);
    }
}

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after)
{
    MODIFIERKEYS beforeMod;
    *((uint8_t*)&beforeMod) = before;
    MODIFIERKEYS afterMod;
    *((uint8_t*)&afterMod) = after;
}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{      

}

void KbdRptParser::OnKeyPressed(uint8_t key)
{
  ///Respetar posicion de los IF
  
  /// PASO 4 Opcion:"A", comienza a grabar la cadena
  /// o PASO 5 Opcion:"B", comienza a grabar la cadena
  if(leer3){         
    cadenaSerial += (char)key;
  }

    
  /// PASO 3 Opcion:"A", si la cadena continua con '='
  /// o PASO 4 Opcion:"B", si la cadena continua con '='
  if(leer2==true){
    if ((char)key == '='){
      leer3=true;
      }else{
        leer4=false;
        leer2=false;
        leer=false;        
        }    
    }

  /// PASO 3 Opcion:"B", si la cadena continua con 'r' completa qr
  if(leer4==true){
    if ((char)key == 'r'){
      leer2=true;
      }else{
        leer4=false;
        }
    }
    
  if(leer==true){
    /// PASO 2 Opcion:"A", si la cadena continua con 'p'
    if((char)key == 'p'){
      leer2=true;            
      }
    /// PASO 2 Opcion:"B", si la cadena continua con 'q'
    else if((char)key == 'q'){
      leer4=true;
      }
     else{
        leer=false;}        
  }
    
  if((char)key == '?'){
    // PASO 1 : si la cadena trae '?' comienza a analizar
    leer=true;
    
  }  


};

USB     Usb;
USBHub       Hub(&Usb);
HIDUniversal Hid(&Usb);
KbdRptParser Prs;
void setup()
{
      wdt_disable();
    Serial.begin( 9600 );
    Serial2.begin( 9600 );
    wdt_enable(WDTO_8S);
    //Serial.println("En linea...");
    if (Usb.Init() == -1)
    {
        Serial.println("No inicio");
    }
    if (!Hid.SetReportParser(0, &Prs))
        ErrorMessage<uint8_t>(PSTR("SetReportParser"), 1  );
    
}
void loop()
{
    Usb.Task();
    wdt_reset();
    //delay(9000);

}
