#include "Arduino.h"
#include <TFT_ILI9341.h>  //https://github.com/Bodmer/TFT_ILI9341
#include <SPI.h>
#include<CountUpDownTimer.h> //https://github.com/AndrewMascolo/CountUpDownTimer
#include "Buttonkey.hpp"   //https://github.com/slavaza/SuperButton
#include "SuperButton.hpp" //https://github.com/slavaza/SuperButton

TFT_ILI9341 tft = TFT_ILI9341(); // configuration of LCD display for ILI9341
#include "alert.h"
#define BUFF_SIZE 64
CountUpDownTimer T(DOWN); // configuration of timer down 
SuperButton ChoiceButton(A1,69,600,600); // configuration of ChoiceButton(bypass_bounce, doubletime, longertime )
  bool pause,Emission,MenuActive,NonStopEmission,CustomActive,MinuteMenu,HourMenu;
  bool i;
  int hh,mm,ss,SettingTime,MenuCounter,x,hour,minutes,MenuHours,MenuMins; 
  char hhmm[10];
  char secs[5],CustomHour[5],CustomMin[5];

void drawIcon(const unsigned short* icon, int16_t x, int16_t y, int8_t width, int8_t height) {

  uint16_t  pix_buffer[BUFF_SIZE];   // Pixel buffer (16 bits per pixel)

  // Set up a window the right size to stream pixels into
  tft.setWindow(x, y, x + width - 1, y + height - 1);

  // Work out the number whole buffers to send
  uint16_t nb = ((uint16_t)height * width) / BUFF_SIZE;

  // Fill and send "nb" buffers to TFT
  for (int i = 0; i < nb; i++) {
    for (int j = 0; j < BUFF_SIZE; j++) {
      pix_buffer[j] = pgm_read_word(&icon[i * BUFF_SIZE + j]);
    }
    tft.pushColors(pix_buffer, BUFF_SIZE);
  }

  // Work out number of pixels not yet sent
  uint16_t np = ((uint16_t)height * width) % BUFF_SIZE;

  // Send any partial buffer left over
  if (np) {
    for (int i = 0; i < np; i++) pix_buffer[i] = pgm_read_word(&icon[nb * BUFF_SIZE + i]);
    tft.pushColors(pix_buffer, np);
  }
}

void menu()
{
  x = 2;
  MenuActive = true;
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawRect(0,0,320,240,TFT_WHITE);
  tft.drawString("PRESET MENU",70,5,x);
  if (MenuCounter == 1){
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.drawString("30 MIN",15,40,x);
  }
  else 
  {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("30 MIN",15,40,x);
  }
  if (MenuCounter == 2){
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.drawString("ALWAYS",15,70,x);
  }
  else {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("ALWAYS",15,70,x);
  }
  if (MenuCounter == 3){
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.drawString("CUSTOM",15,100,x);
  }
  else {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("CUSTOM",15,100,x);
  }
  if (MenuCounter == 4){
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.drawString("20 MIN",15,130,x);
  }
  else {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("20 MIN",15,130,x);
  }
  if (MenuCounter == 5){
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.drawString("25 MIN",15,160,x);
  }
  else {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("25 MIN",15,160,x);
  }
  if (MenuCounter == 6){
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.drawString("45 MIN",15,190,x);
  }
  else {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("45 MIN",15,190,x);
  }
  if (MenuCounter == 7){
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.drawString("1 HOUR",170,40,x);
  }
  else {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("1 HOUR",170,40,x);
  }
  if (MenuCounter == 8){
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.drawString("1H 15MIN",160,70,x);
  }
  else {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("1H 15MIN",160,70,x);
  }
  if (MenuCounter == 9){
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.drawString("1H 30MIM",160,100,x);
  }
  else {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("1H 30MIM",160,100,x);
  }
  if (MenuCounter == 10){
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.drawString("1H 45MIN",160,130,x);
  }
  else {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("1H 45MIN",160,130,x);
  }
  if (MenuCounter == 11){
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.drawString("2 HOURS",170,160,x);
  }
  else {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("2 HOURS",170,160,x);
  }
  if (MenuCounter == 12){
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.drawString("4 HOURS",170,190,x);
  }
  else {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("4 HOURS",170,190,x);
  }
  }

