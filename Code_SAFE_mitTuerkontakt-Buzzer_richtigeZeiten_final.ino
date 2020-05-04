// Bibliotheken
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <SPI.h>

// Pindefinitionen
#define led_gruen 6
#define led_rot 7
#define buzzer 8
#define tuerkontakt 9
#define relais 22
#define SS_PIN 53
#define RST_PIN 5

MFRC522 mfrc522(SS_PIN, RST_PIN);

// Eigenschaften vom Display
LiquidCrystal_I2C lcd(0x3F, 16, 2);


char passwort_richtig[4] = {'0', '1', '8', '7'};  // Hier werden die vier Zeichen des Passwortes eingegeben Hier: "1234"
String tagUID = "7A DF D5 0B";                    // UID des richtigen RFID-Tags ___ RICHTIGE KARTE 7A DF D5 0B
char eingegebenes_pw[4];                          // Unter "eingegebenes_pw" werden im Loop die vier eingegebenen Zeichen gespeichert
boolean RFIDMode = true;                          // boolean um Modi zu toggeln
char taste_gedrueckt = 0;                         // Variable to store incoming keys
uint8_t i = 0;                                    // Variable für einen Counter der Anzahl bisher eingegebener Ziffern

// Variablen
int Pinversuche = 0;
unsigned long tuergeoeffnet;
unsigned long zeittuergeoeffnet;
String tueanzeigen1 = "";
String tueanzeigen2 = "";
String oldtueanzeigen1 = "";
String oldtueanzeigen2 = "";


// Hier wird die größe des Keypads definiert
const byte ROWS = 4;  // 4 Spalten
const byte COLS = 4;  // 4 Zeilen

// Anordnung der Ziffern und Zeichen des Keypads
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte colPins[COLS] = {37, 36, 35, 34};  // Definition der Pins für die 4 Spalten
byte rowPins[ROWS] = {33, 32, 31, 30};  // Definition der Pins für die 4 Zeilen


