/*
        @file   Othello.ino
        @brief  オセロ
        @author CaseyNelson314
        @date   2021/8/31
*/
const int row   [8] = {A8,  A9, A10, A11, A12, A13, A14, A15}; //Vcc
const int r_line[8] = {24,  25,  26,  27,  28,  29,  30,  31};//GND red
const int g_line[8] = {32,  33,  34,  35,  36,  37,  38,  39};//GND green
const int R_led    = 50; const int L_led    = 51;
const int R_up     = 41; const int L_up     = 47;
const int R_right  = 44; const int L_right  = 48;
const int R_down   = 42; const int L_down   = 45;
const int R_left   = 43; const int L_left   = 46;
const int R_point  = 40; const int L_point  = 49;
const int sw_HINT1 = 52; const int sw_HINT2 = 53;
const int sw_RESET = 22; const int buzzer   = 3;//23
const int sw_R_AI  = A6; const int sw_L_AI  = A7;
long Time;        long Timesub;       long AItime;
byte Mode;
byte Turn;
byte skipcount;
int  delaytime , cursordelay;
byte x; byte y;
bool firstsetup;
bool cursorClock;
bool AIclock;
byte total   [8][8] = {};
byte R_canput[8][8] = {};
int  R_putData;   bool R_setupData;
bool R_upData;    bool R_newNumber_up;    bool R_oldNumber_up;
bool R_rightData; bool R_newNumber_right; bool R_oldNumber_right;
bool R_downData;  bool R_newNumber_down;  bool R_oldNumber_down;
bool R_leftData;  bool R_newNumber_left;  bool R_oldNumber_left;
bool R_pointData; bool R_newNumber_point; bool R_oldNumber_point;
byte R_finish;

byte L_canput[8][8] = {};
int  L_putData;   bool L_setupData;
bool L_upData;    bool L_newNumber_up;    bool L_oldNumber_up;
bool L_rightData; bool L_newNumber_right; bool L_oldNumber_right;
bool L_downData;  bool L_newNumber_down;  bool L_oldNumber_down;
bool L_leftData;  bool L_newNumber_left;  bool L_oldNumber_left;
bool L_pointData; bool L_newNumber_point; bool L_oldNumber_point;
byte L_finish;

bool finishData;

byte Print[8][8] = {};                                    //オセロ位置記憶層
void setup() {
  Serial.begin(115200);
  for ( int i = 0; i < 8; i++ ) {
    pinMode( row[i], OUTPUT );
    digitalWrite( row[i], LOW );
  }
  for ( int i = 0; i < 8; i++ ) {
    pinMode( r_line[i], OUTPUT );
    digitalWrite( r_line[i], HIGH );
  }
  for ( int i = 0; i < 8; i++ ) {
    pinMode( g_line[i], OUTPUT );
    digitalWrite( g_line[i], HIGH );
  }
  pinMode(R_up,     INPUT_PULLUP);  pinMode(L_up,    INPUT_PULLUP);
  pinMode(R_right,  INPUT_PULLUP);  pinMode(L_right,  INPUT_PULLUP);
  pinMode(R_down,   INPUT_PULLUP);  pinMode(L_down,   INPUT_PULLUP);
  pinMode(R_left,   INPUT_PULLUP);  pinMode(L_left,   INPUT_PULLUP);
  pinMode(R_point,  INPUT_PULLUP);  pinMode(L_point,  INPUT_PULLUP);
  pinMode(sw_HINT1, INPUT_PULLUP);  pinMode(sw_HINT2, INPUT_PULLUP);
  pinMode(sw_R_AI,  INPUT_PULLUP);  pinMode(sw_L_AI,  INPUT_PULLUP);
  pinMode(R_led,    OUTPUT      );  pinMode(L_led,    OUTPUT      );
  pinMode(sw_RESET, INPUT_PULLUP);  pinMode(buzzer,   OUTPUT      );
  randomSeed(analogRead(0));
}

