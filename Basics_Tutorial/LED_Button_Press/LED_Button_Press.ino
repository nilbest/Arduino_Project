int button = 7;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(button,INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(digitalRead(button));
  if(digitalRead(button)==HIGH){
    digitalWrite(LED_BUILTIN,HIGH);
    //Serial.println("Button is pressed");
  }
  else{
    digitalWrite(LED_BUILTIN,LOW);
  }
}
