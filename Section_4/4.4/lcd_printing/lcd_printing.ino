#include <LiquidCrystal_I2C.h>

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, 16, 2);  
void setup(){
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
}
void loop(){
  // set cursor to first column, first row
  lcd.setCursor(6, 0);
  // print message
  lcd.print("Hello");
 // delay(1000);
 lcd.setCursor(5, 1);
  // print message
  lcd.print("Learner");
  // clears the display to print new message
}
