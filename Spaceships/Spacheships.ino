#include "LedControl.h"
#include "animatii.h"
#include <LiquidCrystal.h>
#define V0_PIN 9
#define INTERVAL 50
#define SW_PIN 13
#define X_PIN A0
#define Y_PIN A1
#define MESAJ__1 "Spaceships"
#define MESAJ__2 "Ala esti tu "
#define MESAJ__3 "Vezi cum joci "
#define TIP_0 " <    usor    > "
#define TIP_1 " <   mediu    >  "
#define TIP_2 " <    greu    > "
#define TIP_3 " < imposibil  >  "
#define TIP_4 " <  cancer    > "
LiquidCrystal lcd( 2 , 3 , 4 , 5 , 6 , 7 );
LedControl lc = LedControl(12, 11, 10);
int level = 0;
int a = 0, b = 0, c = 7, d = 7, e = 0; // variabile pentru parcurgerea matricii
int shiftare_stanga = 2;
int prima_executare = 0;
int shiftare_dreapta = 13;
int indice_cursor = 16;
int indice_cursor_2 = 16;
int sus_sau_jos;
int yvalue, xvalue;
int switch_value;
int bombe = 0;
int cautatoare = 0;
int contor = 0;
int pozx_nava = 3;
int pozy_nava = 7;
int poz_in_matrice = 0;
int valx;
int valy;
int nr_obstacole = -1;
int incrementare_viteza;
int vect_ap[8];
float exponent_de_20;
float viteza;
float previous_viteza;
unsigned long curent_time;
unsigned long previous_time = 0;
unsigned long previous_time_2 = 0;
unsigned long pierdere_de_timp;
unsigned int nr_ap_previous;
unsigned int nr_ap_vechi;
unsigned int nr_ap;
bool afisez_mesaj_principal = true;
bool afisare_animatii_final = false;
bool alegere_meniu = false;
bool aleg_dificultate_nivel = false;
bool am_jucat_ceva = false;
bool afisare_mesaj_lcd = false;
bool mesaj_miscator = false;
bool afisare_animatii_inceput = true;
bool poti_juca = false;
bool animatie_inceput = false;
bool porneste = false;
bool avem_voie = false;
bool mesaj_dupa_joc = false;
bool am_jucat = false;
struct coordonate_dusmani
{
  int x, y, incep;
  bool  bomba_sau_cautatoare = false;
} v[8];
void setup() {  //initializari componente
  lcd.begin ( 16 , 2 );
  lc.shutdown(0, false);
  lc.setIntensity(0, 4);
  Serial.begin(9600);
  pinMode (V0_PIN, OUTPUT );
  analogWrite (V0_PIN, 60);
  pinMode (SW_PIN, INPUT_PULLUP );
}
void umple_matricea()
{

  lc.setLed(0, a, b, 1); //sa fie lumina
  b++;
  if (b == 8)
  {
    a++;
    b = 0;
  }
}
void afiseaza_matricea(int k) // afisez animatii din animatii din matrice
{
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0, i, IMAGES[k][i]);
  }
}
void goleste_matricea()
{

  if ((d != 7 && c > 0) || (c == 0 && d < 2))
    lc.setLed(0, c, d, 0); //sa fie intuneric
  d--;
  if (d == -1)
  {
    c--;
    d = 7;
  }
}

void decapitare()
{
  afiseaza_matricea(poz_in_matrice);
  poz_in_matrice++;
}
void fara_decapitare()
{
  poz_in_matrice--;
  afiseaza_matricea(poz_in_matrice);
}
void initiere_joc()
{
  curent_time = millis();
  if (afisare_animatii_inceput == true)
  {

    if (curent_time >= previous_time + INTERVAL && poz_in_matrice <= 5)
    {
      prima_executare = 1;
      if (a != 8)
        umple_matricea();
      else
      {
        if (c != -1)
          goleste_matricea();
        else

        {
          if (a == 8 && c == -1 )
          {
            afiseaza_matricea(poz_in_matrice);
            poz_in_matrice++;
            if (poz_in_matrice == 6) ///chestia asta se executa o singura data
            {
              afisare_mesaj_lcd = true;
              afisare_animatii_inceput = false;
              a = 0; b = 0; // le initializez pentru final
              c = 7; d = 7;
            }

          }

        }
      }

      previous_time = curent_time;
    }
    if (curent_time >= INTERVAL / 2 + previous_time && poz_in_matrice <= 5 && prima_executare == 1) // execut umplerea matricii in paralel
    {
      prima_executare = 0;
      if (a != 8)
        umple_matricea();
      else
      {
        if (c != -1)
          goleste_matricea();
      }
    }
  }
}
void goleste_matricea_total()
{
  lc.setLed(0, c, d, 0);
  d--;
  if (d == -1)
  {
    d = 7;
    c--;
  }
}

