//
// NB! This is a file generated from the .4Dino file, changes will be lost
//     the next time the .4Dino file is built
//
// Define LOG_MESSAGES to a serial port to send SPE errors messages to. Do not use the same Serial port as SPE
//#define LOG_MESSAGES Serial

#define RESETLINE     30

#define DisplaySerial Serial1


#include "TimerConst.h"

#include "Picaso_Serial_4DLib.h"
#include "Picaso_LedDigitsDisplay.h"
#include "Picaso_Const4D.h"

Picaso_Serial_4DLib Display(&DisplaySerial);

// Uncomment to use ESP8266
//#define ESPRESET 17
//#include <SoftwareSerial.h>
//#define ESPserial SerialS
//SoftwareSerial SerialS(8, 9) ;
// Uncomment next 2 lines to use ESP8266 with ESP8266 library from https://github.com/itead/ITEADLIB_Arduino_WeeESP8266
//#include "ESP8266.h"
//ESP8266 wifi(SerialS,19200);

// routine to handle Serial errors
void mycallback(int ErrCode, unsigned char Errorbyte)
{
#ifdef LOG_MESSAGES
  const char *Error4DText[] = {"OK\0", "Timeout\0", "NAK\0", "Length\0", "Invalid\0"} ;
  LOG_MESSAGES.print(F("Serial 4D Library reports error ")) ;
  LOG_MESSAGES.print(Error4DText[ErrCode]) ;
  if (ErrCode == Err4D_NAK)
  {
    LOG_MESSAGES.print(F(" returned data= ")) ;
    LOG_MESSAGES.println(Errorbyte) ;
  }
  else
    LOG_MESSAGES.println(F("")) ;
  while (1) ; // you can return here, or you can loop
#else
  // Pin 13 has an LED connected on most Arduino boards. Just give it a name
#define led 13
  while (1)
  {
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(200);                // wait for a second
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(200);                // wait for a second
  }
#endif
}
// end of routine to handle Serial errors

word hndl ;
long value;
int state;
long timer;
int ledvalue;
word handle;

void setup()
{
// Ucomment to use the Serial link to the PC for debugging
//  Serial.begin(115200) ;        // serial to USB port
// Note! The next statement will stop the sketch from running until the serial monitor is started
//       If it is not present the monitor will be missing the initial writes
//    while (!Serial) ;             // wait for serial to be established

  pinMode(RESETLINE, OUTPUT);       // Display reset pin
digitalWrite(RESETLINE, 1);       // Reset Display, using shield
  delay(100);                       // wait for it to be recognised
digitalWrite(RESETLINE, 0);       // Release Display Reset, using shield
// Uncomment when using ESP8266
//  pinMode(ESPRESET, OUTPUT);        // ESP reset pin
//  digitalWrite(ESPRESET, 1);        // Reset ESP
//  delay(100);                       // wait for it t
//  digitalWrite(ESPRESET, 0);        // Release ESP reset
  delay(3000) ;                     // give display time to startup

  // now start display as Serial lines should have 'stabilised'
  DisplaySerial.begin(200000) ;     // Hardware serial to Display, same as SPE on display is set to
  Display.TimeLimit4D = 5000 ;      // 5 second timeout on all commands
  Display.Callback4D = mycallback ;

// uncomment if using ESP8266
//  ESPserial.begin(115200) ;         // assume esp set to 115200 baud, it's default setting
                                    // what we need to do is attempt to flip it to 19200
                                    // the maximum baud rate at which software serial actually works
                                    // if we run a program without resetting the ESP it will already be 19200
                                    // and hence the next command will not be understood or executed
//  ESPserial.println("AT+UART_CUR=19200,8,1,0,0\r\n") ;
//  ESPserial.end() ;
//  delay(10) ;                         // Necessary to allow for baud rate changes
//  ESPserial.begin(19200) ;            // start again at a resonable baud rate
  Display.gfx_ScreenMode(PORTRAIT) ; // change manually if orientation change
  Display.putstr("Mounting...\n");
  if (!(Display.file_Mount()))
  {
    while(!(Display.file_Mount()))
    {
      Display.putstr("Drive not mounted...");
      delay(200);
      Display.gfx_Cls();
      delay(200);
    }
  }
//hFontn = Display.file_LoadImageControl("NoName3.dnn", "NoName3.gnn", 1); // Open handle to access uSD fonts, uncomment if required and change nn to font number
//hstrings = Display.file_Open("TIMER~1.txf", 'r') ;                            // Open handle to access uSD strings, uncomment if required
  hndl = Display.file_LoadImageControl("TIMER~1.dat", "TIMER~1.gci", 1);
  // put your setup code here, to run once:

  Display.img_Show(hndl, iLeddigits1);  // Leddigits1 show all digits at 0, only do this once

  Display.img_ClearAttributes(hndl, iWinbutton1, I_TOUCH_DISABLE); // Winbutton1 set to enable touch, only need to do this once
  Display.img_Show(hndl, iWinbutton1);  // Winbutton1 show button, only do this once
  Display.img_ClearAttributes(hndl, iWinbutton2, I_TOUCH_DISABLE); // Winbutton2 set to enable touch, only need to do this once
  Display.img_Show(hndl, iWinbutton2);  // Winbutton2 show button, only do this once
  Display.img_ClearAttributes(hndl, iWinbutton3, I_TOUCH_DISABLE); // Winbutton3 set to enable touch, only need to do this once
  Display.img_Show(hndl, iWinbutton3);  // Winbutton3 show button, only do this once
  Display.touch_Set(TOUCH_ENABLE);

} // end Setup **do not alter, remove or duplicate this line**

