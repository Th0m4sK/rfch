#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "LittleFS.h"
#include <FileSystem.h>
#include <SerialDebug.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <secrets.h>
#include <ESP8266WiFiAP.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN       D3 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 201 // Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 1000 // Time (in milliseconds) to pause between pixels

int Gr_Start[9] = {0,6, 25, 30, 40,  49,  54,  64, 112,};
int Gr_Anzahl[9] = {6, 19, 5, 10, 9, 5, 10, 48, 89};
int segel[3] = {0, 2, 4};
int mast_oben[1] = {1};
int boot[3] = {3,5, 7};
int schrift[1] = {6};
int ring[1] = {8};
bool AUS = false;
unsigned int rxcount = 0;
unsigned long EIN_ZEIT = 0;

int state = 0;
String rxdat = "";
String RXDAT = "";
String LastIN = "";
float backlslash = 0.5;
unsigned long oldtime = 0;
bool NewData = false;
FileSystem fileSys(DEBUG);
SerialDebug DebugMain(DEBUG);
AsyncWebServer server(80);

IPAddress local_ip(192,168,42,1);
IPAddress gateway(192,168,42,254);
IPAddress netmask(255,255,255,0);


struct MData{
  String Auskuck = "#ff0000"; 
  String Segel = "#0000ff";
  String Boot = "#ff0000"; 
  String Schrift = "#ffffff"; 
  String Ring = "#0000ff";
  String Helligkeit = "100";
  
};

MData md;
MData mdOLD;

String processor(const String &var)
{ 
  return "lerr";
}

String CheckREF(String normal){
 
   return normal;
 
}
String CreateTXString(){
   String message = "";
       // ["PaSpeed","PaAccer","PaResul","PaMax","PaBacksl"]
        message += md.Auskuck + "/";
        message += md.Segel + "/";
        message += md.Boot + "/";
        message += md.Schrift + "/";
        message += md.Ring + "/";
        message += md.Helligkeit;
        return message;
}
void SERVER()
{
  // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    DebugMain.print("IndexHTML", "LOAD", DEBUG);
    
    request->send(LittleFS ,CheckREF ("/index.html"), String(), false, processor);
  });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    DebugMain.print("Style.css", "LOAD", DEBUG);
    request->send(LittleFS, "/style.css", "text/css");
  });
  // Rout to load sript.js file
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    DebugMain.print("script.js", "LOAD", DEBUG);
    request->send(LittleFS, "/script.js", "text/javascript");
  });
  
  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
    DebugMain.print("favicon.ico", "LOAD", DEBUG);
    request->send(LittleFS, "favicon.ico", "text/plain");
  });
  
  

   
  
   server.on("/GetData", HTTP_GET, [](AsyncWebServerRequest *request)
             {
               String message = CreateTXString();
               request->send_P(200, "text/plain", message.c_str());
             });
     
   server.on(
       "/SendData", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL,
       [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
       { 
         String val[20];
             for (size_t i = 0; i < len; i++)
          {
            rxdat += char(data[i]);
          }

          if (index + len == total)
          {
            
            DebugMain.print("POST DATA", rxdat, DEBUG);
            int z = 0;
            int s = 0;

            while (true)
            {
            int n = rxdat.indexOf('/', s);
            if (s-1 == rxdat.lastIndexOf('/')){
            break;
             }

            
            String va = rxdat.substring(s, n);
            DebugMain.print("DATA", va, DEBUG);
            val[z] = va;
            z++;
            s = n + 1;

            }
            rxdat = "";

            request->send(200);

                         
            
          }
        
         
         if (val[0] == "savM")
            {

           mdOLD = md;
           String mes = "";
           int z = 1;
           md.Auskuck = val[z];
           mes += val[z] + "/";
           z++;
           md.Segel = val[z];
           mes += val[z] + "/";
           z++;
           md.Boot = val[z];
           mes += val[z] + "/";
           z++;
           md.Schrift = val[z];
           mes += val[z] + "/";
           z++;
           md.Ring = val[z];
           mes += val[z] + "/";
           z++;
           md.Helligkeit = val[z];
           mes += val[z] + "/";
           z++;
          


           fileSys.WriteNewFile((char *)"paraM", (char *)mes.c_str());

          
            
         }
         else if (val[0] == "EIN"){
          unsigned long tmp1;
           
         tmp1= val[1].toInt();
         if (tmp1>0){
tmp1 = tmp1  *60* 1000;
         EIN_ZEIT =  tmp1;
         oldtime = millis();
         AUS = false;
         }
         else
         {
           AUS = true;
           EIN_ZEIT = 0;
         }

         DebugMain.print("EIN", String(EIN_ZEIT), DEBUG);
         DebugMain.print("MILLIS()", String(oldtime), DEBUG);
         

         }

         
           
              });

   // Start server
   
}





