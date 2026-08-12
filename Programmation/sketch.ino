#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
// define width and height of the ssd1306 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

//define width and height of the logo
#define LOGO_HEIGHT 12
#define LOGO_WIDTH 64

//initialisation variables
const char BP_DWS = 3;
const char BP_UPS = 4;
const char BP_GE = 5;
const char BP_CHR_RST = 6;
const char BP_CHR = 7;
const char BP_RPM = 8;

int potentiometre = A3;

//initialisation etat des boutons
char EtatBP_DWS = HIGH;
char EtatBP_GE = HIGH;
char EtatBP_UPS = HIGH;
char EtatBP_CHR_RST = HIGH;
char EtatBP_CHR = HIGH;
char EtatBP_RPM = HIGH;

//initialisation des varaibles comprises dans les actions des boutons
int Gear_Engaged;
int Gear_Prv_Eng;
int RPM = 0;
int PBR_CHR = 0;
int AFF=4;

bool logoDisplayed = false; // initialisation variable pour afficher logo
Adafruit_NeoPixel led(21, 2, NEO_GRB + NEO_KHZ800);

int previousGearDisplayed = -1; // Initialize with an invalid gear value
int pot_cap_RPM;

const int start_pause_pin = 7, resetpin = 6; //initialisation des variables pour chrono
String stopwatch = "stop";
String start_pause = "low";
long start_time = 0;
long seconds = 0;

const unsigned char logo [] PROGMEM = {
0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,
0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000001,
0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000001,
0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000001,
0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000001,
0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000001,
0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000001,
0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000001,
0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000001,
0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000001,
0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000001,
0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111,

};

Adafruit_SSD1306 MonEcran(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
    Serial.begin(9600);

    if (!MonEcran.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;); // Boucle infinie en cas d'échec
    }

    MonEcran.clearDisplay();
    MonEcran.setTextSize(1);
    MonEcran.setTextColor(SSD1306_WHITE);
    MonEcran.setCursor(1000, 1000);
    MonEcran.println(F("Initialisation..."));
    MonEcran.display();
    delay(100);
    MonEcran.clearDisplay();

    // Draw logo
    MonEcran.drawBitmap(32, 23, logo, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);
    MonEcran.display();

    pinMode(BP_DWS, INPUT_PULLUP);
    pinMode(BP_UPS, INPUT_PULLUP);
    pinMode(BP_GE, INPUT_PULLUP);
    pinMode(BP_CHR_RST, INPUT_PULLUP);
    pinMode(BP_CHR, INPUT_PULLUP);
    pinMode(BP_RPM, INPUT_PULLUP);

    pinMode(potentiometre, INPUT);

    pinMode(start_pause_pin, INPUT_PULLUP);
    pinMode(resetpin, INPUT_PULLUP);

    led.begin();
    led.show();

    EtatBP_DWS = digitalRead(BP_DWS);
    EtatBP_GE = digitalRead(BP_GE);
    EtatBP_UPS = digitalRead(BP_UPS);
    EtatBP_CHR = digitalRead(BP_CHR);
    EtatBP_RPM = digitalRead(BP_RPM);
    EtatBP_CHR_RST = digitalRead(BP_CHR_RST);
}

