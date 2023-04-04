int myVariable = 5;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myVariable = myVariable %2;
  Serial.println(myVariable);
}

void loop() {
  // put your main code here, to run repeatedly:

}
