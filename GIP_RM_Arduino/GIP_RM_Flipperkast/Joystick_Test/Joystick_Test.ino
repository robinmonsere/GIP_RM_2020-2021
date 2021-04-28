#define joyX A0
#define joyY A1
int xValue;
int yValue;
byte state = 0;
unsigned long c_Millis = 0;
unsigned long c_PreviousMillis;
void setup() {
  Serial.begin(9600);
}

void loop() {
  yValue = map(analogRead(joyY), 0, 1023, 0, 100);
  Serial.println(yValue);
  delay(1000);
/*  c_Millis = millis();
  if (c_Millis - c_PreviousMillis >= 300)
  {
    if (yValue >= 55 and state != 0)
    {
      state--;
      Serial.println(state);
      c_PreviousMillis = millis();
    }
    if (yValue <= 45 and state != 10)
    {
      state++;
      Serial.println(state);
      c_PreviousMillis = millis();
    }
    delay(100);
  }
  */
}

