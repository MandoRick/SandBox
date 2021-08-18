int randomNumber1, randomNumber2, randomNumber3, randomNumber4, randomNumber5, randomNumber6, randomNumber7;
const int numberBoundLower = 0;
const int numberBoundUpper = 50;
int arrayNumber01[numberBoundUpper];
int arrayNumber02[numberBoundUpper];
int arrayNumber03[numberBoundUpper];
int arrayNumber04[numberBoundUpper];
int arrayNumber05[numberBoundUpper];
int arrayNumber06[numberBoundUpper];
int arrayNumber07[numberBoundUpper];


void setup() {
  delay(5000);
  randomSeed(analogRead(3));
}

void setup1() {
  delay(5000);
  Serial.begin(115200);
}

void loop() {
  delay(10);
  generateNumbers();
  delay(10);
  tallyNumbers();
}

void loop1() {
  delay(1000);
  drawNumbers();
}

void generateNumbers() {
  randomNumber1 = random(numberBoundLower, numberBoundUpper);
  randomNumber2 = random(numberBoundLower, numberBoundUpper);
  randomNumber3 = random(numberBoundLower, numberBoundUpper);
  randomNumber4 = random(numberBoundLower, numberBoundUpper);
  randomNumber5 = random(numberBoundLower, numberBoundUpper);
  randomNumber6 = random(numberBoundLower, numberBoundUpper);
  randomNumber7 = random(numberBoundLower, numberBoundUpper);
}

void tallyNumbers() {
  arrayNumber01[randomNumber1]++;
  arrayNumber02[randomNumber2]++;
  arrayNumber03[randomNumber3]++;
  arrayNumber04[randomNumber4]++;
  arrayNumber05[randomNumber5]++;
  arrayNumber06[randomNumber6]++;
  arrayNumber07[randomNumber7]++;
}

void drawNumbers() {
  Serial.println("");
  Serial.println("");
  Serial.println("------------------------------------------------ RANDOMS ------------------------------------------------");
  Serial.print(randomNumber1);
  Serial.print(" ");
  Serial.print(randomNumber2);
  Serial.print(" ");
  Serial.print(randomNumber3);
  Serial.print(" ");
  Serial.print(randomNumber4);
  Serial.print(" ");
  Serial.print(randomNumber5);
  Serial.print(" ");
  Serial.print(randomNumber6);
  Serial.print(" ");
  Serial.println(randomNumber7);
  Serial.println("---------------------------------------------------------------------------------------------------------");
  Serial.println("------------------------------------------------ RESULTS ------------------------------------------------");
  for (int i = 0; i < numberBoundUpper; i++)
  {
    Serial.print(arrayNumber01[i]);
    Serial.print(" ");
  }
  Serial.println("");
  for (int i = 0; i < numberBoundUpper; i++)
  {
    Serial.print(arrayNumber02[i]);
    Serial.print(" ");
  }
  Serial.println("");
  for (int i = 0; i < numberBoundUpper; i++)
  {
    Serial.print(arrayNumber03[i]);
    Serial.print(" ");
  }
  Serial.println("");
  for (int i = 0; i < numberBoundUpper; i++)
  {
    Serial.print(arrayNumber04[i]);
    Serial.print(" ");
  }
  Serial.println("");
  for (int i = 0; i < numberBoundUpper; i++)
  {
    Serial.print(arrayNumber05[i]);
    Serial.print(" ");
  }
  Serial.println("");
  for (int i = 0; i < numberBoundUpper; i++)
  {
    Serial.print(arrayNumber06[i]);
    Serial.print(" ");
  }
  Serial.println("");
  for (int i = 0; i < numberBoundUpper; i++)
  {
    Serial.print(arrayNumber07[i]);
    Serial.print(" ");
  }
  Serial.println("");
  Serial.println("---------------------------------------------------------------------------------------------------------");
  Serial.println("");
  Serial.println("");
}

