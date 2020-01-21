#include <LiquidCrystal_I2C.h>
#include <Wire.h> // search address 

String msg = "Wellcome to my Home!!! Enjoy Arduino"; 

LiquidCrystal_I2C lcd(0x27, 16, 2); //Create lcd Object I2Caddress, cols, rows
void setup()
{
    lcd.init(); // init I2C lcd
    lcd.backlight(); // turn on the Backlight of I2C LCD 
}

void loop()
{
    int value = random(0,3); //random number 0~2
    lcd.clear();
    
    // switch (value)
    // {
    // case 0:
    //     lcd.setCursor(0,0);
    //     lcd.print("Hello!!");
    //     break;
    // case 1:
    //     lcd.setCursor(0,1);
    //     lcd.print("Welcome!!");
    //     break;
    // case 2:
    //     lcd.setCursor(0,0);
    //     lcd.print("See you again!!");
    //     break;
    
    // default:
    //     break;
    // }
    lcd.setCursor(0,0);
    lcd.print(msg);
    while (true)
    {
        lcd.scrollDisplayLeft();
        delay(500);
    }
    delay(1000);
}