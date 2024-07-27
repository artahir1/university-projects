////////////////////////////////////////////////////////
int led[7] =      {8, 7, 6, 5, 4, 3, 2};  // obstacle leds
int state[7] =    {0, 0, 0, 0, 0, 0, 1};  // their states
int lastLed =                        6;
int dist;
int dinoDown = 9;
int dinoDownState = 1;
int dinoUp = 10;
int jumpFlag = 0;
int initialTime;
int Do = 1;

////////////////////////////////////////////////////////
void setup() {

    Serial.begin(9600);

    pinMode(2, OUTPUT); // obstacle
    pinMode(3, OUTPUT); // obstacle
    pinMode(4, OUTPUT); // obstacle
    pinMode(5, OUTPUT); // obstacle
    pinMode(6, OUTPUT); // obstacle
    pinMode(7, OUTPUT); // obstacle
    pinMode(8, OUTPUT); // obstacle
    pinMode(dinoDown, OUTPUT);  // dino
    pinMode(dinoUp, OUTPUT); // dino
    pinMode(13, OUTPUT); // buzzer
    
    randomSeed(analogRead(0)); 

    dist = getRandomDist();

    digitalWrite(dinoDown,1);
}

// nextLed  <-  currentLed

////////////////////////////////////////////////////////
void loop() {
  
  if (shouldJump())
    jumpFlag = 1;

  if (jumpFlag) 
    jump();

  show_current_LEDS_States();

  if (isCollide())
    gameOver();

  if (Do) { 
    initialTime = millis(); 
    Do = 0;
  }
  int currentTime = millis();

  if (currentTime - initialTime >= 486) {
    update_LEDS_States();
    Do = 1;
  }
}

////////////////////////////////////////////////////////
void show_current_LEDS_States() {
  for (int i = 0; i < 7; i++) {
    digitalWrite(led[i], state[i]);
  }
}

////////////////////////////////////////////////////////
void update_LEDS_States() {
  for (int i = 0; i < 7; i++) {

    int currentLed = i;
    int nextLed = i-1;

    if (state[currentLed] == 1) {

      state[currentLed] = 0;

      if (nextLed >= 0)
      state[nextLed] = 1;

      if (currentLed == lastLed)
      lastLed = nextLed;

    }
  }

  if (lastLed + dist == 6) {
    state[6] = 1;
    lastLed = 6;
    dist = getRandomDist();
  }
}

////////////////////////////////////////////////////////
int getRandomDist() {
  return random(2, 6);
}

////////////////////////////////////////////////////////
int shouldJump() {

   if (Serial.available() > 0) {
    char receivedChar = Serial.read(); 
    if (receivedChar == '\n')
      return 1;
  }
  return 0;
}


////////////////////////////////////////////////////////
int flag[4] = {1, 0, 0, 0};
int firstTime[4] = {1, 1, 1, 1};
int startTime[4];

void jump() {

  ///// 0 /////             // dino's down led off after 200 ms
  if (flag[0]) {

    if (firstTime[0]) {
      startTime[0] = millis();
      firstTime[0] = 0;
    }

    int currentTime = millis();

    if (currentTime - startTime[0]  >= 200) {
      digitalWrite(dinoDown, 0);
      dinoDownState = 0;

      firstTime[0] = 1;
      flag[0] = 0;
      flag[1] = 1;
    }
  }

  ///// 1 /////             // dino's up led on after 200 ms
  else if (flag[1]) {

    if (firstTime[1]) {
      startTime[1] = millis();
      firstTime[1] = 0;
    }

    int currentTime = millis();

    if (currentTime - startTime[1]  >= 200) {
      digitalWrite(dinoUp, 1); 

      firstTime[1] = 1;
      flag[1] = 0;
      flag[2] = 1;
    }
  }
  
  ///// 2 /////             // dino's up led off after 300 ms
  else if (flag[2]) {

    if (firstTime[2]) {
      startTime[2] = millis();
      firstTime[2] = 0;
    }

    int currentTime = millis();

    if (currentTime - startTime[2]  >= 300) {
      digitalWrite(dinoUp, 0);

      firstTime[2] = 1;
      flag[2] = 0;
      flag[3] = 1;
    }
  }
  
  ///// 3 /////             // dino's down led on after 200 ms
  else if (flag[3]) {

    if (firstTime[3]) {
      startTime[3] = millis();
      firstTime[3] = 0;
    }

    int currentTime = millis();

    if (currentTime - startTime[3]  >= 200) {
      digitalWrite(dinoDown, 1);
      dinoDownState = 1;
      
      firstTime[3] = 1;
      flag[3] = 0;
      flag[0] = 1;
      jumpFlag = 0;
    }
  }
}

////////////////////////////////////////////////////////
int isCollide() {

  if (state[1] && dinoDownState) 
  return 1;

  return 0;
}

////////////////////////////////////////////////////////
void gameOver() {
 
  for (int i=0; i<10; i++) {

    digitalWrite(7, 1);
    digitalWrite(dinoDown, 1);
    digitalWrite(13, 1);

    delay(50);
    
    digitalWrite(7, LOW);
    digitalWrite(dinoDown, LOW);
    digitalWrite(13, LOW);

    delay(50);

  }
    asm volatile ("  jmp 0");
}
////////////////////////////////////////////////////////