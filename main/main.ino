#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x20,16,2);

#define start 4
#define finish 5
#define wireRod 6
#define buzzer 9

int leastTime = 0;
long gameTime  = 0;
long millisFlag = 0;

bool backAtHome = false;
bool gameOver = false;

void setup(){
    pinMode(start,INPUT);
    pinMode(finish,INPUT);
    pinMode(wireRod,INPUT);
    pinMode(buzzer,OUTPUT);
    lcd.begin(16,2); 
    lcd.backlight();
    lcd.print("Initializing");
    delay(1000);
    lcd.clear();
    while(digitalRead(start) == HIGH){
        lcd.print("Place the Rod At");
        lcd.setCursor(0,1);
        lcd.print("Start Position");
    }
    lcd.print("Ready!!");
    lcd.setCursor(0,1);
    lcd.print("Least Time - NO");
}
void loop(){
    if(digitalRead(start) == LOW){
        lcd.setCursor(0,0);
        lcd.print("Ready!!");
        backAtHome = true;
        gameOver = false;
    }
    if(backAtHome == false  && gameOver == false){
        lcd.setCursor(0,0);
        lcd.print("Return backAtHome");
    }    

    if(digitalRead(start) == HIGH && digitalRead(finish) == HIGH && backAtHome == true){
        millisFlag = millis()/1000;
        while(digitalRead(wireRod) == HIGH){
            lcd.setCursor(0,0);
            lcd.print("Let's Go    ");
            gameTime = millis()/1000;
            gameTime = gameTime - millisFlag;
            lcd.setCursor(0,1);
            lcd.print("Your Time ");
            lcd.print(gameTime);
            lcd.print("s");
            if(digitalRead(wireRod) == LOW){
                lcd.setCursor(0,0);
                lcd.print("Game-Over!!!");
                alarmGameOver();
                lcd.setCursor(0,1);
                lcd.print("Return backAtHome");
                backAtHome = false;
                gameOver = true;
                break;
            }
            if(digitalRead(finish) == LOW){
                lcd.setCursor(0,0);
                lcd.print("You Win !!!");
                lcd.setCursor(0,1);
                if(leastTime < gameTime){
                    leastTime = gameTime;
                }
                backAtHome = false;
                break;
            }
        }
    }
}
void alarmGameOver(){
    tone(buzzer,450);
    delay(1000);
    noTone(buzzer);
    delay(700);
}
