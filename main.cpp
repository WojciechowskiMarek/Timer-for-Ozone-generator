#include "Arduino.h"
#include <TFT_ILI9341.h> 
#include <SPI.h>
#include<CountUpDownTimer.h>
#include "Buttonkey.hpp"
#include "SuperButton.hpp"

TFT_ILI9341 tft = TFT_ILI9341(); // configuration of LCD display for ILI9341

CountUpDownTimer T(DOWN); // configuration of timer down 
SuperButton ChoiceButton(A1,69,400,400); // configuration of ChoiceButton(bypass_bounce, doubletime, longertime )
  bool pause,Emission;
  bool i;
  int hh,mm,ss,SettingTime; 
  char hhmm[10];
  char secs[5];

void time_changed() 
{
  hh = T.ShowHours();
  mm = T.ShowMinutes();
  ss = T.ShowSeconds();
  sprintf (hhmm, "%02u:%02u", hh, mm);
  sprintf (secs, "%02u", ss);
  tft.drawString(hhmm,20,20,7);
  tft.drawString(secs,99,130,7); 
}

void end_of_ozone_emision_display() 
{
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_DARKGREEN);
  tft.fillScreen(TFT_DARKGREEN);
  tft.drawString(hhmm,20,20,7); //hours and minutes display
  tft.drawString(secs,99,130,7);//seconds display
}
   
void setup() {
  pinMode(A1, INPUT_PULLUP);
  Serial.begin(9600);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLUE);
  T.SetTimer(0,0,0,5); // initial value for timer, still have'nt implement + - buttons
  T.StartTimer();
  i = true;   //merker to go into green end display only once 
  pause = false; //merker to catch timer is paused or not
  SettingTime = 0; // default window for hours setting
  Emission = false; // merker is emision is active or not ( timer is running)
   
}
void loop() {

  T.Timer();
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  if (T.TimeHasChanged()) time_changed();  // time remaining display routine
  if ((T.TimeCheck()) & (i == true)) {     // green display with end of countdown
    end_of_ozone_emision_display();
    i = false; 
    Emission = false;                            //once input into green display at end of countdown
  }
   switch (ChoiceButton.pressed())  // ChoiceButton
        {
          case SuperButton::Press::SINGLE: 
            {
              if (!(T.TimeCheck()) & (pause == true)) {//resume timer by single pressing Choice button
                T.ResumeTimer();
                pause = false; 
                break;                           
            }
            {
              if (!(T.TimeCheck()) & (pause == false)) { //pausing timer by short pressing ChoiceButton  
                T.PauseTimer();
                pause = true;
                break;                            
            }
                }
              if ((T.TimeCheck()) & (SettingTime == 0)) { //enter into setup mode for timer times
                tft.fillScreen(TFT_BLACK);                // setting hours counter
                tft.setTextColor(TFT_WHITE, TFT_BLACK);
                tft.drawString(hhmm,20,20,7);
                tft.fillTriangle(85, 125, 35, 150, 135, 150, TFT_YELLOW);
                tft.drawString("HOURS",55,160,1);
                SettingTime = 1;
                break;
              }
              if ((T.TimeCheck()) & (SettingTime == 1)) {  // setting minutes counter
                tft.fillScreen(TFT_BLACK);
                tft.setTextColor(TFT_WHITE, TFT_BLACK);
                tft.drawString(hhmm,20,20,7);
                tft.fillTriangle(235, 125, 190, 150, 280, 150, TFT_YELLOW);
                tft.drawString("MINUTES",195,160,1);
                SettingTime = 2;
                break; 
               }
                if ((T.TimeCheck()) & (SettingTime == 2)) {  // setting minutes counter 
                tft.fillScreen(TFT_BLACK);
                tft.setTextColor(TFT_YELLOW, TFT_BLACK);
                tft.drawString(hhmm,20,20,7);
                tft.setTextSize(3);
                tft.drawString("START EMISSION",5,160,2);
                SettingTime = 0;
                Emission = true;
                break; }
            } 
                case SuperButton::Press::DOUBLE:
                {
                Serial.println("double pressed");
                }  
                break;

                case SuperButton::Press::LONGER:{
                  if ((T.TimeCheck()) & (Emission == true)) {  // starting up timer to emmision of ozone
                  tft.fillScreen(TFT_BLUE);
                  tft.setTextColor(TFT_WHITE, TFT_BLUE);
                  tft.drawString(hhmm,20,20,7);
                  SettingTime = 0;
                  T.SetTimer(0,0,0,5); 
                  T.StartTimer(); 
                  i = true;       
                  }  break;
                default:;
                }
        }
}