Keypad Tastenfeld = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup() {

  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(relais, OUTPUT);     // relais
  pinMode(led_gruen, OUTPUT);  // grün
  pinMode(led_rot, OUTPUT);    // rot
  pinMode(tuerkontakt, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("(c) MORITZ LERCH");
  lcd.setCursor(0, 1);
  lcd.print("    KONRAD G");
  lcd.write(239);
  lcd.print("TZ");
  Serial.println("Initialisierung!");

  int a = 0;
  while (a < 5) // 5x rot --> grün blinken
  {
    digitalWrite  (led_rot, HIGH);
    digitalWrite  (led_gruen, LOW);
    delay (300);
    digitalWrite  (led_rot, LOW);
    digitalWrite  (led_gruen, HIGH);
    delay (300);
    a++;
  }

  digitalWrite  (led_rot, HIGH);
  digitalWrite  (led_gruen, LOW);
  delay (700);

  lcd.clear();

}

void loop() {

  noTone(buzzer);

  if (RFIDMode == true) {
    mfrc522.PCD_Init();
    lcd.setCursor(0, 0);
    lcd.print("BITTE RFID TAG");
    lcd.setCursor(0, 1);
    lcd.print("SCANNEN.");
    Serial.println("Bitte RFID-Tag scannen!");
    digitalWrite(led_rot, HIGH);
    digitalWrite(led_gruen, LOW);
    Pinversuche = 0;


    if ( ! mfrc522.PICC_IsNewCardPresent()) // Wenn keine Karte in Reichweite ist...
    {
      delay(5);
      return; // ...springt das Programm zurück vor die if-Schleife, womit sich die Abfrage wiederholt.
    }


    if ( ! mfrc522.PICC_ReadCardSerial()) // Wenn kein RFID-Sender ausgewählt wurde
    {
      return; // ...springt das Programm zurück vor die if-Schleife, womit sich die Abfrage wiederholt.
    }

    // Auslesen der Karte
    String tag = "";
    for (byte j = 0; j < mfrc522.uid.size; j++)
    {
      tag.concat(String(mfrc522.uid.uidByte[j] < 0x10 ? " 0" : " "));
      tag.concat(String(mfrc522.uid.uidByte[j], HEX));
      Serial.println(String(mfrc522.uid.uidByte[j], HEX));
    }
    tag.toUpperCase();

    Serial.println("Die Kartennummer lautet: "); // "Die Kartennummer lautet:" --> Serial Monitor
    Serial.print(tag.substring(1));

    // Prüfen der erkannten Karte
    if (tag.substring(1) == tagUID)
    {
      // Wenn UID des Tags übereinstimmt
      digitalWrite(led_rot, LOW);
      digitalWrite(led_gruen, HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("RICHTIGER RFID");
      lcd.setCursor(0, 1);
      lcd.print("TAG !");
      Serial.println("Richtiger RFID Tag.");
      tonepositive();
      delay(2000);

      digitalWrite(led_rot, HIGH);
      digitalWrite(led_gruen, LOW);

      lcd.clear();
      Serial.println("Eingegebener Code: ");
      digitalWrite(led_rot, LOW);
      lcd.setCursor(0, 0);
      lcd.print("PIN EINGEBEN!");
      lcd.setCursor(0, 1);          // damit direkt in die nächste Zeile geschrieben wird
      RFIDMode = false;             // RFID Mode wird auf "false" gesetzt
    }

    else
    {
      // Wenn UID des Tags NICHT übereinstimmt
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("!!FALSCHER RFID");
      lcd.setCursor(0, 1);
      lcd.print("  TAG!!");
      Serial.println("FALSCHER TAG!");
      tonenegative();
      delay(30);

      int b = 0;
      while (b < 3)
      {
        digitalWrite(led_rot, LOW);
        delay (100);
        digitalWrite(led_rot, HIGH);
        delay (100);
        b++;
      }

      delay(50);
      lcd.clear();
      delay(10);
      lcd.setCursor(0, 0);
      lcd.print("BITTE ERNEUT");
      lcd.setCursor(0, 1);
      lcd.print("RFID-TAG SCANNEN! ");
      Serial.println("Bitte erneut RFID-Tag scannen!");
      delay(2000);
      lcd.clear();
    }
  }

  // Wenn RFID-Mode auf "false" --> Wartet auf Eingabe von Tastenfeld
  if (RFIDMode == false) {
    taste_gedrueckt = Tastenfeld.getKey(); // Tasteneingabe speichern
    if (taste_gedrueckt)
    {
      eingegebenes_pw[i++] = taste_gedrueckt; // Tasteneingabe speichern in "eingegebenes_pw"
      lcd.print("*");
      tone(buzzer, 700);
      digitalWrite(led_rot, HIGH);
      delay(50);
      Serial.print("*");
      digitalWrite(led_rot, LOW);
      delay(20);
      noTone(buzzer);

    }
    if (i == 4) // Wenn 4 Tasten gedrückt wurden
    {
      delay(200);
      if (!(strncmp(eingegebenes_pw, passwort_richtig, 4))) // Wenn das Passwort stimmt
      {
        Serial.println("Code korrekt!");
        delay(200);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("CODE KORREKT!");
        lcd.setCursor(0, 1);
        lcd.print("SAFE OFFEN!");
        Serial.println("Relais wird geschaltet.");
        digitalWrite(led_rot, LOW);
        digitalWrite(led_gruen, HIGH);

        digitalWrite(relais, HIGH);
        tonepositive();
        delay(80);
        tone(buzzer, 1200);
        delay(90);
        noTone(buzzer);
        delay(80);
        tone(buzzer, 1200);
        delay(90);
        noTone(buzzer);

        tuergeoeffnet = millis();


        while ((digitalRead(tuerkontakt) == LOW) && tuergeoeffnet + 10000 > millis())
        {
          Serial.println("==");
          delay(10);
        }

        lcd.clear();
        int q = 3;
        while (q >= 0)
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Noch ");
          lcd.print(q);
          lcd.print("Sek.");
          lcd.setCursor(0, 1);
          lcd.print("ge");
          lcd.write(239); // --> ö
          lcd.print("ffnet!");
          delay(1000);
          q--;
        }

        lcd.clear();
        lcd.print("TUER IST OFFEN!");
        digitalWrite(led_rot, HIGH);
        digitalWrite(led_gruen, LOW);
        digitalWrite(relais, LOW);

        zeittuergeoeffnet = millis();
        Serial.println(zeittuergeoeffnet);


        while (digitalRead(tuerkontakt) != LOW)
        {
          Serial.println("!=");
          if (zeittuergeoeffnet + 15000 < millis())
          {
            if (zeittuergeoeffnet + 30000 < millis())
            {
              if (zeittuergeoeffnet + 45000 < millis())
              {
                tueanzeigen1 = "SOFORT TUER";
                tueanzeigen2 = "SCHLIESSEN!";
                refresh();
                tone(buzzer, 600);
                delay(200);
                noTone(buzzer);
                delay(200);
              }

              else
              {
                tueanzeigen1 = "JETZT TUER";
                tueanzeigen2 = "SCHLIESSEN!";
                refresh();
                tone(buzzer, 600);
                delay(100);
                noTone(buzzer);
                delay(100);
                tone(buzzer, 600);
                delay(100);
                noTone(buzzer);
                delay(100);
                tone(buzzer, 600);
                delay(100);
                noTone(buzzer);
                delay(100);

                delay(3000);
              }
            }

            else
            {
              tueanzeigen1 = "BITTE TUER";
              tueanzeigen2 = "SCHLIESSEN!";
              refresh();
              tone(buzzer, 600);
              delay(100);
              noTone(buzzer);
              delay(100);
              tone(buzzer, 600);
              delay(100);
              noTone(buzzer);
              delay(100);

              delay(6000);
            }
          }
        }



        lcd.clear();

        digitalWrite(led_rot, HIGH);
        digitalWrite(led_gruen, LOW);
        digitalWrite(relais, LOW);
        lcd.clear();
        i = 0;
        RFIDMode = true; // RFID-Mode --> "true"
      }

      else    // Wenn das Passwort nicht stimmt
      {
        Serial.println("Code falsch");
        if (Pinversuche <= 1)
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("CODE FALSCH!");
          lcd.setCursor(0, 1);
          lcd.print("ERNEUT VERSUCHEN!");
          Pinversuche++;
          Serial.println("Pinversuche ");
          Serial.print(Pinversuche);
          digitalWrite(led_rot, HIGH); // Die rote LED leuchtet
          digitalWrite(led_gruen, LOW); // Die grüne LED is aus
          digitalWrite(relais, LOW);

          tonenegative();
          delay(1500);

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("PIN EINGEBEN!");
          lcd.setCursor(0, 1);          // damit direkt in die nächste Zeile geschrieben wird
          i = 0;
          RFIDMode = false;             // RFID Mode wird auf "false" gesetzt
        }

        else
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("ZU VIELE");
          lcd.setCursor(0, 1);
          lcd.print("VERSUCHE!");

          tonenegative();

          Serial.println("Neustart, da zu viele Versuche!");
          delay(2500);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(" !!!NEUSTART!!! ");
          lcd.setCursor(0, 1);
          lcd.print("////////////////");

          int v = 1;
          while (v <= 3) {
            tone(buzzer, 600);
            delay(700);
            noTone(buzzer);
            delay(500);
            v++;
          }

          delay(2000);
          Pinversuche = 0;
          lcd.clear();
          i = 0;
          RFIDMode = true;  // RFID-Mode --> "true"
        }
      }
    }
  }
}

void refresh()
{
  if ((tueanzeigen1 != oldtueanzeigen1) || (tueanzeigen2 != oldtueanzeigen2))
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(tueanzeigen1);
    lcd.setCursor(0, 1);
    lcd.print(tueanzeigen2);
  }

  oldtueanzeigen1 = tueanzeigen1;
  oldtueanzeigen2 = tueanzeigen2;
}

void tonepositive()
{
  tone(buzzer, 900);
  delay(150);
  tone(buzzer, 1200);
  delay(90);
  noTone(buzzer);
}

void tonenegative()
{
  tone(buzzer, 1000);
  delay(150);
  tone(buzzer, 500);
  delay(250);
  noTone(buzzer);
}
