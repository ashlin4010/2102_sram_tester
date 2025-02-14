const uint8_t LED_RED = A1;
const uint8_t LED_GREEN = A2;
const uint8_t SRAM_A0 = 4;
const uint8_t SRAM_A1 = 5;
const uint8_t SRAM_A2 = 6;
const uint8_t SRAM_A3 = 7;
const uint8_t SRAM_A4 = 8;
const uint8_t SRAM_A5 = 9;
const uint8_t SRAM_A6 = 10;
const uint8_t SRAM_A7 = 11;
const uint8_t SRAM_A8 = 12;
const uint8_t SRAM_A9 = 13;
const uint8_t CS = A4;
const uint8_t DIN = A5;
const uint8_t DOUT = 3;
const uint8_t RW = A3;
const uint8_t BUTTON = 2; 
const uint8_t *ADDRESS[10] = {&SRAM_A0, &SRAM_A1, &SRAM_A2, &SRAM_A3, &SRAM_A4, &SRAM_A5, &SRAM_A6, &SRAM_A7, &SRAM_A8, &SRAM_A9 };
const uint8_t ADDRESS_COUNT = 10;
const uint16_t ADDRESS_MAX = 2 << ADDRESS_COUNT;

volatile bool is_running_tests = false;

void setup() {
  Serial.begin(9600);             // Debug 
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(CS, OUTPUT);
  pinMode(DIN, OUTPUT);
  pinMode(RW, OUTPUT);
  pinMode(DOUT, INPUT);

  digitalWrite(CS, HIGH);         // Chip select active low
  digitalWrite(RW, HIGH);         // Write active low

  // Set up SRAM address pins
  for (uint8_t i = 0; i < ADDRESS_COUNT; i++) {
    pinMode(*ADDRESS[i], OUTPUT);
  }
  attachInterrupt(digitalPinToInterrupt(BUTTON), buttonISR, RISING);
}

void buttonISR() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();

  if (interrupt_time - last_interrupt_time > 150) {
    is_running_tests = !is_running_tests;
  }
  last_interrupt_time = interrupt_time;
}

bool writeReadTest(uint16_t address, uint8_t value)  {
  // This code is very slow but is a basic test.
  // For better testing this code should be written with timers and in AVRC
  for(uint16_t b = 0; b < ADDRESS_COUNT; b++) {
    int bit = (address >> b) & 1;
    digitalWrite(*ADDRESS[b], bit);   
  }
  delayMicroseconds(1);             // Allow time to settle

  digitalWrite(DIN, value);    // Set bit to write
  digitalWrite(CS, 0);              // Write bit
  digitalWrite(RW, 0);
  digitalWrite(RW, 1);
  digitalWrite(CS, 1);
  delayMicroseconds(1);             // Wait
  digitalWrite(CS, 0);              // Read bit
  uint8_t value_out = digitalRead(DOUT);
  digitalWrite(CS, 1);
  return value_out == value;
}

void loop() {
  if(is_running_tests) {
    digitalWrite(LED_RED, LOW);
    for(uint16_t address = 0; (address < ADDRESS_MAX) && is_running_tests; address++) {

      bool testPass = writeReadTest(address, 1) && writeReadTest(address, 0) && writeReadTest(address, 1);

      if (!testPass) {
        digitalWrite(LED_RED, 1);
        is_running_tests = false;
        Serial.print("Test failed: ");
        Serial.println(address);
        break;
      }
    }
    // Flash green light at the end of the test
    if (is_running_tests){
      digitalWrite(LED_GREEN, HIGH);
      delay(100);
      digitalWrite(LED_GREEN, LOW);
      delay(100);
    }
  }
} 
