void setup() {
  Serial.begin(9600);
  int Kt;
  String t = "50";
  String Klassement_Punten_BR[11] = {"10", "5", "8", "10", "20", "9", "7" , "1", "2", "3"};
  for (byte n = 0; n <= 10; n++)
  {
   Serial.println(Klassement_Punten_BR[n].toInt()+1);
  }
//  Kt = t.toInt();
//  Kt++;
 // Serial.println(Kt);
  //  for (byte n = 0; n <= 10; n++)
  //  {
  //    Serial.println(Klassement_Punten_BR[n]);
  //  delay(100);
  // }

}

void loop() {


}
