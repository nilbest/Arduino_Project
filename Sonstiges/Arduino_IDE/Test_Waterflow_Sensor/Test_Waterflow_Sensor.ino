int flowSensor=2;
float Data=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(flowSensor, INPUT);
}


void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead==High){

  }
  Data=digitalRead(flowSensor);
  Serial.println(Data);
  //pulseIn()
}
