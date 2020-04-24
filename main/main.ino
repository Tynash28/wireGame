//#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//LiquidCrystal_I2C lcd(0x3F,16,2);

const int HIGH_SCORE = 2;

#define start 6
#define finish 8
#define wireRod 7
#define buzzer 10
#define redLed 11
#define greenLed 9

int leastTime = 0;
long gameTime  = 0;
long millisFlag = 0;

bool backAtHome = false;
bool gameOver = false;

void setup(){
    byte hByte = EEPROM.read(HIGH_SCORE);
    byte lByte = EEPROM.read(HIGH_SCORE+1);
    leastTime = word(hByte, lByte);
    pinMode(start,INPUT);
    pinMode(finish,INPUT);
    pinMode(wireRod,INPUT);
    pinMode(buzzer,OUTPUT);
    //lcd.init();
    lcd.begin(20, 4);
    //lcd.backlight();
    lcd.print("Initializing");
    delay(1000);
    lcd.clear();
    while(digitalRead(start) == HIGH){
        lcd.setCursor(0,0);
        lcd.print("Place the Rod At");
        lcd.setCursor(0,1);
        lcd.print("Start Position  "  );
    }
    lcd.print("Ready!!          ");
    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(0,1);
    lcd.print("Least Time: ");
    lcd.print(leastTime);
    lcd.print("s");
    startGame();
}
void loop(){
    lcd.setCursor(0,3);
    lcd.print("Least Score: ");
    lcd.print(leastTime);
    lcd.print("s");
    if(digitalRead(start) == LOW){
        lcd.setCursor(0,0);
        lcd.print("Ready!!           ");
        backAtHome = true;
        gameOver = false;
    }
    if(backAtHome == false  && gameOver == false){
        lcd.setCursor(0,0);
        lcd.print("Return backAtHome");
    }    

    if(digitalRead(start) == HIGH && digitalRead(finish) == HIGH && backAtHome == true){
        millisFlag = millis()/1000;
        if(digitalRead(wireRod) == HIGH){
          lcd.setCursor(0,1);
          lcd.print("                    ");
        }
        while(digitalRead(wireRod) == HIGH){
            digitalWrite(greenLed,HIGH);
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
                lcd.print("   Game-Over!!!");
                alarmGameOver();
                lcd.setCursor(0,1);
                lcd.print("Return backAtHome");
                backAtHome = false;
                digitalWrite(greenLed,LOW);
                gameOver = true;
                break;
            }
            if(digitalRead(finish) == LOW){
                lcd.setCursor(0,0);
                lcd.print("    You Win !!!");
                alarmWin();
                if(leastTime > gameTime){
                    //leastTime = gameTime;
                    byte hiByte = highByte(gameTime);
                    byte loByte = lowByte(gameTime);
                    EEPROM.write(HIGH_SCORE, hiByte);
                    EEPROM.write(HIGH_SCORE+1, loByte);
                    lcd.setCursor(0,2);
                    lcd.print("New Least Time !!!");
                    lcd.setCursor(0,3);
                    lcd.print("Least Score: ");
                    lcd.print(gameTime);
                    lcd.print("s");
                }
                backAtHome = false;
                digitalWrite(greenLed,LOW);
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
void alarmWin(){
    tone(buzzer,100);
    delay(1000);
    tone(buzzer,1000);
    delay(1000);
    noTone(buzzer);
    tone(buzzer,450);
    delay(1000);
    tone(buzzer,500);
    delay(700);
    noTone(buzzer);
}
void startGame(){
  for(int i=0; i<9; i++){
    tone(buzzer,10);
    delay(100);
    noTone(buzzer);
    delay(100);
  }  
}
