const int flipper_1 = 9;
const int flipper_2 = 10;
const int drukknop_1 = 50;
const int drukknop_2 = 51;

bool stateF1 = LOW;
bool stateF2 = LOW;

unsigned long previousMillisF1 = 0;
unsigned long previousMillisF2 = 0;
unsigned long currentMillis = 0;
const int timeBetweenUses = 1000; // een halve seconde tussen elke flipperactie



void setup()
{
  Serial.begin(9600);
  pinMode(flipper_1, INPUT);
  pinMode(flipper_2, INPUT);
}

void loop()
{
  //
  //
  //
  //
  //
  //
  currentMillis = millis();
  if (currentMillis - previousMillisF1 >= timeBetweenUses)
  {
    stateF1 = LOW;
    //  digitalWrite(flipper_1, LOW);
   //Serial.println("Flipper 1 kan weer ingedrukt worden.");
    delay(1);
  }
  currentMillis = millis();
  if (currentMillis - previousMillisF2 >= timeBetweenUses)
  {
    stateF2 = LOW;
    //  digitalWrite(flipper_2, LOW);
    //Serial.println("Flipper 2 kan weer ingedrukt worden.");
    delay(1);
  }
  if (digitalRead(drukknop_1) == HIGH and stateF1 == LOW)
  {
    stateF1 = HIGH;
    previousMillisF1 = millis();
    // digitalWrite(flipper_1, HIGH);
    Serial.println("Flipper 1 ingedrukt.");
    delay(1);
  }
  if (digitalRead(drukknop_2) == HIGH and stateF2 == LOW)
  {
    stateF2 = HIGH;
    previousMillisF2 = millis();
    // digitalWrite(flipper_2, HIGH);
    Serial.println("Flipper 2 ingedrukt.");
    delay(1);
  }

}