void loop() {
  pot_cap_RPM = analogRead(potentiometre); //lecture de l'état du potentiomètre simulant le capteur compte tour
  Serial.println(pot_cap_RPM);
  EtatBP_DWS = digitalRead(BP_DWS);
  EtatBP_GE = digitalRead(BP_GE);
  EtatBP_UPS = digitalRead(BP_UPS);
  EtatBP_CHR = digitalRead(BP_CHR);
  EtatBP_RPM = digitalRead(BP_RPM);
  EtatBP_CHR_RST = digitalRead(BP_CHR_RST);

  if (EtatBP_UPS == LOW && Gear_Engaged != 5 && Gear_Engaged <= 5) {
    Gear_Engaged = Gear_Engaged + 1;
    Gear_Prv_Eng = Gear_Engaged;
    delay(200);
  }
  else if (EtatBP_DWS == LOW && Gear_Engaged > 0) {
    Gear_Engaged = Gear_Engaged - 1;
    Gear_Prv_Eng = Gear_Engaged;
    delay(200);
  }

  // Refresh display if gear changes and display is showing gear information
  if (AFF == 1 && Gear_Engaged != previousGearDisplayed) {
    MonEcran.clearDisplay();
    MonEcran.setCursor(0, 0);
    MonEcran.setTextSize(1);
    MonEcran.print("Gear Engaged: ");
    if (Gear_Engaged != 0) {
      MonEcran.println(Gear_Engaged);
    } 
    else {
      MonEcran.println("N");
    }
    MonEcran.display();
    previousGearDisplayed = Gear_Engaged; // Update previous displayed gear
  }

  if (EtatBP_RPM == LOW) { // RPM
    MonEcran.clearDisplay();
    MonEcran.setCursor(0, 0);
    MonEcran.setTextSize(2);
    MonEcran.print(F("RPM: "));
    MonEcran.println("valeur-RPM");
    MonEcran.display();
    AFF = 2;
    // Afficher la valeur RPM sur le moniteur série pour le débogage
    Serial.print(F("RPM: "));
    Serial.println(RPM);
    delay(100);
  }

  if (EtatBP_GE == LOW) { // Gear engaged
    MonEcran.clearDisplay();
    MonEcran.setCursor(0, 0);
    MonEcran.setTextSize(1);
    MonEcran.print("Gear Engaged: ");
    if (Gear_Engaged != 0) {
      MonEcran.println(Gear_Engaged);
    } 
    else {
      MonEcran.println("N");
    }
    MonEcran.display();
    AFF = 1;
    Serial.print(F("Gear Engaged: "));
    Serial.println(Gear_Engaged);
    previousGearDisplayed = Gear_Engaged; // Update previous displayed gear
    delay(1000);
  }

  if (stopwatch == "run") {
    MonEcran.clearDisplay(); // Efface l'écran avant d'afficher le chronomètre
    MonEcran.setCursor(0, 0);
    MonEcran.print("Chrono");
    MonEcran.setCursor(0, 20);
    seconds = (millis() - start_time) / 1000;
    long centisecond = ((millis() - start_time) % 1000) % 100;
    // Afficher le chrono sur l'afficheur SSD1306
    MonEcran.setCursor(0, 20);   
    MonEcran.print(seconds);
    MonEcran.setCursor(20, 20);   
    MonEcran.print(":");
    MonEcran.setCursor(30, 20);   
    MonEcran.print(centisecond);
    MonEcran.display(); 
    Serial.println(start_time);
    Serial.println(millis());
    AFF = 3;
  }
 
  if (digitalRead(resetpin) == LOW) { // Bouton de réinitialisation en INPUT_PULLUP, donc LOW signifie appuyé
    // Reset stopwatch    
    stopwatch = "stop";
    start_time = 0;
    AFF = 0;
    MonEcran.clearDisplay(); // Efface l'écran avant d'afficher le message "Press Start"
    MonEcran.setCursor(0, 0);
    MonEcran.print("Press");
    MonEcran.setCursor(0, 20);
    MonEcran.print("Start");
    MonEcran.display(); 
  } else if (stopwatch == "stop" && AFF == 0) {
    MonEcran.clearDisplay(); // Efface l'écran avant d'afficher le message "Press Start"
    MonEcran.setCursor(0, 0);
    MonEcran.print("Press");
    MonEcran.setCursor(0, 20);
    MonEcran.print("Start");
    MonEcran.display();
  }

  if (digitalRead(start_pause_pin) == LOW && stopwatch == "stop") { // Bouton de démarrage/pause en INPUT_PULLUP, donc LOW signifie appuyé
    // Start stopwatch
    start_time = millis();
    stopwatch = "run";
    start_pause = "high";
    AFF = 3;
  }
  else if (digitalRead(start_pause_pin) == LOW && stopwatch == "run" && start_pause == "low") {
    // Pause stopwatch
    stopwatch = "pause";
    start_pause = "high";
  }
  else if (digitalRead(start_pause_pin) == LOW && stopwatch == "pause" && start_pause == "low") {
    // Resume stopwatch
    start_time = millis() - (seconds * 1000);
    stopwatch = "run";
    start_pause = "high";
    AFF = 3;
  }
  else if (digitalRead(start_pause_pin) == HIGH) { // Bouton relâché
    start_pause = "low";
  }
  
    //les boucles if qui suivent servent a allumer les leds
  if (pot_cap_RPM >= 0 && pot_cap_RPM < 48){
    led.clear();
    led.setPixelColor(2, led.Color(0, 255, 0));
    led.fill(led.Color(0, 255, 0),2,1);
    led.show();
  }
  if (pot_cap_RPM >= 48 && pot_cap_RPM < 96){
    led.clear();
    led.setPixelColor(3, led.Color(0, 255, 0));
    led.fill(led.Color(0, 255, 0),2,2);
    led.show();
  }
  if (pot_cap_RPM >= 96 && pot_cap_RPM < 144){
    led.clear();
    led.fill(led.Color(0, 255, 0),2,3);
    led.show();
  }
  if (pot_cap_RPM >= 144 && pot_cap_RPM < 192){
    led.clear();
    led.setPixelColor(5, led.Color(0, 255, 0));
    led.fill(led.Color(0, 255, 0),2,4);
    led.show();
  }
  if (pot_cap_RPM >= 192 && pot_cap_RPM < 240){
    led.clear();
    led.fill(led.Color(0, 255, 0),2,5);
    led.show();
  }
  if (pot_cap_RPM >= 240 && pot_cap_RPM < 288){
    led.clear();
    led.fill(led.Color(0, 255, 0),2,6);
    led.show();
  }
  if (pot_cap_RPM >= 288 && pot_cap_RPM < 336){
    led.clear();
    led.fill(led.Color(0, 255, 0),2,7);
    led.show();
  }
  if (pot_cap_RPM >= 336 && pot_cap_RPM < 384){
    led.clear();
    led.fill(led.Color(0, 255, 0),2,8);
    led.show();
  }
  if (pot_cap_RPM >= 384 && pot_cap_RPM < 432){
    led.clear();
    led.fill(led.Color(0, 255, 0),2,9);
    led.show();
  }
  if (pot_cap_RPM >= 432 && pot_cap_RPM < 480){
    led.clear();
    led.fill(led.Color(0, 255, 0),2,9);
    led.fill(led.Color(255, 0, 0),10,1);
    led.show();
  }
  if (pot_cap_RPM >= 480 && pot_cap_RPM < 528){
    led.clear();
    led.fill(led.Color(0, 255, 0),2,9);
    led.fill(led.Color(255, 0, 0),10,2);
    led.show();
  }
  if (pot_cap_RPM >= 528 && pot_cap_RPM < 576){
    led.clear();
    led.fill(led.Color(0, 255, 0),2,9);
    led.fill(led.Color(255, 0, 0),10,3);
    led.show();
  }
  if (pot_cap_RPM >= 576 && pot_cap_RPM < 624){
    led.clear();
    led.fill(led.Color(0, 255, 0),2,9);
    led.fill(led.Color(255, 0, 0),10,4);
    led.show();
  }
  if (pot_cap_RPM >= 624 && pot_cap_RPM < 672){
    led.clear();
    led.fill(led.Color(0, 255, 0),2,9);
    led.fill(led.Color(255, 0, 0),10,5);
    led.show();
  }
  if (pot_cap_RPM >= 672 && pot_cap_RPM < 720){
    led.clear();
    led.fill(led.Color(0, 255, 0),2,9);
    led.fill(led.Color(255, 0, 0),10,6);
    led.show();
  }
  if (pot_cap_RPM >= 720 && pot_cap_RPM < 768){
    led.clear();
    led.fill(led.Color(0, 255, 0),2,9);
    led.fill(led.Color(255, 0, 0),10,6);
    led.fill(led.Color(255, 0, 255),16,1);
    led.show();
  }
  if (pot_cap_RPM >= 768 && pot_cap_RPM < 816){
    led.clear();
    led.fill(led.Color(0, 255, 0),2,9);
    led.fill(led.Color(255, 0, 0),10,6);
    led.fill(led.Color(255, 0, 255),16,2);
    led.show();
  }
  if (pot_cap_RPM >= 816 && pot_cap_RPM < 864){
    led.clear();
    led.fill(led.Color(0, 255, 0),2,9);
    led.fill(led.Color(255, 0, 0),10,6);
    led.fill(led.Color(255, 0, 255),16,3);
    led.show();
  }
  if (pot_cap_RPM >= 864 && pot_cap_RPM < 912){
    led.clear();
    led.fill(led.Color(0, 255, 0),2,9);
    led.fill(led.Color(255, 0, 0),10,6);
    led.fill(led.Color(255, 0, 255),16,3);
    led.show();
  }
  if (pot_cap_RPM >= 912 && pot_cap_RPM < 960){
    led.clear();
    led.fill(led.Color(0, 255, 0),2,9);
    led.fill(led.Color(255, 0, 0),10,6);
    led.fill(led.Color(255, 0, 255),16,4);
    led.show();
  }
  if (pot_cap_RPM >= 960){
    led.clear();
    led.fill(led.Color(0, 255, 0),2,9);
    led.fill(led.Color(255, 0, 0),10,6);
    led.fill(led.Color(255, 0, 255),16,5);
    led.show();
  }
  delay(10);
}