void loop() {
  Time = millis();
  cursordelay = 300;  //カーソル点滅周期
  if (Time - Timesub > cursordelay) {
    Timesub = Time;
    cursorClock = cursorClock == 1 ? 0 : 1;
  }
  if (!digitalRead(sw_R_AI) && !digitalRead(sw_L_AI)) {
    delaytime = 300;
  } else {
    delaytime = 1000;
  }
  if (Time - AItime > delaytime) {
    AItime = Time;
    AIclock = AIclock == 1 ? 0 : 1;
  }

  byte Cursor  [8][8] = {};
  if (!digitalRead(sw_RESET)) {
    Mode = 0;

  }
  switch (Mode) {
    case 0://初期設定
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          total[i][j] = 0;
          Print[i][j] = 0;
          total[i][j] = 0;
          R_canput[i][j] = 0;
          L_canput[i][j] = 0;
          R_finish = 0;
          L_finish = 0;
        }
      }
      Turn = random(2);//先攻決め0~1生成 (2):(0,2)の略
      Print[3][3] = 2;
      Print[3][4] = 1;
      Print[4][3] = 1;
      Print[4][4] = 2;
      Mode = 1; //オセロプログラムへ
      finishData = 0; //1:finish一回実行する
      R_setupData = 0;
      L_setupData = 0;
      break;

    case 1://オセロ
      firstsetup = 0;
      byte x_t; byte y_t;
      switch (Turn) {

        //赤側
        case 0:
          if (R_setupData == 0) {             //順が来るたび一回実行
            digitalWrite(R_led, LOW);
            digitalWrite(L_led, LOW);
            Serial.println("R");
            R_putData = 0;
            L_putData = 0;
            x = 7; y = 7;
            digitalWrite(R_led, HIGH);
            digitalWrite(L_led, LOW );
            for (int y_t = 0; y_t < 8; y_t++) {
              for (int x_t = 0; x_t < 8; x_t++) {
                R_putData = R_putData + test(y_t, x_t, 0, 0);
                Serial.print(test(y_t, x_t, 0, 0)); Serial.print("  ");
              }
              Serial.println();
            }
            Serial.println();
            if (R_putData == 0) { //スキップ
              Turn = 1;
            }
            AItime = Time;
            AIclock = 0;
            R_setupData = 1;
            L_setupData = 0;
          }
          if (!digitalRead(sw_R_AI) && R_putData > 0) {        //AIswHIGH&&置けるとき
            if (AIclock) {
              AI(0);
            }
          }
          if (!digitalRead(sw_R_AI) == LOW) {               //AIswitchLow

            R_newNumber_up    = !digitalRead(R_up);
            R_upData    = R_newNumber_up    == 1 && R_oldNumber_up    == 0 ? 1 : 0;     //click
            R_oldNumber_up    = R_newNumber_up;
            R_newNumber_right = !digitalRead(R_right);
            R_rightData = R_newNumber_right == 1 && R_oldNumber_right == 0 ? 1 : 0;
            R_oldNumber_right = R_newNumber_right;
            R_newNumber_down  = !digitalRead(R_down);
            R_downData  = R_newNumber_down  == 1 && R_oldNumber_down  == 0 ? 1 : 0;
            R_oldNumber_down  = R_newNumber_down;
            R_newNumber_left  = !digitalRead(R_left);
            R_leftData  = R_newNumber_left  == 1 && R_oldNumber_left  == 0 ? 1 : 0;
            R_oldNumber_left  = R_newNumber_left;
            R_newNumber_point = !digitalRead(R_point);
            R_pointData = R_newNumber_point == 1 && R_oldNumber_point == 0 ? 1 : 0;
            R_oldNumber_point = R_newNumber_point;
            //カーソル
            if (R_upData    && x > 0) {
              x--; tone(buzzer, 1000, 100);
            }
            if (R_rightData && y < 7) {
              y++; tone(buzzer, 1000, 100);
            }
            if (R_downData  && x < 7) {
              x++; tone(buzzer, 1000, 100);
            }
            if (R_leftData  && y > 0) {
              y--; tone(buzzer, 1000, 100);
            }

            if (R_pointData && test(x, y, 0, 0) > 0) { //決定ボタン押され、反転できるとき
              test(x, y, 0, 1);
              Print[x][y] = 1;                         //反転
              tone(buzzer, 2000, 100);
              Turn = 1;                                //順番変更
            }

            if (Print[x][y] == 1) {                    //カーソルとオセロ盤の色が重なったとき→オレンジ
              Cursor[x][y] = 2;
            } else {
              Cursor[x][y] = 1;
            }
          }
          if (R_putData == 0) {
            for (int y_t = 0; y_t < 8; y_t++) {
              for (int x_t = 0; x_t < 8; x_t++) {
                L_putData = L_putData + test(y_t, x_t, 1, 0);          //置けるところがない→０
                Serial.print(test(y_t, x_t, 1, 0)); Serial.print("  ");
              }
              Serial.println();
            }
            Serial.println();
            if (L_putData == 0) {
              Mode = 2;
            }
          }
          break;

        //緑側
        case 1:

          if (L_setupData == 0) {                    //順が来るたび一回実行
            Serial.println("L");
            R_putData = 0;
            L_putData = 0;
            x = 0; y = 0;
            digitalWrite(R_led, LOW );
            digitalWrite(L_led, HIGH);
            for (int y_t = 0; y_t < 8; y_t++) {
              for (int x_t = 0; x_t < 8; x_t++) {
                L_putData = L_putData + test(y_t, x_t, 1, 0);          //置けるところがない→０
                Serial.print(test(y_t, x_t, 1, 0)); Serial.print("  ");
              }
              Serial.println();
            }
            Serial.println();

            if (L_putData == 0) {
              Turn = 0;
            }
            AItime = Time;
            AIclock = 0;
            L_setupData = 1;
            R_setupData = 0;
          }
          if (!digitalRead(sw_L_AI) == HIGH && L_putData > 0) {            //AI
            if (AIclock) {
              AI(1);
            }
          }
          if (!digitalRead(sw_L_AI) == LOW) {                 //AIswitchLow

            L_newNumber_up    = !digitalRead(L_up);
            L_upData    = L_newNumber_up    == 1 && L_oldNumber_up    == 0 ? 1 : 0;     //click
            L_oldNumber_up    = L_newNumber_up;
            L_newNumber_right = !digitalRead(L_right);
            L_rightData = L_newNumber_right == 1 && L_oldNumber_right == 0 ? 1 : 0;
            L_oldNumber_right = L_newNumber_right;
            L_newNumber_down  = !digitalRead(L_down);
            L_downData  = L_newNumber_down  == 1 && L_oldNumber_down  == 0 ? 1 : 0;
            L_oldNumber_down  = L_newNumber_down;
            L_newNumber_left  = !digitalRead(L_left);
            L_leftData  = L_newNumber_left  == 1 && L_oldNumber_left  == 0 ? 1 : 0;
            L_oldNumber_left  = L_newNumber_left;
            L_newNumber_point = !digitalRead(L_point);
            L_pointData = L_newNumber_point == 1 && L_oldNumber_point == 0 ? 1 : 0;
            L_oldNumber_point = L_newNumber_point;
            //カーソル
            if (L_upData    && x < 7) {
              x++; tone(buzzer, 1000, 100);
            }
            if (L_rightData && y > 0) {
              y--; tone(buzzer, 1000, 100);
            }
            if (L_downData  && x > 0) {
              x--; tone(buzzer, 1000, 100);
            }
            if (L_leftData  && y < 7) {
              y++; tone(buzzer, 1000, 100);
            }
            if (L_pointData && test(x, y, 1, 0) > 0) { //決定ボタン押され、反転できるとき
              test(x, y, 1, 1);
              Print[x][y] = 2;
              tone(buzzer, 2000, 100);
              Turn = 0;                              //順番変更
            }
            if (Print[x][y] == 2) {                    //カーソルとオセロ盤の色が重なったとき→オレンジ
              Cursor[x][y] = 1;
            } else {
              Cursor[x][y] = 2;
            }
          }
          if (L_putData == 0) {
            for (int y_t = 0; y_t < 8; y_t++) {
              for (int x_t = 0; x_t < 8; x_t++) {
                R_putData = R_putData + test(y_t, x_t, 0, 0);
                Serial.print(test(y_t, x_t, 0, 0)); Serial.print("  ");
              }
              Serial.println();
            }
            Serial.println();
            if (R_putData == 0) {
              Mode = 2;
            }
          }
      }
      break;

    case 2://結果表示
      if (finishData == 0) {                    //setup
        digitalWrite(R_led, LOW);
        digitalWrite(L_led, LOW);
        //赤の合計コマ数
        for (int y_t = 0; y_t < 8; y_t++) {
          for (int x_t = 0; x_t < 8; x_t++) {
            if (Print[y_t][x_t] == 1) {
              R_finish++;
            }
          }
        }
        //緑の合計コマ数
        for (int y_t = 0; y_t < 8; y_t++) {
          for (int x_t = 0; x_t < 8; x_t++) {
            if (Print[y_t][x_t] == 2) {
              L_finish++;
            }
          }
        }
        if (L_finish > R_finish) {  //Lwin
          digitalWrite(L_led, HIGH);
        }
        if (L_finish < R_finish) {  //Rwin
          digitalWrite(R_led, HIGH);
        }
        if (L_finish == R_finish) { //draw
          digitalWrite(L_led, HIGH);
          digitalWrite(R_led, HIGH);
        }
      }
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          if (R_finish > 0) {
            R_finish--;
            total[7 - i][7 - j] = 1;
          }
        }
      }
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          if (L_finish > 0) {
            L_finish--;
            total[i][j] = 2;
          }
        }
      }
      finishData = 1;
      //Mode = 0;
      break;

  }


  //------------------------matrix--------------------------//
  //赤
  for ( int i = 0; i < 8; i++ ) {            //縦
    digitalWrite( r_line[i], LOW );
    for ( int j = 0; j < 8; j++ ) {          //横
      switch (Mode) {
        case 1:
          if (!digitalRead(sw_HINT1) || !digitalRead(sw_HINT2)) {
            if (test(i, j, 0, 0) > 0 && Turn == 0) {
              digitalWrite( row[j], cursorClock);
            }
          }  if (Print[i][j] == 1 || Cursor[i][j] == 1) {
            digitalWrite( row[j], HIGH );
          }
          break;

        case 2:
          if (total[i][j] == 1) {
            digitalWrite( row[j], HIGH );
          }
      }
    }
    delayMicroseconds(100);
    for ( int j = 0; j < 8; j++ ) {
      digitalWrite( row[j], LOW );
    }
    digitalWrite( r_line[i], HIGH );
  }
  //緑
  for ( int i = 0; i < 8; i++ ) {            //縦
    digitalWrite( g_line[i], LOW );
    for ( int j = 0; j < 8; j++ ) {          //横
      switch (Mode) {
        case 1:
          if (!digitalRead(sw_HINT1) || !digitalRead(sw_HINT2)) {
            if (test(i, j, 1, 0) > 0 && Turn == 1) {
              digitalWrite( row[j], cursorClock );
            }
          } if (Print[i][j] == 2 || Cursor[i][j] == 2 ) {
            digitalWrite( row[j], HIGH );
          }
          break;

        case 2:
          if (total[i][j] == 2) {
            digitalWrite( row[j], HIGH );
          }
      }
    }
    delayMicroseconds(100);
    for ( int j = 0; j < 8; j++ ) {
      digitalWrite( row[j], LOW );
    }
    digitalWrite( g_line[i], HIGH );
  }
}
//------------------------matrix--------------------------//



