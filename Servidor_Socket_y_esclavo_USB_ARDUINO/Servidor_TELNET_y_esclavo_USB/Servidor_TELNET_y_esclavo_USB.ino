#include <avr/wdt.h>
#include <SoftwareSerial.h>
#include <Ethernet.h>
#include <SPI.h>
#include <SD.h>

byte mac[] =
{
    0xFC, 0xA6, 0x21, 0x0E, 0xBB, 0xB5
};


IPAddress ip(192, 168, 1, 101);
IPAddress myDns(8, 8, 8, 8);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
EthernetServer server(999);

SoftwareSerial mySerial1(9, 7); // Define rx/tx pins (rxpin, txpin)

//PARA COMUNICAciON TCP/IP

bool entradaCompleta=false;
int barreraSalida1 = 6;

//PARA GRABAR SD
File myFile;
bool saltar=false;
char inChar;
String cadenaTCP="";
String cadenaSerial="", cadenaBuffer="";

void setup()
{
    wdt_disable();
    Serial.begin(9600);
    mySerial1.begin(9600);
    Ethernet.begin(mac, ip, myDns, gateway, subnet);
    server.begin();
    pinMode(barreraSalida1, OUTPUT);
    digitalWrite (barreraSalida1, LOW);
    if (!SD.begin(4))
    {
      server.println("Error en SD!");
      return;
    }
    wdt_enable(WDTO_8S);
    //Serial.println("Inicializando SD");
    //leerDatos();
}

void loop()
{
    serialEvento2();
    if(entradaCompleta)
    {

        analizarDatos();
        
        
        entradaCompleta=false;
        cadenaSerial="";
    }
    comunicacionTCP();
    wdt_reset();
    cadenaSerial="";
    cadenaBuffer="";


}

//Funcion para intercambio via SERIAL
void serialEvento2()
{
    if(mySerial1.available())
    {
      char c = mySerial1.read();
                if(c == '#'){
                   //Obtener bytes de entrada
                   cadenaSerial = mySerial1.readString();
                   entradaCompleta= true;
                  }else{
                    mySerial1.flush();
                    }
                  

    }
}



// FUNCIONES Abrir/Escribir/Modificar SD
void analizarDatos()
{
    myFile = SD.open("dat.csv", O_READ | O_WRITE);
    if (myFile)
    {
        while (myFile.available())
        {
            byte dato=myFile.read();
            inChar =(char)dato;
            Serial.write(dato);
            if(inChar == '*')
            {
                cadenaSerial.trim();
                cadenaBuffer.trim();
                if(cadenaSerial == cadenaBuffer)
                {
                    char estado[1];// crea un buffer para almacenar el siguiente char
                    myFile.read(estado,1); //leer (al leer se mueve el puntero)                    
                    //Serial.print(" VALROR DE ESTADOOOOOOOOOOOOOO ::::::::: ");
                    //Serial.println(estado);
                    if(estado[0] == 'T'){
                      // ABRIR BARRERA AQUI !!!!!!!
                      abrirBarreraS1();
                      Serial.println("\nABRIENDO BARRERA");
                      myFile.seek(myFile.position()-1); //Retorna el puntero a la posicion anterior                      
                      myFile.print("F;"); //Grabar
                      myFile.seek(myFile.position()-1);
                      break;                   
                      }
                     if(estado[0] == 'P'){
                      // ABRIR BARRERA AQUI !!!!!!!
                      abrirBarreraS1();
                      Serial.println("\nABRIENDO BARRERA");
                      break;                   
                      }
                    myFile.seek(myFile.position()-1); //Retorna el puntero a la posicion anterior                     
                    saltar=true; //PARA que no grabe los ultimos caracteres de la cadena

                }
            }
            if(saltar==false)
            {
                cadenaBuffer += inChar;
            }
            if (inChar == '\n')
            {
                saltar=false;
                cadenaBuffer="";
            }
        }
        myFile.close();
    }
}


void guardarDatos()
{
    myFile = SD.open("dat.csv", FILE_WRITE);
    if (myFile)
    {
        String cadenaTCP2=cadenaTCP;        
        myFile.println(cadenaTCP2);
        myFile.close();
    }
}


void eliminarArchivo()
{
    if (SD.exists("dat.csv"))
    {
        SD.remove("dat.csv");
        //saltar=0;
    }
}

/*
void leerDatos()
{
    myFile = SD.open("dat.csv", O_READ | O_WRITE);
    if (myFile)
    {
        while (myFile.available())
        {
            byte dato=myFile.read();
            inChar =(char)dato;
            Serial.write(dato);
        }
        myFile.close();
    }
    else
    {
        server.println("Error4 al LeerDatos de dat.csv");
    }
}
*/


// Funcion para abrir relee
void abrirBarreraS1(){
      digitalWrite(barreraSalida1, HIGH);
      delay (3000);
      digitalWrite (barreraSalida1, LOW);
  }



// Funcion para leer TCP/IP
void comunicacionTCP()
{
    cadenaTCP="";
    EthernetClient client = server.available();
    
    if (client)
    {
      server.println("CONECTADO! ");        
        while (client.connected()) {
              
              if (client.available())
              {
                char c = client.read();
                if(c == '~'){
                  abrirBarreraS1();
                  }
                if(c == '#'){
                  cadenaTCP += client.readString();
                  }
                if(c == '*'){
                  File pfile;
                  pfile = SD.open("dat.csv");
                  if (pfile) {
                    //server.println("Enviando Datos desde SD... ");
                    String enviarCadena="";
                    while (pfile.available()){
                      byte dato=pfile.read();
                      char carac =(char)dato;
                      enviarCadena += carac;
                      if(carac == ';'){
                        server.println(enviarCadena);
                        enviarCadena="";
                        }
                      
                     }
                     pfile.close();                     
                  } 
                 //server.println("\n FIN de lectura SD... ");         
            }
               if(c == '$'){
                eliminarArchivo();
                server.println(" \n Archivo Eliminado ...");
                break;                           
            }           
            break;         
        }
        

     }
        client.flush();
        client.stop();
        cadenaTCP.trim();       
        if(cadenaTCP != ""){
          guardarDatos();
          server.println(cadenaTCP);
          cadenaTCP="";
          }
    }
}