void dezinitiere_joc()
{
  if (am_jucat == true && afisare_animatii_final == true)
  {
    if (curent_time >= INTERVAL * 2 + previous_time && poz_in_matrice > 5 ) // il decapiteaza
    {
      prima_executare = 1;
      if (poz_in_matrice != 15)
        decapitare();
      else
      {
        if (a != 8)
          umple_matricea();
        else if (c != -1)
          goleste_matricea_total();
        else
        {
          lcd.clear();
          exit(0);//end of game
        }
      }
      previous_time = curent_time;
    }
    if (curent_time >= previous_time + INTERVAL  && poz_in_matrice > 14 && prima_executare == 1)
    {
      prima_executare = 0;
      if (a != 8)
        umple_matricea();
      else if (c != -1)
        goleste_matricea_total();
      else
      {
        lcd.clear();
        exit(0);//end of game
      }
    }
  }
  else if (am_jucat == false && afisare_animatii_final == true)
  {

    if (( curent_time >= previous_time + INTERVAL * 2 ) && poz_in_matrice <= 6 )
    {
      prima_executare = 1;
      if (poz_in_matrice != 0)
        fara_decapitare();
      else
      {
        if (a != 8)
          umple_matricea();
        else if (c != -1) 
          goleste_matricea_total();
        else
        {
          lcd.noDisplay(); // prea mult clear
          exit(0);//end of game
        }
      }
      previous_time = curent_time;
    }
    if ((curent_time >= previous_time + INTERVAL)  && poz_in_matrice <= 0 && prima_executare == 1)
    {
      prima_executare = 0;
      if (a != 8)
        umple_matricea();
      else if (c != -1)
        goleste_matricea_total();
      else
      {
        lcd.noDisplay();
        exit(0);//end of game
      }
    }
  }
}

