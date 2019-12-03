
byte indexOfCharacter(const String &Character, byte &index, byte &sequence){
    // Les chaines de caractères UTF-8 sont composées de 1 ou plusieurs octets
    // C'est pourquoi la variable Character est transmise comme une chaîne et non comme un char
    // La fonction calcul la position du caractère comme si tous les caractères tenaient sur 1 seul octet 
    // pour une comparaison future avec une autre chaine de référence ASCII dont tous les caractères tiennent par nature sur 1 octet

    const String keyAzertyAltGr      = F("0000€0000000000000000000000~#{[|`\\^@]}0¤");
    const String keyAzertyShift      = F("QBCDEFGHIJKL?NOPARSTUVZXYZ1234567890°+¨£µ0M%0./§>");
    const String keyAzerty           = F("qbcdefghijkl,noparstuvzxyz&é\"'(-è_çà)=^$*0mù²;:!<");
    const String circomflexNoShift   = F("âêîôû");
    const String circomflexShift     = F("ÂÊÎÔÛ");
    const String tremaNoShift        = F("äëïöüÿ");
    const String tremaShift          = F("ÄËÏÖÜ");
    
    // il y a un doublon avec le caractère ^ qui peut être soit interprété comme un accent ou soit comme une puissance
    // il apparait dans la chaine keyAzertyAltGr et dans la chaine keyAzerty
    // si le caractère ^ apparait seul, il sera traité comme une puissance, cas le plus intéressant pour nous
    // par conséquent le chaîne keyAzertyAltGr sera traité avant la chaîne keyAzerty

    // \ est un caractère d'échappement, donc \\ sera transformé en \ et  \" sera transformé en "
     
    String r="", compare="";
    byte c, align, indice=0;

    compare.reserve(50);
    r.reserve(4);
    
    if (keyAzertyShift.indexOf(Character)>-1){ 
      compare=keyAzertyShift;
      sequence=2;
    }else if (keyAzertyAltGr.indexOf(Character)>-1){ 
      compare=keyAzertyAltGr;
      sequence=3;
    }else if (keyAzerty.indexOf(Character)>-1){
      compare=keyAzerty;
      sequence=1;
    }else if (circomflexNoShift.indexOf(Character)>-1){ 
      compare=circomflexNoShift;
      sequence=4;
    }else if (circomflexShift.indexOf(Character)>-1){ 
      compare=circomflexShift;
      sequence=5;
    }else if (tremaNoShift.indexOf(Character)>-1){ 
      compare=tremaNoShift;
      sequence=6;
    }else if (tremaShift.indexOf(Character)>-1){ 
      compare=tremaShift;
      sequence=7;
    }

    if (compare!=""){
        for (unsigned int i=0; i<compare.length(); i++){
            align=0;
            c=compare.charAt(i);
            if (c<128){                  // 1 octet
              align=1;
            }else if (c>191 && c<224){   // 2 octets
              align=2;
            }else if (c>223 && c<240){   // 3 octets
              align=3;
            }else if (c>239 && c<248){   // 4 octets (section qui ne sera pas utilisé dans notre cas)
              align=4;
            }
    
            if (align!=0){     
              r=compare.substring(i, i+align);
              if (r==Character){
                index=indice;
                return 1;
              }
            }else{
              return 0;
            } 
            i=i+align-1;
            indice+=1;
        } 
    }else{
        return 0;
    } 
    return 0;  
}

void sendkeypad(const String &keys){
  // Trick by johnwasser
  // add 136 to the keycode
  byte test,keyc;
  String key; 
  
  if (keys.length()<255){
     for (byte i=0; i < keys.length(); i++){
        key = keys.substring(i, i + 1);
        
        test=1;
        if (key=="1"){
          keyc=225;
        }else if (key=="2"){
          keyc=226;
        }else if (key=="3"){
          keyc=227;
        }else if (key=="4"){
          keyc=228;
        }else if (key=="5"){
          keyc=229;
        }else if (key=="6"){
          keyc=230;
        }else if (key=="7"){
          keyc=231;
        }else if (key=="8"){
          keyc=232;
        }else if (key=="9"){
          keyc=233;
        }else if (key=="0"){
          keyc=234; 
        }else if (key=="/"){
          keyc=220;
        }else if (key=="*"){
          keyc=221;
        }else if (key=="-"){
          keyc=222;
        }else if (key=="+"){
          keyc=223;
        }else if (key=="E"){ // touche enter du clavier
          keyc=224;
        }else{
          test=0;
        }
    
        if (test==1){
          Keyboard.press(keyc);
          delay(5);
          Keyboard.release(keyc);
        }
     }
  }
}

