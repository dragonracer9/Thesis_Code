int x;
int maxint=INT32_MAX;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1);
  Serial.print(maxint);
}

void  loop() {
  while (!Serial.available());
  x = Serial.readString().toInt();
  Serial.print(x + 1);
}