byte  test (byte x_test, byte y_test, bool collor , bool change) {
  //collor=0:赤(R) 1:緑(L)   change:0=反転処理なし 1:反転処理あり
  byte plus;
  byte canput1 = 0;  byte canputsub1 = 0;
  byte canput2 = 0;  byte canputsub2 = 0;
  byte canput3 = 0;  byte canputsub3 = 0;
  byte canput4 = 0;  byte canputsub4 = 0;
  byte canput5 = 0;  byte canputsub5 = 0;
  byte canput6 = 0;  byte canputsub6 = 0;
  byte canput7 = 0;  byte canputsub7 = 0;
  byte canput8 = 0;  byte canputsub8 = 0;
  byte canput  = 0;  bool cannotput  = 0;
  byte collor_true;  byte collor_false;

  if (Print[x_test][y_test] == 0) {  //置こうとしてるマスに駒がないとき
    if (collor == 0) {               //入力赤
      collor_true = 1;               //Printの赤
      collor_false = 2;              //Printの緑(反対)
    } else {                         //入力緑
      collor_true = 2;               //Printの緑
      collor_false = 1;              //Printの赤(反対)
    }
    //---------------------------８方向検索--------------------------//
    //上
    cannotput = 0; plus = 2; canput1 = 1;
    if (Print[x_test - 1][y_test    ] == collor_false && x_test - 1 >= 0                   ) {  //１マス先にcollorと逆の駒がある
      while (cannotput == 0 && x_test - plus >= 0) {                                            //ループ(反転する、盤からはみ出す、２マス先が０まで)
        if (Print[x_test - plus][y_test       ] == collor_true) {                               //２マス先にcollorと同じ駒→反転
          cannotput = 1; canputsub1 = canput1;                                                  //裏返した枚数を代入whileを抜ける,反転できるコマ数を代入
        }
        if (Print[x_test - plus][y_test       ] == 0) {                                         //２マス先に駒がないwhileを抜ける
          cannotput = 1; canputsub1 = 0;
        }
        if (Print[x_test - plus][y_test       ] == collor_false) {                              //２マス先にcollorと逆の駒→３マス上へwhileでループ
          if (x_test - plus >= 0) {
            plus++; canput1++;
          }
        }
      }
    }
    if (change) {                                                                       //反転
      for (int i = 0; i < canputsub1; i++) {
        Print[x_test - 1 - i][y_test] = collor_true;
      }
    }

    //右上
    cannotput = 0; plus = 2; canput2 = 1;
    if (Print[x_test - 1][y_test + 1] == collor_false && x_test - 1 >= 0 && y_test + 1 <= 8) {
      while (cannotput == 0 && x_test - plus >= 0 && y_test + plus <= 7) {
        if (Print[x_test - plus][y_test + plus] == collor_true) {
          cannotput = 1; canputsub2 = canput2;
        }
        if (Print[x_test - plus][y_test + plus] == 0) {
          cannotput = 1; canputsub2 = 0;
        }
        if (Print[x_test - plus][y_test + plus] == collor_false) {
          if (x_test - plus >= 0 && y_test + plus <= 7) {
            plus++; canput2++;
          }
        }
      }
    }
    if (change) {                                                                       //反転
      for (int i = 0; i < canputsub2; i++) {
        Print[x_test - 1 - i][y_test + 1 + i] = collor_true;
      }
    }

    //右
    cannotput = 0; plus = 2; canput3 = 1;
    if (Print[x_test    ][y_test + 1] == collor_false                    && y_test + 1 <= 8) {
      while (cannotput == 0 && y_test + plus <= 7) {
        if (Print[x_test       ][y_test + plus] == collor_true) {
          cannotput = 1; canputsub3 = canput3;
        }
        if (Print[x_test       ][y_test + plus] == 0) {
          cannotput = 1; canputsub3 = 0;
        }
        if (Print[x_test       ][y_test + plus] == collor_false) {
          if (y_test + plus <= 7) {
            plus++; canput3++;
          }
        }
      }
    }
    if (change) {                                                                       //反転
      for (int i = 0; i < canputsub3; i++) {
        Print[x_test    ][y_test + 1 + i] = collor_true;
      }
    }

    //右下
    cannotput = 0; plus = 2; canput4 = 1;
    if (Print[x_test + 1][y_test + 1] == collor_false && x_test + 1 <= 8 && y_test + 1 <= 8) {
      while (cannotput == 0 && x_test + plus <= 7 && y_test + plus <= 7) {

        if (Print[x_test + plus][y_test + plus] == collor_true) {
          cannotput = 1; canputsub4 = canput4;
        }
        if (Print[x_test + plus][y_test + plus] == 0) {
          cannotput = 1; canputsub4 = 0;
        }
        if (Print[x_test + plus][y_test + plus] == collor_false) {
          if (x_test + plus <= 7 && y_test + plus <= 7) {
            plus++; canput4++;
          }
        }
      }
    }
    if (change) {                                                                       //反転
      for (int i = 0; i < canputsub4; i++) {
        Print[x_test + 1 + i][y_test + 1 + i] = collor_true;
      }
    }

    //下
    cannotput = 0; plus = 2; canput5 = 1;
    if (Print[x_test + 1][y_test    ] == collor_false && x_test + 1 <= 8                   ) {
      while (cannotput == 0 && x_test + plus <= 7) {

        if (Print[x_test + plus][y_test       ] == collor_true) {
          cannotput = 1; canputsub5 = canput5;
        }
        if (Print[x_test + plus][y_test       ] == 0) {
          cannotput = 1; canputsub5 = 0;
        }
        if (Print[x_test + plus][y_test       ] == collor_false) {
          if (x_test + plus <= 7) {
            plus++; canput5++;
          }
        }
      }
    }
    if (change) {                                                                       //反転
      for (int i = 0; i < canputsub5; i++) {
        Print[x_test + 1 + i][y_test    ] = collor_true;
      }
    }

    //左下
    cannotput = 0;    plus = 2; canput6 = 1;
    if (Print[x_test + 1][y_test - 1] == collor_false && x_test + 1 <= 8 && y_test - 1 >= 0) {
      while (cannotput == 0 && x_test + plus <= 7 && y_test - plus >= 0) {

        if (Print[x_test + plus][y_test - plus] == collor_true) {
          cannotput = 1; canputsub6 = canput6;
        }
        if (Print[x_test + plus][y_test - plus] == 0) {
          cannotput = 1; canputsub6 = 0;
        }
        if (Print[x_test + plus][y_test - plus] == collor_false) {
          if (x_test + plus <= 7 && y_test - plus >= 0) {
            plus++; canput6++;
          }
        }
      }
    }
    if (change) {                                                                       //反転
      for (int i = 0; i < canputsub6; i++) {
        Print[x_test + 1 + i][y_test - 1 - i] = collor_true;
      }
    }

    //左
    cannotput = 0; plus = 2; canput7 = 1;
    if (Print[x_test    ][y_test - 1] == collor_false                    && y_test - 1 >= 0) {
      while (cannotput == 0 && y_test - plus >= 0) {

        if (Print[x_test       ][y_test - plus] == collor_true) {
          cannotput = 1; canputsub7 = canput7;
        }
        if (Print[x_test       ][y_test - plus] == 0) {
          cannotput = 1; canputsub7 = 0;
        }
        if (Print[x_test       ][y_test - plus] == collor_false) {
          if (  y_test - plus >= 0) {
            plus++; canput7++;
          }
        }
      }
    }
    if (change) {                                                                       //反転
      for (int i = 0; i < canputsub7; i++) {
        Print[x_test    ][y_test - 1 - i] = collor_true;
      }
    }

    //左上
    cannotput = 0; plus = 2; canput8 = 1;
    if (Print[x_test - 1][y_test - 1] == collor_false && x_test - 1 >= 0 && y_test - 1 >= 0) {
      while (cannotput == 0 && x_test - plus >= 0 && y_test - plus >= 0) {
        if (Print[x_test - plus][y_test - plus] == collor_true) {
          cannotput = 1; canputsub8 = canput8;
        }
        if (Print[x_test - plus][y_test - plus] == 0) {
          cannotput = 1; canputsub8 = 0;
        }
        if (Print[x_test - plus][y_test - plus] == collor_false) {
          plus++; canput8++;
        }
      }
    }
    if (change) {
      for (int i = 0; i < canputsub8; i++) {
        Print[x_test - 1 - i][y_test - 1 - i] = collor_true;
      }
    }
  }
  //---------------------------検索終了---------------------------//
  /*方向ごとに反転できるコマ数をプリント
    Serial.print(canputsub1);
    Serial.print(canputsub2);
    Serial.print(canputsub3);
    Serial.print(canputsub4);
    Serial.print(canputsub5);
    Serial.print(canputsub6);
    Serial.print(canputsub7);
    Serial.print(canputsub8); */
  canput = canputsub1 + canputsub2 + canputsub3 + canputsub4 + canputsub5 + canputsub6 + canputsub7 + canputsub8; //反転できる数の計
  return canput;    //返り値:入力座標に置いたとき反転できるコマ数
}



