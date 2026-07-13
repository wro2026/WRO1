#define S0 5
#define S1 4
#define S2 7
#define S3 6
#define sensorOut 8
#define PI_PIN_A 2
#define PI_PIN_B 3

int r = 0;
int g = 0;
int b = 0;

const int WHITE_LIMIT = 45; 

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  pinMode(PI_PIN_A, OUTPUT);
  pinMode(PI_PIN_B, OUTPUT);
  
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  r = pulseIn(sensorOut, LOW);
  delay(10);
  
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  g = pulseIn(sensorOut, LOW);
  delay(10);
  
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  b = pulseIn(sensorOut, LOW);

  if (r < WHITE_LIMIT && g < WHITE_LIMIT && b < WHITE_LIMIT) {
    Serial.println("WHITE");
    digitalWrite(PI_PIN_A, LOW);
    digitalWrite(PI_PIN_B, LOW);
  } else if (b < r && b < g) {
    Serial.println("BLUE");
    digitalWrite(PI_PIN_A, HIGH);
    digitalWrite(PI_PIN_B, LOW);
  } else if (r < b && g < b && r < (g + 30)) {
    Serial.println("ORANGE");
    digitalWrite(PI_PIN_A, LOW);
    digitalWrite(PI_PIN_B, HIGH);
  } else {
    Serial.println("UNKNOWN");
    digitalWrite(PI_PIN_A, LOW);
    digitalWrite(PI_PIN_B, LOW);
  }
  
  delay(50);
}