void altunicode(const String &keys){
  if (keys.length()<5){
    Keyboard.press(KEY_LEFT_ALT);
    delay(5);
    sendkeypad(keys);
    delay(5);
    Keyboard.releaseAll();
    delay(5);
  }
}

void altspecialkey(unsigned long specialkey){
  if (specialkey==0xC387){ 
      altunicode("128"); // Ç
  } else if (specialkey==0xC389){
      altunicode("144"); // É
  } else if (specialkey==0xC3A6){
      altunicode("145"); // æ
  } else if (specialkey==0xC3B8){
      altunicode("155"); // ø
  } else if (specialkey==0xC398){
      altunicode("157"); // Ø
  } else if (specialkey==0xC593){
      altunicode("0156"); // œ
  } else if (specialkey==0xC388){
      altunicode("212"); // È
  } else if (specialkey==0xE296BA){
      altunicode("16"); // ►
  } else if (specialkey==0xE29784){
      altunicode("17"); // ◄
  } else if (specialkey==0xE286A8){
      altunicode("23"); // ↨
  } else if (specialkey==0xE28691){
      altunicode("24"); // ↑
  } else if (specialkey==0xE28693){
      altunicode("25"); // ↓
  } else if (specialkey==0xE28692){
      altunicode("26"); // →
  } else if (specialkey==0xE28690){
      altunicode("27"); // ←
  } else if (specialkey==0xE28694){
      altunicode("29"); // ↔
  } else if (specialkey==0xE296B2){
      altunicode("30"); // ▲
  } else if (specialkey==0xE296BC){
      altunicode("31"); // ▼
  } else if (specialkey==0xE298BA){
      altunicode("1"); // ☺
  } else if (specialkey==0xE298BB){
      altunicode("2"); // ☻
  } else if (specialkey==0xE299A5){
      altunicode("3"); // ♥
  } else if (specialkey==0xE299A6){
      altunicode("4"); // ♦
  } else if (specialkey==0xE299A3){
      altunicode("5"); // ♣
  } else if (specialkey==0xE299A0){
      altunicode("6"); // ♠
  } else if (specialkey==0xE29982){
      altunicode("11"); // ♂
  } else if (specialkey==0xE29980 ){
      altunicode("12"); // ♀
  } else if (specialkey==0xE299AA){
      altunicode("13"); // ♪
  } else if (specialkey==0xE299AB ){
      altunicode("14"); // ♫
  } else if (specialkey==0xE298BC){
      altunicode("15"); // ☼
  } else if (specialkey==0xE2809C ){
      altunicode("0147"); // “
  } else if (specialkey==0xE2809D){
      altunicode("0148"); // ”
  } else if (specialkey==0xE284A2){
      altunicode("0153"); // ™
  } else if (specialkey==0xC2A9){
      altunicode("0169"); // ©
  } else if (specialkey==0xC2AE ){
      altunicode("0174"); // ®
  }
}

void SendKey(byte key, byte control, byte alt, byte shift){
  
    if (control==1){
      Keyboard.press(KEY_LEFT_CTRL);
    }

    if (alt==1){
      Keyboard.press(KEY_LEFT_ALT);
    }

    if (shift==1){
        Keyboard.press(KEY_LEFT_SHIFT);
    }
   
    Keyboard.press(key+136);
    delay(5);
    Keyboard.releaseAll();
    delay(5);
}


void sendkeycommand(byte key){
  //#define KEY_UP_ARROW    0xDA
  //#define KEY_DOWN_ARROW    0xD9
  //#define KEY_LEFT_ARROW    0xD8
  //#define KEY_RIGHT_ARROW   0xD7
  //#define KEY_BACKSPACE   0xB2
  //#define KEY_TAB       0xB3
  //#define KEY_RETURN      0xB0
  //#define KEY_ESC       0xB1
  //#define KEY_INSERT      0xD1
  //#define KEY_DELETE      0xD4
  //#define KEY_PAGE_UP     0xD3
  //#define KEY_PAGE_DOWN   0xD6
  //#define KEY_HOME      0xD2
  //#define KEY_END       0xD5
  //#define KEY_CAPS_LOCK   0xC1
  //#define KEY_F1        0xC2
  //#define KEY_F2        0xC3
  //#define KEY_F3        0xC4
  //#define KEY_F4        0xC5
  //#define KEY_F5        0xC6
  //#define KEY_F6        0xC7
  //#define KEY_F7        0xC8
  //#define KEY_F8        0xC9
  //#define KEY_F9        0xCA
  //#define KEY_F10       0xCB
  //#define KEY_F11       0xCC
  //#define KEY_F12       0xCD
    Keyboard.write(key);
    delay(5);
}