void loop()
{
  // put your main code here, to run repeatedly:

  Display.img_SetWord(hndl, iWinbutton1, IMAGE_INDEX, 0); // Winbutton1 where state is 0 for up and 1 for down
  Display.img_Show(hndl,iWinbutton1) ;  // Winbutton1

  Display.img_SetWord(hndl, iWinbutton2, IMAGE_INDEX, 0); // Winbutton2 where state is 0 for up and 1 for down
  Display.img_Show(hndl,iWinbutton2) ;  // Winbutton2

  Display.img_SetWord(hndl, iWinbutton3, IMAGE_INDEX, 0); // Winbutton3 where state is 0 for up and 1 for down
  Display.img_Show(hndl,iWinbutton3) ;  // Winbutton3

  state = Display.touch_Get(TOUCH_STATUS);
  int n = Display.img_Touched(hndl, -1);
  //int n2 = Display.img_Touched(hndl, iWinbutton2);
  //int n3 = Display.img_Touched(hndl, iWinbutton3);

  //INCREASE BUTTON
if (state == TOUCH_PRESSED && n == iWinbutton1) {
Display.img_SetWord(hndl, iWinbutton1, IMAGE_INDEX, 1); // button_increase where state is 0 for up and 1 for down
Display.img_Show(hndl,iWinbutton1) ;
if(value < 99){
value++;
}
   LedDigitsDisplay(Display, hndl, value, iLeddigits1+1, 67, 2, 1, 43, 0) ;  // Leddigits1
}
  //DECREASE BUTTON
if (state == TOUCH_PRESSED && n == iWinbutton2) {
Display.img_SetWord(hndl, iWinbutton2, IMAGE_INDEX, 1); // button_increase where state is 0 for up and 1 for down
Display.img_Show(hndl,iWinbutton2) ;
if(value > 0){
value--;
}
   LedDigitsDisplay(Display, hndl, value, iLeddigits1+1, 67, 2, 1, 43, 0) ;  // Leddigits1
}
   //START BUTTON
if (state == TOUCH_PRESSED && n == iWinbutton3) {
Display.img_SetWord(hndl, iWinbutton3, IMAGE_INDEX, 1); // button_increase where state is 0 for up and 1 for down
Display.img_Show(hndl,iWinbutton3) ;
//i would like to start the countdown if i press this button;


handle = Display.file_Open("SAMPLE.csv",'w');
delay (2000);
Display.file_Write(11,"READING DATA",value);
Display.file_Close(handle);

ledvalue = value;
 LedDigitsDisplay(Display, hndl, value, iLeddigits1+1, 67, 2, 1, 43, 0) ;  // Leddigits1
}
  if (state == TOUCH_RELEASED){
Display.img_SetWord(hndl, iWinbutton1, IMAGE_INDEX, 0); // button_increase where state is 0 for up and 1 for down
Display.img_Show(hndl,iWinbutton1) ;
Display.img_SetWord(hndl, iWinbutton2, IMAGE_INDEX, 0); // button_increase where state is 0 for up and 1 for down
Display.img_Show(hndl,iWinbutton2) ;
Display.img_SetWord(hndl, iWinbutton3, IMAGE_INDEX, 0); // button_increase where state is 0 for up and 1 for down
Display.img_Show(hndl,iWinbutton3) ;
}
}
