#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(32,16,2);
#include <Adafruit_NeoPixel.h>
#define TEMPO_DO_TIMER 2

Adafruit_NeoPixel pixels(7, 4, NEO_GRB + NEO_KHZ800);

int liga = 0;
int cont_timer = 0;
bool inicia_timer = false;
bool perdeu = false;

// C++ code
//
void setup()
{  
  //configurando os pinos
  pinMode(7,OUTPUT);//pino do buzzer
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), blink, RISING);
  pinMode(8,INPUT);
  pinMode(9,INPUT);
  pinMode(10,INPUT);
  pinMode(11,INPUT);
  pinMode(12,INPUT);
  pinMode(13,INPUT);
  
  pixels.begin();
  
  //configuracao do timer
  TCCR1A = 0;                        //confira timer para operação normal pinos OC1A e OC1B desconectados
  TCCR1B = 0;                        //limpa registrador
  TCCR1B |= (1<<CS10)|(1 << CS12);   // configura prescaler para 1024: CS12 = 1 e CS10 = 1
 
  TCNT1 = 0xC2F7;                    // incia timer com valor para que estouro ocorra em 1 segundo
                                     // 65536-(16MHz/1024/1Hz) = 49911 = 0xC2F7
  
  TIMSK1 |= (1 << TOIE1);           // habilita a interrupção do TIMER1
  
  lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight 
  Serial.begin(9600);
}
// colocar tempo do timer dps
typedef struct{
  int red;
  int blue;
  int green;
}tp_cores;

tp_cores cores;
int corpixels(int ordem){
  if(ordem == 1){
    cores.red = 255;
    cores.blue = 0;
    cores.green = 0;
  }
  if(ordem == 2){
    cores.red = 0;
    cores.blue = 255;
    cores.green = 0;
  }
  if(ordem == 3){
    cores.red = 0;
    cores.blue = 0;
    cores.green = 255;
  }
  if(ordem == 4){
    cores.red = 255;
    cores.blue = 255;
    cores.green = 0;
  }
  if(ordem == 5){
    cores.red = 255;
    cores.blue = 0;
    cores.green = 255;
  }
  if(ordem == 6){
    cores.red = 0;
    cores.blue = 255;
    cores.green = 255;
  }
  
}

int jogar()
{
  int aux, i, rodada=1, ordem[11];
  int fase=1;
  int qtdleds=5;
  lcd.clear();
  pixels.clear();
  if(liga%2 != 0)// teste de botão
  {
    while(fase <=3){
    for(i=1; i<=10; i++){

        if(liga%2 == 0)
          return 0;

        aux = random(1,qtdleds);
        ordem[i] = aux; 
    }
    rodada=1;
    while(rodada<=10){
      lcd.clear();
      for(i=1; i<=rodada; i++) {
      lcd.setCursor(0,0); 
      lcd.print("LEVEL ");
      lcd.setCursor(6,0); 
      lcd.print(fase);
      lcd.setCursor(0,1); 
      lcd.print("ROUND ");
      lcd.setCursor(7,1); 
      lcd.print(rodada);

        if(liga%2 == 0)
          return 0;
      
      corpixels(ordem[i]);
      pixels.setPixelColor(ordem[i], pixels.Color(cores.red, cores.green, cores.blue));
      pixels.show();
      pixels.clear();

      tone(7, 392, 100);

      delay(1000);// tem q usar o timer
    } 
    
    int cliques[10] = {0,0,0,0,0,0,0,0,0,0};
    i=1;
    inicia_timer = true;
    while(true){
      if(liga%2 == 0)
        return 0;
      
      
      if(digitalRead(13) == HIGH){
      	cont_timer = 0;
        cliques[i] = 1;
        corpixels(cliques[i]);
        pixels.setPixelColor(cliques[i], pixels.Color(cores.red, cores.green, cores.blue));
        pixels.show();
      	pixels.clear();
      	i++;
        tone(7, 392, 100);
        delay(300);
      }
      
      else if(digitalRead(11) == HIGH){
      	cont_timer = 0;
        cliques[i] = 2;
        corpixels(cliques[i]);
        pixels.setPixelColor(cliques[i], pixels.Color(cores.red, cores.green, cores.blue));
        pixels.show();
      	pixels.clear();
      	i++;
        tone(7, 392, 100);
        delay(300);
      }
      
      else if(digitalRead(10) == HIGH){
        cont_timer = 0;
        cliques[i] = 3;
        corpixels(cliques[i]);
        pixels.setPixelColor(cliques[i], pixels.Color(cores.red, cores.green, cores.blue));
        pixels.show();
      	pixels.clear();
      	i++;
        tone(7, 392, 100);
        delay(300);
      }
      
      else if(digitalRead(9) == HIGH){
        cont_timer = 0;
        cliques[i] = 4;
        corpixels(cliques[i]);
        pixels.setPixelColor(cliques[i], pixels.Color(cores.red, cores.green, cores.blue));
        pixels.show();
      	pixels.clear();
      	i++;
        tone(7, 392, 100);
        delay(300);
      }
      
      if(cliques[i-1] != 0 && cliques[rodada] == ordem[rodada]){
      	inicia_timer = false;
        lcd.setCursor(0,0); 
        lcd.print("    NEXT   ");
        rodada++;
      	delay(1000);
     	break;}
      else if((cliques[i-1] != 0 && cliques[i-1] != ordem[i-1]) || perdeu == true){
        inicia_timer = false;
        perdeu = false;
        lcd.clear();
        lcd.setCursor(0,0); 
        lcd.print("  GAME OVER!  ");
        liga++;
        delay(5000);
     	return 0;}
      }
        
    }
    
    qtdleds+=1;
    fase+=1;}
    
    
      lcd.setCursor(0,1); 
      lcd.print("  OR NOT...  ");
      delay(5000);
      lcd.clear();
      lcd.setCursor(6,0); 
      lcd.print("YOU");
      lcd.setCursor(6,1); 
      lcd.print("WIN");
      delay(7000);
      liga++;
      return 1;
    
  }  
}


void loop() {
	jogar();
  }

void blink(){
 liga++;
}

ISR(TIMER1_OVF_vect)  //interrupção do TIMER1 
{
  if(true){
    Serial.println(cont_timer);
    cont_timer++;}
  if(cont_timer > TEMPO_DO_TIMER){
    inicia_timer = false;
    perdeu = true;
  }
  if(inicia_timer == false){
    cont_timer = 0;
  }
  TCNT1 = 0xC2F7;  // Renicia TIMER
}