void AI(bool AIcollor) {  //計算する色を指定
  byte MAX_0 = 0;
  byte MAXcount_0 = 0;
  byte randomcount = 0;

  byte MAX_1 = 0;
  byte MAXcount_1 = 0;
  bool latch = 0;
  Serial.println("AI");

  byte priority[8][8] {   //優先順位決め
    {9, 2, 5, 6, 6, 5, 2, 9} ,
    {2, 1, 3, 4, 4, 3, 1, 2} ,
    {5, 3, 8, 7, 7, 8, 3, 5} ,
    {6, 4, 7, 0, 0, 7, 4, 6} ,
    {6, 4, 7, 0, 0, 7, 4, 6} ,
    {5, 3, 8, 7, 7, 8, 3, 5} ,
    {2, 1, 3, 4, 4, 3, 1, 2} ,
    {9, 2, 5, 6, 6, 5, 2, 9} ,
  };

  for (int i = 0; i < 8; i++) {//優先順位の最大数:MAX_0
    for (int j = 0; j < 8; j++) {
      if (MAX_0 < priority[i][j]) {
        MAX_0 = priority[i][j];
      }
    }
  }

  while (MAX_0 >= 0 && latch == 0) { //優先順位の高い順に計算
    for (int i = 0; i < 8; i++) {//優先して置くところで一番多くとれる個数
      for (int j = 0; j < 8; j++) {
        if (priority[i][j] == MAX_0) { //i,j,が優先される座標にある時、
          if (MAX_1 < test(i, j, AIcollor, 0)) { //反転できる最大値検索
            MAX_1 = test(i, j, AIcollor, 0);
          }
        }
      }
    }

    for (int i = 0; i < 8; i++) {//優先して置くところで一番多くとれる場所が何個あるか
      for (int j = 0; j < 8; j++) {
        if (priority[i][j] == MAX_0) {
          if (MAX_1 == test(i, j, AIcollor, 0) && MAX_1 > 0 && test(i, j, AIcollor, 0) > 0) {
            MAXcount_1++;
          }
        }
      }
    }
    Serial.print("優先順位の最大数= ");
    Serial.println(MAX_0);
    Serial.print("その優先度の順で裏返せる最大値");
    Serial.println(MAX_1);
    Serial.print("の数");
    Serial.println(MAXcount_1);
    Serial.print("MAXcount = ");
    Serial.println(MAXcount_1);
    randomcount = random(0, MAXcount_1 + 1);

    Serial.print("ランダム = ");
    Serial.println(randomcount);
    for (int i = 0; i < 8; i++) {//ランダムに算出した場所におく
      for (int j = 0; j < 8; j++) {
        if (priority[i][j] == MAX_0 && MAXcount_1 > 0) {
          if (randomcount == 0 && latch == 0 && test(i, j, AIcollor, 0) > 0)  {
            test(i, j, AIcollor, 1);
            Print[i][j] = AIcollor + 1;
            latch = 1;
            tone(buzzer,2000,100);
          }
          if (randomcount > 0 && latch == 0) {
            randomcount--;
          }
        }
      }
    }
    Serial.println(latch);
    MAX_0--;
    if (MAX_0 == 0) {
      latch = 1;
    }
    MAX_1 = 0;
    MAXcount_1 = 0;
    randomcount = 0;
  }


  Turn = !AIcollor;
}
