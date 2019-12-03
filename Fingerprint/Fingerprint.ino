#include <M5StickC.h>
#include "finger.h"

#include "SPIFFS.h"
#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2SNoDAC.h"
uint8_t userNum;           //User number
FingerPrint FP_M;

AudioGeneratorMP3 *mp3;
AudioFileSourceSPIFFS *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;

// Called when a metadata event occurs (i.e. an ID3 tag, an ICY block, etc.
void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string)
{
  (void)cbData;
  Serial.printf("ID3 callback for: %s = '", type);

  if (isUnicode) {
    string += 2;
  }

  while (*string) {
    char a = *(string++);
    if (isUnicode) {
      string++;
    }
    Serial.printf("%c", a);
  }
  Serial.printf("'\n");
  Serial.flush();
}

void CleanScreen()
{
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  //M5.Lcd.setTextSize(2);

  M5.Lcd.println("Lecteur d'empreinte");
  userNum = FP_M.fpm_getUserNum();
  while (userNum == 255) {
    userNum = FP_M.fpm_getUserNum();
  }
  M5.Lcd.print("Utilisateur:");
  M5.Lcd.println(userNum);
}


void play(const char *filename) {
  file = new AudioFileSourceSPIFFS(filename);
  id3 = new AudioFileSourceID3(file);
  id3->RegisterMetadataCB(MDCallback, (void*)"ID3TAG");
  mp3->begin(id3, out);
}


void setup() {
  M5.begin();
  SPIFFS.begin();
  out = new AudioOutputI2S(0, 1);
  mp3 = new AudioGeneratorMP3();
  Serial.begin(115200);
  Serial2.begin(19200, SERIAL_8N1, 33, 32);
  M5.Lcd.setRotation(3);
  //M5.Lcd.clear(BLACK);
  //M5.Lcd.setTextColor(YELLOW);
  //M5.Lcd.setTextFont(2);
  //M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("Lecteur d'empreinte");

  M5.Lcd.setTextColor(WHITE);
  //M5.Lcd.setCursor(0, 50);
  userNum = FP_M.fpm_getUserNum();
  M5.Lcd.print("Utilisateur:");
  M5.Lcd.println(userNum);
  play("/startup.mp3");

}

//ButtonA: Add user
//ButtonB: Matching
void loop() {
  if (mp3->isRunning()) {
    if (!mp3->loop()) mp3->stop();
  }
  uint8_t res1;
  if (M5.BtnA.wasPressed()) {
    CleanScreen();
    M5.Lcd.println("Ajout empreinte");
    play("/ajout.mp3");
    while(mp3->loop());
    mp3->stop();

    res1 = FP_M.fpm_addUser(userNum, 1);
    if (res1 == ACK_SUCCESS) {
      M5.Lcd.println("OK");
      play("/ajouter.mp3");
    }
    else if (res1 == ACK_FAIL) {
      M5.Lcd.println("Echec");
      play("/erreur.mp3");
    }
    else if (res1 == ACK_FULL) {
      M5.Lcd.println("Parfait");
      play("/ajouter.mp3");
    }
    else {
      M5.Lcd.println("Pas d'empreinte");
      play("/erreur.mp3");
    }
    //userNum++;
  }

  if (M5.BtnB.wasPressed()) {
    CleanScreen();
    M5.Lcd.println("");
    M5.Lcd.println("Verification");
    play("/id.mp3");
    while(mp3->loop());
    mp3->stop();
    
    res1 = FP_M.fpm_compareFinger();

    if (res1 == ACK_SUCCESS) {
      M5.Lcd.println("reconnu");
      play("/reconnu.mp3");
    }
    else if (res1 == ACK_NOUSER) {
      M5.Lcd.println("inconnu");
      play("/nonreconnu.mp3");
    }
    else if (res1 == ACK_TIMEOUT) {
      M5.Lcd.println("Pas d'empreinte");
      play("/nonreconnu.mp3");
    }
    else {
      M5.Lcd.println("Erreur");
      play("/erreur.mp3");
    }
  }

  M5.update();

}
