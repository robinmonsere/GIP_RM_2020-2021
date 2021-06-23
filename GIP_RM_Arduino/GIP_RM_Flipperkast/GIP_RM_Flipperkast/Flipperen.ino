void FLIPPEREN()
{
  if (digitalRead(drukknop_1) == HIGH and stateF1 == LOW and isUsableF1 == true)
  {
    stateF1 = HIGH;
    previousMillisF1 = millis();
    digitalWrite(flipper_1, HIGH);
    Serial.println("Flipper 1 ingedrukt.");
    delay(1);
  }
  if (digitalRead(drukknop_1) == LOW and stateF1 == HIGH)
  {
    digitalWrite(flipper_1, LOW);
    Serial.println("flipper 1 is laag (drukknop)");
    millisHTL1 = millis();
    isUsableF1 = false;
    stateF1 = LOW;
    delay(1);
  }
  currentMillis = millis();
  if (currentMillis - previousMillisF1 >= timerFlipper and stateF1 == HIGH)
  {
    digitalWrite(flipper_1, LOW);
    Serial.println("Flipper 1 is laag (timer)");
    millisHTL1 = millis();
    isUsableF1 = false;
    stateF1 = LOW;
    delay(1);
  }
  currentMillis = millis();
  if (currentMillis - millisHTL1 >= timeBetweenUses and stateF1 == LOW)
  {
    isUsableF1 = true;
    Serial.println("flipper 1 kan gebruikt worden.");
  }
  // Flipper 2
  if (digitalRead(drukknop_2) == HIGH and stateF2 == LOW and isUsableF2 == true)
  {
    stateF2 = HIGH;
    previousMillisF2 = millis();
    digitalWrite(flipper_2, HIGH);
    Serial.println("Flipper 2 ingedrukt.");
    delay(1);
  }
  if (digitalRead(drukknop_2) == LOW and stateF2 == HIGH)
  {
    //digitalWrite(flipper_2, LOW);
    Serial.println("flipper 2 is laag (drukknop)");
    millisHTL2 = millis();
    isUsableF2 = false;
    stateF2 = LOW;
    delay(1);
  }
  currentMillis = millis();
  if (currentMillis - previousMillisF2 >= timerFlipper and stateF2 == HIGH)
  {
    digitalWrite(flipper_1, LOW);
    Serial.println("Flipper 1 is laag (timer)");
    millisHTL2 = millis();
    isUsableF2 = false;
    stateF2 = LOW;
    delay(1);
  }
  currentMillis = millis();
  if (currentMillis - millisHTL2 >= timeBetweenUses and stateF2 == LOW)
  {
    isUsableF2 = true;
    Serial.println("flipper 2 kan gebruikt worden.");
  }
  // add if voor een sensor.
  if (digitalRead(joystickSW == LOW))
  {
    digitalWrite(flipper_1, LOW);
    digitalWrite(flipper_2, LOW);
  }
}
