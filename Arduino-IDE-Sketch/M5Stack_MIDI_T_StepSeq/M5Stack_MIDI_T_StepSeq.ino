#include <M5Stack.h>
#include <MIDI.h>
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);

  int i = 0;
  int j;

  //ノートナンバーをノート名に変換するための配列
  char noteNum[128][5] = {"C-2 ", "C-2#", "D-2 ", "D-2#", "E-2 ", "F-2 ", "F-2#", "G-2 ", "G-2#", "A-2 ", "A-2#", "B-2 ", "C-1 ", "C-1#", "D-1 ", "D-1#", "E-1 ", "F-1 ", "F-1#", "G-1 ", "G-1#", "A-1 ", "A-1#", "B-1 ", "C0  ", "C0# ", "D0  ", "D0# ", "E0  ", "F0  ", "F0# ", "G0  ", "G0# ", "A0  ", "A0# ", "B0  ", "C1  ", "C1# ", "D1  ", "D1# ", "E1  ", "F1  ", "F1# ", "G1  ", "G1# ", "A1  ", "A1# ", "B1  ", "C2  ", "C2# ", "D2  ", "D2# ", "E2  ", "F2  ", "F2# ", "G2  ", "G2# ", "A2  ", "A2# ", "B2  ", "C3  ", "C3# ", "D3  ", "D3# ", "E3  ", "F3  ", "F3# ", "G3  ", "G3# ", "A3  ", "A3# ", "B3  ", "C4  ", "C4# ", "D4  ", "D4# ", "E4  ", "F4  ", "F4# ", "G4  ", "G4# ", "A4  ", "A4# ", "B4  ", "C5  ", "C5# ", "D5  ", "D5# ", "E5  ", "F5  ", "F5# ", "G5  ", "G5# ", "A5  ", "A5# ", "B5  ", "C6  ", "C6# ", "D6  ", "D6# ", "E6  ", "F6  ", "F6# ", "G6  ", "G6# ", "A6  ", "A6# ", "B6  ", "C7  ", "C7# ", "D7  ", "D7# ", "E7  ", "F7  ", "F7# ", "G7  ", "G7# ", "A7  ", "A7# ", "B7  ", "C8  ", "C8# ", "D8  ", "D8# ", "E8  ", "F8  ", "F8# ", "G8  "};
  
void setup() {
  M5.begin();
  dacWrite(25, 0); // Speaker OFF
  MIDI.begin();
  MIDI.turnThruOff();
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print("MIDI Stp Seq.");
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(0, 25);
  M5.Lcd.print("-----------------------------------------------------");
  M5.Lcd.setCursor(0, 60);
  M5.Lcd.print("Note Number:");
  M5.Lcd.setCursor(0, 90);
  M5.Lcd.print("-----------------------------------------------------");
}

void loop() {

  //シーケンスのパターン数
  int ptnNum = 4;

  //シーケンスの最大STEP数
  int arrayNum = 8;

  //シーケンスデータ
  int stepArray[ptnNum][arrayNum] = {
    {49, 51, 54, 56, 58, 61, 63, 66},
    {58, 79, 93, 90, 52, 66, 82, 99},
    {48, 48, 60, 80, 48, 60, 48, 60},
    {50, 49, 48, 57, 80, 75, 42, 69}
  };

  for (j = 0; j < arrayNum; j++) {
    //現在再生されているパターンの表示
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor(0, 40);
    M5.Lcd.print("Selected Ptn:");
    M5.Lcd.print(i + 1);
    
    //ノートオン送信
    MIDI.sendNoteOn(stepArray[i][j], 127, 1);    // Send a Note (pitch 0, velo 127 on channel 1)

    //ノートナンバーの表示
    M5.Lcd.setTextSize(1);
    M5.Lcd.setCursor((j * 41), 70);
//    M5.Lcd.print("   ");
    M5.Lcd.fillRect((j * 41), 70, 40, 10, BLACK);
    M5.Lcd.print(noteNum[stepArray[i][j]]);

    //疑似ピアノロールの表示クリア
    M5.Lcd.fillRect((j * 41) + 5, 100, 40, 150, BLACK);
   //疑似ピアノロールの表示
    M5.Lcd.setCursor((j * 41) + 5, 240 - (stepArray[i][j]));
    String str = noteNum[stepArray[i][j]];

   //黒鍵は青で表示
    if(str.indexOf("#") != -1) {
      M5.Lcd.setTextColor(BLUE);
      M5.Lcd.print("---");
    } else {
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.print("---");
    }
    
    //再生位置の表示
    M5.Lcd.setCursor((j * 41), 80);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.print(">>>");
    delay(100);                // Wait for a second

    //パターン切り替え
    if (M5.BtnA.wasPressed()) {
      if (i > 0) {
        i = i - 1;
        M5.Lcd.fillRect(0, 40, 320, 10, BLACK);
      }
    }  
    if (M5.BtnB.wasPressed()) {
      if (i < 3) {
        i = i + 1;
        M5.Lcd.fillRect(0, 40, 320, 10, BLACK);
      }
    }

    M5.update();

   //ノートオフ送信
   MIDI.sendNoteOff(stepArray[i][j], 0, 1);

    //再生位置のクリア
    M5.Lcd.fillRect((j * 41), 80, 40, 10, BLACK);
  }
}
