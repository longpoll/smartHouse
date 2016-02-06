
#define LENGTH_PASSWORD 4

int PinOut[4] {5, 4, 3, 2}; // пины выходы
int PinIn[4] {9, 8, 7, 6}; // пины входа

int val = 0;
int userPassword[LENGTH_PASSWORD], k = 0, q = 0, isOff = 0; // userPassword = массив ведённых символов; k = кол-во ведённых символов; isOff = включения сигнализации
const char value[4][4]
  { {'1', '4', '7', '*'},
    {'2', '5', '8', '0' },
    {'3', '6', '9', '#'},
    {'A', 'B', 'C', 'D'}
  };
// двойной массив, обозначающий кнопку
 
int b = 0; // переменная, куда кладется число из массива(номер кнопки)
 
void setup() {
  pinMode (2, OUTPUT); // инициализируем порты на выход (подают нули на столбцы)
  pinMode (3, OUTPUT);
  pinMode (4, OUTPUT);
  pinMode (5, OUTPUT);
 
  pinMode (6, INPUT); // инициализируем порты на вход с подтяжкой к плюсу (принимают нули на строках)
  pinMode (7, INPUT);
  pinMode (8, INPUT);
  pinMode (9, INPUT);

  pinMode(11, OUTPUT);
  
  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);

 
  Serial.begin(9600); // открываем Serial порт
}
 
void matrix () { // создаем функцию для чтения кнопок
  int realPassword[] {49, 51, 51, 55}; // верный пароль: 1337
  int currentButton; // текущая нажатая кнопка
  for (int i = 1; i <= 4; i++) { // цикл, передающий 0 по всем столбцам
    digitalWrite(PinOut[i - 1], LOW); // если i меньше 4 , то отправляем 0 на ножку
    for (int j = 1; j <= 4; j++) { // цикл, принимающих 0 по строкам
      if (digitalRead(PinIn[j - 1]) == LOW) { // если один из указанных портов входа равен 0, то..
        currentButton = value[i - 1][j - 1];
        tone(10, 500, 100);
        delay(100);
        switch( currentButton ) {
          case '#':
            /* локальный велосипед. Сравниваем массивы */
            for ( int i = 0, l = LENGTH_PASSWORD; i < l; i++ ) {
              if ( realPassword[i] == userPassword[i] ) { 
                q++;  // чтобы проверить, сколько совпадений в массивах
              }
              userPassword[i] = 0; // сразу же обнулим, чтобы можно было повторно вводить пароль
            }
            if ( q == LENGTH_PASSWORD ){
              isOff = 1;
              for ( int i = 0, l = LENGTH_PASSWORD; i < l; i++ ) {
                userPassword[i] = 0;
              }
              k = -1;
              q = 0; // сбрасываем счётчик верных
              tone(10, 500, 10);
            } else {
              tone(10, 6000, 1000);
              delay(1100);
              tone(10, 6000, 1000);
              delay(1000);
              k = -1; // потому что потом сразу инкремент идёт   
            }
          break;
          
          case '*':
            isOff = 0;
            for ( int i = 0, l = LENGTH_PASSWORD; i < l; i++ ) {
              userPassword[i] = 0;
            }
            k = -1;
          break;
          
          default:
            userPassword[k] = currentButton;
        }
        /*if ( currentButton != '#' && currentButton != '*' && currentButton ) {
          userPassword[k] = currentButton;
        } else if ( currentButton == '*' ) {
          isOff = 0;
          for ( int i = 0, l = LENGTH_PASSWORD; i < l; i++ ) {
            userPassword[i] = 0; 
          }
        } else {
          // локальный велосипед. Сравниваем массивы 
          for ( int i = 0, l = LENGTH_PASSWORD; i < l; i++ ) {
            if ( realPassword[i] == userPassword[i] ) { 
              q++;  // чтобы проверить, сколько совпадений в массивах
            }
            Serial.println(i);
            userPassword[i] = 0; // сразу же обнулим, чтобы можно было повторно вводить пароль
          }
          if ( q == LENGTH_PASSWORD ){
            isOff = 1;
            q = 0; // сбрасываем счётчик верных
            tone(10, 500, 10);
          } else {
            tone(10, 6000, 1000);
            delay(1000);
            delay(100);
            tone(10, 6000, 1000);
            delay(1000);
            k = -1; // потому что потом сразу инкремент идёт   
          }
        }*/
        if ( k >= LENGTH_PASSWORD && isOff == 0 && q != LENGTH_PASSWORD) {  // если введён слишком длинный пароль
          tone(10, 1000, 500);
          Serial.println("Very big!");
        } else { 
          k++;
        }
        delay(500);
        Serial.println(isOff);
      }
    }
    digitalWrite(PinOut[i - 1], HIGH); // подаём обратно высокий уровень
  }
  digitalWrite(11, isOff);
}
 
void loop() {
  matrix(); // используем функцию опроса матричной клавиатуры
}