void mesaje_lcd_inceput()
{
  if (afisare_mesaj_lcd == false) 
  {
    lcd.setCursor(3, 0);
    lcd.print(MESAJ__1);
  }
  if ((curent_time > previous_time_2 + INTERVAL * 5) && afisare_mesaj_lcd == false) // afiseaza constant mesajul Spaceships
  {
    lcd.clear();
    lcd.setCursor(shiftare_stanga, 0);
    lcd.print("<");
    lcd.setCursor(shiftare_dreapta, 0);
    lcd.print(">");
    shiftare_stanga--; //fac cele doua sageti sa se miste
    shiftare_dreapta++;
    if (shiftare_stanga == -1)
      shiftare_stanga = 2;
    if (shiftare_dreapta == 16)
      shiftare_dreapta = 13;
    previous_time_2 = curent_time;
  }
  if (afisare_mesaj_lcd == true)
  {
    if ((curent_time > previous_time + INTERVAL) && indice_cursor != -40) // afisez cele doua mesaje miscatoare
    {

      indice_cursor--; 
      if (indice_cursor == -20) // sa il fac sa plece in partea stanga
        indice_cursor = -24;
      if ( 0 >= indice_cursor  && indice_cursor >= -19) // in intervalul asta sta pe loc
      { lcd.setCursor(0, 0);
        lcd.print(MESAJ__2);
      }
      else
      {
        lcd.clear();
        lcd.setCursor(indice_cursor, 0);
        lcd.print(MESAJ__2);
      }
      previous_time = curent_time;
    }
    if (indice_cursor == -40 && (curent_time > previous_time + INTERVAL) && indice_cursor_2 != -40) // incepe dupa ce se terminca primul mesaj miscator
    {

      indice_cursor_2--;
      if (indice_cursor_2 == -20)
        indice_cursor_2 = -24;
      if ( 0 >= indice_cursor_2  && indice_cursor_2 >= -19)
      { lcd.setCursor(0, 1);
        lcd.print(MESAJ__3);
      }
      else
      {
        lcd.clear();
        lcd.setCursor(indice_cursor_2, 1);
        lcd.print(MESAJ__3);
      }
      previous_time = curent_time;
      if (indice_cursor_2 == -40)
        alegere_meniu = true;
    }
  }
}
void meniu_joc()
{

  if (alegere_meniu == true)
  {

    yvalue = analogRead (Y_PIN);
    if (afisez_mesaj_principal == true) // o sa afisez chestia asta ( meniul complet) pana o sa ma joc pe axa OY
    { lcd.setCursor(6, 0);
      lcd.print("Play");
      lcd.setCursor(6, 1);
      lcd.print("Quit");
      sus_sau_jos = -1;
      afisez_mesaj_principal = false;
    }
    if (yvalue <= 200)
    {
      sus_sau_jos = 0;
      lcd.clear();
      lcd.setCursor(6, 0);
      lcd.print("Play");
    }
    else if (yvalue >= 700)
    {
      sus_sau_jos = 1;
      lcd.clear();
      lcd.setCursor(6, 1);
      lcd.print("Quit");
    }
    switch_value = digitalRead (SW_PIN);
    if (switch_value == 0 && sus_sau_jos == 0)
    {
      pierdere_de_timp = curent_time + 300; // altfel switch-ul de aici o sa fie acelasi de la alegere meniu
      alegere_meniu = false;
      aleg_dificultate_nivel = true;
      am_jucat = true; // din moment ce am inceput un joc, se stie ca va trebui sa te decapitez
    }
    else if (switch_value == 0 && sus_sau_jos == 1)
    {
      alegere_meniu = false;
      afisare_animatii_final = true;
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Bye-Bye");
    }
  }
}
void alegere_tip_joc()
{
  if (aleg_dificultate_nivel == true)
  {
    if (curent_time > pierdere_de_timp) // un delay absolut necesar
    {
      switch_value = digitalRead (SW_PIN); 
      if (switch_value == 0)
        porneste = true;
      if (curent_time > previous_time + INTERVAL * 4 ) // aici ne alegem tipul de joc (mediiu, usor, cancer)
      {
        xvalue = analogRead (X_PIN);
        if (xvalue < 350)
        { level--;
          if (level < 0)
            level = 0;
        }
        else if (xvalue > 650)
        { level++;
          if (level > 4)
            level = 4;
        }
        lcd.setCursor(2, 0);
        lcd.print("Tip de joc");
        switch (level) //iti afisez ce nivel de joc selectezi
        {
          case 0 : {
              lcd.setCursor(0, 1);
              lcd.print(TIP_0);
              break;
            }
          case 1 : {
              lcd.setCursor(0, 1);
              lcd.print(TIP_1);
              break;
            }
          case 2 : {
              lcd.setCursor(0, 1);
              lcd.print(TIP_2);
              break;
            }
          case 3 : {
              lcd.setCursor(0, 1);
              lcd.print(TIP_3);
              break;
            }
          case 4 : {
              lcd.setCursor(0, 1);
              lcd.print(TIP_4);
              break;
            }
        }

        if (porneste == true) // vreau sa joc, am ales nivelul locului
        {
          aleg_dificultate_nivel = false; // initializari pentru joc
          poti_juca = true;
          animatie_inceput = true;
          nr_ap = 0;
          nr_ap_previous = -1;
          nr_ap_vechi = 0;
          viteza = 4;
          previous_viteza = -3;
          incrementare_viteza = 0;
          exponent_de_20 = 100;
          prima_executare = 0;
          nr_obstacole = -1;
          contor = 0;
          pozx_nava = 3;
          pozy_nava = 7;
          for (int i = 0; i < 8; i++)
          {
            vect_ap[i] = 0;
            v[i].bomba_sau_cautatoare = false;
          }
          lcd.clear();
          switch (level) // iti setez tipurile si numarul de bombe necesare
          {
            case 0 : {
                bombe = 2;
                cautatoare = 0;
                break;
              }
            case 1 : {
                bombe = 5;
                cautatoare = 0;
                break;
              }
            case 2 : {
                bombe = 2;
                cautatoare = 1;
                avem_voie = true;
                break;
              }
            case 3 : {
                bombe = 3;
                cautatoare = 2;
                avem_voie = true;
                break;
              }
            case 4 : {
                bombe = 0;
                cautatoare = 5;
                avem_voie = true;
                break;
              }
          }

        }
        previous_time = curent_time;
      }

    }
  }
}
void afisare_animatie_inceput()
{
  if (curent_time > previous_time + INTERVAL * 3 && contor != 16) // o sa fie un efect special, pentru ca sting si aprind un led foarte rapid
  {
    lc.setLed(0, pozy_nava, pozx_nava, 1);
    contor ++ ;
    previous_time = curent_time;
    if (contor == 16) // vaface chestia asta de 16 ori
    {
      animatie_inceput = false;
      contor = 0;
    }
    lc.clearDisplay(0);
  }

}
void luminare_bombe(int k)
{
  for (int i = 0; i <= k; i++)
    lc.setLed(0, v[i].y, v[i].x, 1); //sa fie lumina
}
void aprindere_completa(int k)
{
  for (int i = 0; i <= k; i++)
    if (v[i].bomba_sau_cautatoare == true)
      lc.setLed(0, v[i].y, v[i].x, 1); //sa fie lumina
}
void corelare_obstacole_aparitie(int k)
{
  int i;
  for (i = 0; i <= k; i++)
  {
    v[i].y++;
    if (v[i].y > 7)
    {
      v[i].y = 0;
      vect_ap[v[i].incep] = 0;
    }
    if (v[i].y == 0)
    {
      v[i].x = pozx_nava;
      while (vect_ap[v[i].x] == 1 ) // cauta pana cand pozitia pe care vine nu este ocupata si pica daca se poate pe pozitita ta
        v[i].x = random(0, 8);
      vect_ap[v[i].x] = 1;
      v[i].incep = v[i].x;
    }

  }
}
int verificare(int k)
{
  for (int i = 0; i <= k; i++)
    if (v[i].y == pozy_nava && v[i].x == pozx_nava )
      return 1;
  return 0;
}
void bomba_te_cauta_jos(int k)
{
  int i;
  for (i = 0; i <= k; i++)
  {
    if (v[i].bomba_sau_cautatoare == true) // doar in cazul cautatoarelor facem asta
    {
      if (v[i].x != pozx_nava) // asa il cauta bomba
        if (v[i].x < pozx_nava)
          v[i].x++;
        else
          v[i].x--;
    }
  }
}
void ai_pierdut()
{
  if (curent_time > previous_time + INTERVAL)  // iti arata scorul si feedback-ul
  {
    if(contor <= 16)
    {
      lcd.setCursor(contor, 1);
      lcd.print(" ");
    }
    contor++;
    if (nr_ap > 300 )
      for (int i = 0; i < 8; i++)
        lc.setRow(0, i, IMAGES[IMAGES_LEN - 1][i]); //smile face
    else
      for (int i = 0; i < 8; i++)
        lc.setRow(0, i, IMAGES[IMAGES_LEN - 2][i]); //sad face
    switch_value = digitalRead (SW_PIN); //ceva nou
    if (contor >= 25 && switch_value == 0)  // scorul va sta putin
    {                                       //cateva initializari inainte sa se termine mesajul
      level = 0;
      alegere_meniu = true; // revin la primul meniu
      porneste = false;
      mesaj_dupa_joc = false;
      afisez_mesaj_principal = true;
      avem_voie = false;
      lcd.clear();
      for (int i = 0; i < 8; i++)
        lc.setRow(0, i, IMAGES[poz_in_matrice - 1][i]); //ramane spanzuratoarea
      contor = 0;
    }
    previous_time = curent_time;
  }
}
void joc()
{
  if (poti_juca == true)
  {
    if (nr_ap_previous != nr_ap) // de cate ori sa va schimba scorul se va suprascrie
    {
      lcd.setCursor(0, 0);
      lcd.print("Scor  :");
      lcd.setCursor(8 , 0);
      lcd.print(nr_ap);
      nr_ap_previous = nr_ap;
    }
    if (viteza != previous_viteza) // cand se schimba viteza se incrementeaza si nivelul
    {
      incrementare_viteza++;
      lcd.setCursor(0, 1);
      lcd.print("Nivel : ");
      lcd.setCursor(8, 1);
      if (viteza == 1.1)
      {
        lcd.print("maxim ");
      }
      else
        lcd.print(incrementare_viteza);
      previous_viteza = viteza;
    }
    if (animatie_inceput == true) // licarire inainte de joc
      afisare_animatie_inceput();
    else // si aici incepem jocul nostru frumos
    {
      if (curent_time - previous_time >  + INTERVAL * viteza)
      {
        prima_executare = 1;
        nr_ap++;
        if (nr_ap > nr_ap_vechi + exponent_de_20 && viteza != 1.1) // fiecare nivel va tine 20 de secunde
        {
          nr_ap_vechi = nr_ap;
          viteza = viteza - 0.3;
          if (viteza < 1.1)
            viteza = 1.1;
          exponent_de_20 = 20000 / (INTERVAL * viteza);
        }
        if (bombe != 0 || cautatoare != 0) // cat timp mai exista bombe de generat
        {
          if (nr_ap % 70 == 0 || nr_ap == 1) // din 70 in 70 loop-uri mai generam o bomba
          {
            nr_obstacole++;
            v[nr_obstacole].y = -1;
            v[nr_obstacole].x = -2;
            if (bombe)
              bombe--;
            else // cand termin cu bombele simple apar si cele cautatoare
            {
              v[nr_obstacole].bomba_sau_cautatoare = true;
              cautatoare--;
            }
          }
        }
        valx = analogRead(X_PIN);
        if (valx < 200)
          pozx_nava--;
        else if ( valx > 800)
          pozx_nava++;
        if (pozx_nava < 0)
          pozx_nava = 0;
        else if (pozx_nava > 7)
          pozx_nava = 7;
        lc.clearDisplay(0);
        corelare_obstacole_aparitie(nr_obstacole);
        bomba_te_cauta_jos(nr_obstacole);
        luminare_bombe(nr_obstacole);
        lc.setLed(0, pozy_nava, pozx_nava, 1);
        if (verificare(nr_obstacole) == 1) // vad daca am murit
        {
          contor = 0;
          poti_juca = false;
          lc.clearDisplay(0);
          mesaj_dupa_joc = true;
        }
        previous_time = curent_time;
      }
      if (curent_time > previous_time + (INTERVAL * viteza) / 2  && prima_executare == 1) //pentru cazul bombelor cautatoare si stingerea celor normale
      {
        valx = analogRead(X_PIN);
        if (valx < 200)
          pozx_nava--;
        else if ( valx > 800)
          pozx_nava++;
        if (pozx_nava < 0)
          pozx_nava = 0;
        else if (pozx_nava > 7)
          pozx_nava = 7;
        lc.clearDisplay(0);
        if (avem_voie == true) // le aprind doar pe cele cautatoare, ca sa pot fi distinse = > cele simple o sa licare
          aprindere_completa(nr_obstacole);
        lc.setLed(0, pozy_nava, pozx_nava, 1);
        if (verificare(nr_obstacole) == 1)
        {
          contor = 0;
          poti_juca = false;
          lc.clearDisplay(0);
          mesaj_dupa_joc = true;
        }
        prima_executare = 0;
      }
    }
  }
  if (mesaj_dupa_joc == true)
  {
    ai_pierdut();
  }
}
void loop() {
  initiere_joc();
  mesaje_lcd_inceput();
  meniu_joc();
  alegere_tip_joc();
  joc();
  dezinitiere_joc();
}