void menu_item_choosed (int MenuHours, int MenuMins) {
  tft.fillScreen(TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  MenuActive = false;
  Emission = true;
  T.SetTimer(0,MenuHours,MenuMins,0); 
  T.StartTimer(); 
 }
void time_changed() // refreshing contdown timer 
{
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.drawRect(0,0,320,240,TFT_WHITE);
  hh = T.ShowHours();
  mm = T.ShowMinutes();
  ss = T.ShowSeconds();
  sprintf (hhmm, "%02u:%02u", hh, mm);
  sprintf (secs, "%02u", ss);
  if (NonStopEmission == false) tft.drawString(hhmm,20,20,7);
  if (NonStopEmission == false) tft.drawString(secs,99,130,7); 
  if (Emission) drawIcon(alert, 40, 160, alertWidth, alertHeight);
}

void end_of_ozone_emision_display() 
{
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_DARKGREEN);
  tft.fillScreen(TFT_DARKGREEN);
  tft.drawRect(0,0,320,240,TFT_WHITE);
  tft.drawString(hhmm,20,20,7); //hours and minutes display
  tft.drawString(secs,99,130,7);//seconds display
}  
void setup() {
  pinMode(A1, INPUT_PULLUP);
  Serial.begin(9600);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  i = true;   //merker to go into green end display only once 
  pause = false; //merker to catch timer is paused or not
  Emission = false;// merker is emision is active or not ( timer is running)  
  MenuCounter = 1; //default active item in menu
  NonStopEmission = false; // merker for hiding time on timer screen
  hour = 0; // hour for custom setting
  minutes = 0; //minutes for custom setting
  CustomActive = false; // flag for custom menu
  MinuteMenu = false; // flag minutes setup
  HourMenu = false; // flag hours setup
  menu();           // showing menu

}
void loop() {
  
  T.Timer();

  if (T.TimeHasChanged()) time_changed();  // time remaining display routine
  
  if ((T.TimeCheck()) & (i == true) & (MenuActive == false) & (HourMenu == false) & (MinuteMenu == false) & (CustomActive == false)) {     // green display with end of countdown
    end_of_ozone_emision_display();
    i = false; //once input into green display at end of countdown
    Emission = false;  
  }
   switch (ChoiceButton.pressed())  // ChoiceButton
        {
          case SuperButton::Press::SINGLE: 
            {
              if (!(T.TimeCheck()) & (pause == true) & (MenuActive==false)) {//resume timer by single pressing Choice button
                T.ResumeTimer();
                pause = false;
                Emission = true; 
                break;                           
            }
              if (MenuActive) {
                CustomActive = false; // flag for custom menu
                MinuteMenu = false;
                HourMenu = false;
                ++MenuCounter;
                if (MenuCounter > 12) MenuCounter = 1;
                menu();
                break;
              }
              if (!(T.TimeCheck()) & (pause == false)& (MenuActive==false) ) { //pausing timer by short pressing ChoiceButton  
                T.PauseTimer();
                pause = true;
                Emission = false;
                tft.drawString("PAUSE",40,20,TFT_WHITE);
                break;                            
               }
              if ((MenuCounter == 3) & (HourMenu == true) & (MinuteMenu == false) & (CustomActive == false) & (MenuActive == false)){      // inc hours in custom menu
                  ++hour;
                  if (hour > 99) hour = 0;
                  tft.setTextColor(TFT_WHITE, TFT_BLACK);
                  sprintf(CustomHour,"%02u", hour);
                  tft.drawString(CustomHour,20,20,7);  
                               
                  break;
                }
              if  ((MenuCounter == 3) & (HourMenu == false) & (MinuteMenu == true) & (CustomActive == false) & (MenuActive == false)) {   // inc minutes in custom menu
                ++minutes;
                if (minutes >59) minutes = 0;
                sprintf (CustomMin,"%02u",minutes);
                tft.drawString(CustomMin,173,20,7);
                break; 
                }
            
            } 
              
          case SuperButton::Press::DOUBLE:
              {
              if ((MenuCounter == 3) & (HourMenu == true) & (MinuteMenu == false) & (CustomActive == false) & (MenuActive == false)) {
                  //double click to inc 10 minutes at once
                  hour = hour + 10;
                  if (hour > 99) hour = 0;
                  sprintf(CustomHour,"%02u", hour);
                  tft.drawString(CustomHour,20,20,7); 
                  break;
                }
              if ((MenuCounter == 3) & (HourMenu == false) & (MinuteMenu == true) & (CustomActive == false) & (MenuActive == false)) {
                    // double click to inc 10 minutes at once
                      minutes = minutes + 10;
                    if (minutes > 59) minutes = 0;
                    sprintf (CustomMin,"%02u",minutes);
                    tft.drawString(CustomMin,173,20,7);
                    break;
                    }                
                }  
                           
          case SuperButton::Press::LONGER:{
                
              if ((T.TimeCheck()) & (Emission == false) & (MenuActive == true)) { // starting emission with 20min
                  if (MenuCounter == 1) {
                   menu_item_choosed(0,30);
                   break;
                  }
              if (MenuCounter == 2) {    // starting infinitive emission - 2nd choose of menu
                  tft.fillScreen(TFT_RED);
                  tft.setTextColor(TFT_WHITE, TFT_BLUE);
                  tft.drawString("NON STOP EMISSION",20,20,2);
                  T.SetTimer(99,0,30,0); 
                  T.StartTimer(); 
                  MenuActive = false;
                  Emission = true;
                  NonStopEmission = true;
                  break;
                  }
              if  ((MenuCounter == 3)) {      // entering starting custom set time emission - 3 choose of menu
                    hour = 0;
                    minutes = 0;
                    tft.fillScreen(TFT_BLACK); 
                    tft.setTextColor(TFT_WHITE, TFT_BLACK);
                    tft.drawRect(0,0,320,240,TFT_WHITE);
                    tft.drawString("00:00",20,20,7);
                    tft.fillTriangle(85, 125, 35, 150, 135, 150, TFT_YELLOW);
                    tft.drawString("HOURS",55,160,1);
                    MenuActive = false;
                    HourMenu = true;
                    MinuteMenu = false;
                    CustomActive = false;
                   break;
                  }
              if  (MenuCounter == 4) {
                   menu_item_choosed(0,20);
                   break;
                  }
              if  (MenuCounter == 5) {
                   menu_item_choosed(0,25);
                   break;
                  }
              if  (MenuCounter == 6) {
                   menu_item_choosed(0,45);
                   break;
                  }
               if  (MenuCounter == 7) {
                   menu_item_choosed(1,0);
                   break;
                  }
              if  (MenuCounter == 8) {
                   menu_item_choosed(1,15);
                   break;
                  }
              if  (MenuCounter == 9) {
                   menu_item_choosed(1,30);
                   break;
                  }
              if  (MenuCounter == 10) {
                   menu_item_choosed(1,40);
                   break;
                  }
              if  (MenuCounter == 11) {
                   menu_item_choosed(2,0);
                   break;
                  }
              if  (MenuCounter == 12) {
                   menu_item_choosed(4,0);
                   break;
                  }
              }
              if ((MenuCounter == 3) & (HourMenu == true) & (CustomActive == false)) {    //entering minutes setup
                  tft.fillTriangle(85, 125, 35, 150, 135, 150, TFT_BLACK);
                  tft.drawRect(30,120,140,160,TFT_BLACK);
                  tft.fillTriangle(235, 125, 190, 150, 280, 150, TFT_YELLOW);
                  tft.drawString("MINUTES",195,160,1);
                  HourMenu = false;
                  MinuteMenu = true;
                  CustomActive = false;
                break; 
                }
              if ((MenuCounter == 3) & (MinuteMenu == true) & (CustomActive == false)) {     //starting custom emission 
                  
                  if ((hour == 0) & (minutes == 0)) {
                    tft.fillScreen(TFT_BLACK); 
                    tft.setTextColor(TFT_WHITE, TFT_BLACK);
                    tft.drawRect(0,0,320,240,TFT_WHITE);
                    tft.drawString("00:00",20,20,7);
                    tft.fillTriangle(85, 125, 35, 150, 135, 150, TFT_YELLOW);
                    tft.drawString("HOURS",55,160,1);
                    MenuActive = false;
                    HourMenu = true;
                    MinuteMenu = false;
                    CustomActive = false;
                    break;
                  }
                  else {
                    tft.fillTriangle(235, 125, 190, 150, 280, 150, TFT_BLACK);
                    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
                    tft.setTextSize(3);
                    tft.drawString("START EMISSION",5,160,2);
                    MenuActive = false;
                    HourMenu = false;
                    MinuteMenu = false;
                    CustomActive = false;
                    delay(1000);
                    tft.fillScreen(TFT_BLUE); 
                    T.SetTimer(0,0,hour,minutes); 
                    T.StartTimer(); 
                    Emission = true;
                    break; 
                    }
                 }
              if (!(T.TimeCheck()) & (Emission == true) & (MenuActive==false)){ //exit from running timer and get into menu
                  T.SetTimer(0,0,0,0); 
                  T.StopTimer();
                  Emission = false;
                  tft.fillScreen(TFT_BLACK);
                  NonStopEmission = false;
                  CustomActive = false;
                  menu();
                  break;
                } 
              if ((T.TimeCheck()) & (i == false) & (MenuActive == false)) {     // jump from green display end of emission into menu
                  Emission = false;
                  tft.fillScreen(TFT_BLACK);
                  NonStopEmission = false;
                  CustomActive = false;
                  i = true;
                  menu();
                  break;
                 }
              } // longer close
        }
}