void setPixelLine(int start, int Anzahl, uint8_t r, uint8_t g, uint8_t b ){
   for (int i = start; i < start+Anzahl; i++)
  {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
}
uint8_t getValueHex(String ZAHL) {
    char Z[5];
    ZAHL.toCharArray(Z,5);
  
        if (Z[0] >= 48 & Z[0] <= 57) {
                 
            return Z[0] - 48;
        }
    if (Z[0] >= 65 & Z[0] <= 70) {
               return Z[0] - 55; 
    }
    return -1;


}
uint8_t HEXString_2_INT (String color1,byte Start) {
    uint8_t zahl = 0;
    color1.toUpperCase();
    zahl = getValueHex(color1.substring(Start, Start+1)) * 16;
  
    zahl += getValueHex(color1.substring(Start+1, Start+2));
   
 
    return zahl;
}

void setColor2Gr(int obj[],int size,String Farbe){

  int r = HEXString_2_INT(Farbe, 1);
  int g = HEXString_2_INT(Farbe, 3);
  int b = HEXString_2_INT(Farbe, 5);
 


  for ( int i = 0; i < size ; i++)
  {

    //Serial.println(size);
    //Serial.println(Gr_Start[obj[i]]);
    //Serial.println(Gr_Anzahl[obj[i]]);

    setPixelLine(Gr_Start[obj[i]], Gr_Anzahl[obj[i]], r, g, b);
 }

}
void setup() {
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.
  Serial.begin(115200);
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  fileSys.mount();

  String TXT = fileSys.ReadFile((char *)"paraM");
 if (TXT=="notExists"){
   String mes = CreateTXString();
   mes += "/";
   fileSys.WriteNewFile((char *)"paraM", (char *)mes.c_str());
   TXT = fileSys.ReadFile((char *)"paraM");
 }

int s = 0;
String val[20];
int z = 0;

val[0] = "";

while (true)
{
  int n = TXT.indexOf('/', s);
  if (s-1 == TXT.lastIndexOf('/')){
    break;
  }

  z++;
  String va = TXT.substring(s, n);
  DebugMain.print("DATA", va, DEBUG);
  val[z] = va;
  s = n + 1;

 }
 
 z  = 1;
 
 md.Auskuck = val[z];
 z++;
md.Segel = val[z];
 z++;
 md.Boot = val[z];
 z++;
md.Schrift= val[z];
 z++;
 md.Ring = val[z];
z++;
 md.Helligkeit = val[z];


 // Wifi Config
   //WiFi.begin(SSID, PASS);
  WiFi.mode(WIFI_AP);           //Only Access point
  //Start HOTspot removing password will disable security
	Serial.print("HotSpt IP:");
	Serial.print("Setting soft-AP configuration ... ");
	Serial.println(WiFi.softAPConfig(local_ip, gateway, netmask) ? "Ready" : "Failed!");


	Serial.print("Setting soft-AP ... ");
	Serial.println(	WiFi.softAP(SSID, PASS) ? "Ready" : "Failed!");

	Serial.print("Soft-AP IP address = ");

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());
  SERVER();
  server.begin();
  oldtime = millis();
  EIN_ZEIT = 60000;
}




void loop() {
  // Set all pixel colors to 'off'
  
  if (millis()-oldtime>EIN_ZEIT || AUS==true){
    AUS = true;
    EIN_ZEIT = 0;
    pixels.clear();
pixels.setBrightness(0);
pixels.show();
Serial.println("AUS");
  }
  else{
    pixels.clear();
    Serial.println("EIN");
    // The first NeoPixel in a strand is #0, second is 1, all the way up
    // to the count of pixels minus one.
    setColor2Gr(mast_oben, 1, md.Auskuck);
    setColor2Gr(segel, 3, md.Segel);
    setColor2Gr(boot, 3, md.Boot);
    setColor2Gr(schrift, 1, md.Schrift);
    setColor2Gr(ring, 1, md.Ring);
    pixels.setBrightness(int(md.Helligkeit.toInt()*2.55));

    pixels.show(); // Send the updated pixel colors to the hardware.
  }
    

  

    delay(1000);


}