void decodeUTF8(const String & Texte){
    String r="";
    byte c, ctrl, alt, shift;
    byte indice, index, sequence, align;
    unsigned long specialkey;
      
    for (byte i=0; i<Texte.length(); i++)
    {
        ctrl=0, alt=0, shift=0;
        align=0;
        specialkey=0;
        c = Texte.charAt(i);
        if (c<128){
          // 1 octet
          align=1;
        }else if (c>191 && c<224){
          // 2 octets
          align=2;
        }else if (c>223 && c<240){
          // 3 octets
          align=3;
        }else if (c>239 && c<248){ // section qui ne sera pas utilisé dans notre cas
          // 4 octets 
          align=4;
        }

        if (align!=0){
            indice = i;
            i=i+align-1;  
            r=Texte.substring(indice, indice + align);
         
            // traitement particulier de certaines commandes et caractères
            if (r==" "){
              sendkeycommand(0x2c+136);
            }else if (c==9){
              sendkeycommand(KEY_TAB);
            }else if (c==13){
              sendkeycommand(KEY_RETURN);
            }else{
                if (indexOfCharacter(r, index, sequence)>0){  
                    if (sequence==1){         // normal
                        shift=0;
                    }else if (sequence==2){   // shift
                        shift=1;
                    }else if (sequence==3){   // altgr  
                        ctrl=1, alt=1;

                    // à partir de là, traitement des dead keys ^ et ¨
                    }else if (sequence==4){   // circomflexe
                        SendKey(47, 0, 0, 0);
                       
                    }else if (sequence==5){  // circomflexe + shift caractère                   
                        SendKey(47, 0, 0, 0);
                        shift=1;

                    }else if (sequence==6){  // tréma     
                        SendKey(47, 0, 0, 1); 

                    }else if (sequence==7){  // tréma + shift caractère
                        SendKey(47, 0, 0, 1); 
                        shift=1;  
                    }
                    
                    if (sequence<4){
                        //c=scancode[index];
                        c=pgm_read_byte_near(scancode+index);
                        SendKey(c, ctrl, alt, shift); 
                    }else{
                        //c=keyDead[index];
                        c=pgm_read_byte_near(keyDead+index);
                        SendKey(c, ctrl, alt, shift);          
                    }
                }else{
                  if (align>1){
                    for (byte j=0; j<align; j++){
                      c=Texte.charAt(indice+j);
                      specialkey=(specialkey<<8);
                      specialkey+=c;
                    }
                    altspecialkey(specialkey);
                  }
                }
            }   
        }  
    }       
}


void sendtext(const String &Texte){
  if (Texte.length()<255){
    decodeUTF8(Texte);
  }
}

void sendtextln(const String &Texte){
  if (Texte.length()<255){
    decodeUTF8(Texte);
    sendkeycommand(KEY_RETURN);
  }
}


void homekey(){
    sendkeycommand(KEY_HOME);
}
void endkey(){
    sendkeycommand(KEY_END);
}
void leftkey(byte nb){
  for (byte i=0; i<nb; i++){
    sendkeycommand(KEY_LEFT_ARROW);
  }
}

void rightkey(byte nb){
  for (byte i=0; i<nb; i++){
    sendkeycommand(KEY_RIGHT_ARROW);
  }
}
void upkey(byte nb){
  for (byte i=0; i<nb; i++){
    sendkeycommand(KEY_UP_ARROW);
  }
}
void downkey(byte nb){
  for (byte i=0; i<nb; i++){
    sendkeycommand(KEY_DOWN_ARROW);
  }
}

void returnkey(byte nb){
  for (byte i=0; i<nb; i++){
    sendkeycommand(KEY_RETURN);
  }
}

void backspacekey(byte nb){
  for (byte i=0; i<nb; i++){
    sendkeycommand(KEY_BACKSPACE);
  }
}

void deletekey(byte nb){
  for (byte i=0; i<nb; i++){
    sendkeycommand(KEY_DELETE);
  }
}

void tabkey(byte nb){
  for (byte i=0; i<nb; i++){
    sendkeycommand(KEY_TAB);
  }
}

void sendkeycombi(byte key1, char key){
  String keys;
  keys+=key;
  Keyboard.press(key1);
  delay(5);
  sendtext(keys);
  delay(500);
}


void selectall(){
  sendkeycombi(KEY_LEFT_CTRL,'a');
}
void cut(){
  sendkeycombi(KEY_LEFT_CTRL, 'x');
}
void copy(){
  sendkeycombi(KEY_LEFT_CTRL, 'c');
}
void paste(){
  sendkeycombi(KEY_LEFT_CTRL, 'v');
}
