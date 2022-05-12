#define SEPARATOR 90

#define LED_LIVE 25
#define LED_COM 26
#define BTN_COLOR 32
#define BTN_SIZE 33

static bool evWriteSizeOn = true;
static bool evWriteSizeOff = true;
static bool evWriteColorOn = true;
static bool evWriteColorOff = true;

void beatTask();
void serialReqTask();
void serialComTask();

void setup() {
  Serial.begin(9600);
  serialReqTask();
  beatTask();
  serialComTask();
}

void loop() {
  serialReqTask();
  beatTask();
  serialComTask();
}

void serialReqTask() {
  enum class SerialReqStates {INIT, WAIT_REQST, LED_ON};
  static SerialReqStates serialReqState = SerialReqStates::INIT;
  static uint8_t ledReqState = LOW;

  switch (serialReqState) {
    case SerialReqStates::INIT: {
        pinMode(LED_COM, OUTPUT);
        serialReqState = SerialReqStates::WAIT_REQST;
        break;
      }
    case SerialReqStates::WAIT_REQST: {
        if (Serial.available()) {
          if (Serial.read() == 0x3e) {
            serialReqState = SerialReqStates::LED_ON;
          }
        }
        break;
      }
    case SerialReqStates::LED_ON: {
        if (ledReqState == LOW)
          ledReqState = HIGH;
        else
          ledReqState = LOW;
        digitalWrite(LED_COM, ledReqState);
        serialReqState = SerialReqStates::WAIT_REQST;
        break;
      }
    default:
      break;

  }
}

void beatTask() {
  enum class BeatStates {INIT, BEATING};

  static BeatStates beatState = BeatStates::INIT;
  const uint32_t interval = 500;
  static uint32_t previousMillis = 0;
  static uint8_t ledState = LOW;

  switch (beatState) {
    case BeatStates::INIT:
      {
        digitalWrite(LED_LIVE, ledState);
        pinMode(LED_LIVE, OUTPUT);
        beatState = BeatStates::BEATING;
        break;
      }
    case BeatStates::BEATING: {
        if (millis() - previousMillis >= interval) {
          previousMillis = millis();
          if (ledState == LOW)
            ledState = HIGH;
          else
            ledState = LOW;
          digitalWrite(LED_LIVE, ledState);
        }
        break;
      }
    default:
      break;
  }
}

void serialComTask() {
  enum class SerialComStates {INIT, PRESS_BUTTON,};
  static SerialComStates serialComState = SerialComStates::INIT;
  byte vecSendPackSize[] = { 0, 2, 8, 4, SEPARATOR}; //el primer dato esta reservado
  byte vecSendPackColor[] = { 0, SEPARATOR};//el primer dato esta reservado
  switch (serialComState) {
    case SerialComStates::INIT: {
        pinMode(BTN_SIZE, INPUT_PULLUP);
        pinMode(BTN_COLOR, INPUT_PULLUP);
        serialComState = SerialComStates::PRESS_BUTTON;
        break;
      }
    case SerialComStates::PRESS_BUTTON: {
        if (digitalRead(BTN_SIZE) == LOW && evWriteSizeOff == true) {
          vecSendPackSize[0] = digitalRead(BTN_SIZE);
          Serial.write(vecSendPackSize, sizeof(vecSendPackSize));
          evWriteSizeOff = false;
          evWriteSizeOn = true;
        }
        else if (digitalRead(BTN_SIZE) == HIGH && evWriteSizeOn == true) {
          vecSendPackSize[0] = digitalRead(BTN_SIZE);
          vecSendPackSize[1] = 1;
          vecSendPackSize[2] = 1;
          vecSendPackSize[3] = 1;
          Serial.write(vecSendPackSize, sizeof(vecSendPackSize));
          evWriteSizeOff = true;
          evWriteSizeOn = false;
        }

        if (digitalRead(BTN_COLOR) == LOW && evWriteColorOff == true) {
          evWriteColorOff = false;
          evWriteColorOn = true;
          vecSendPackColor[0] = digitalRead(BTN_COLOR);
          Serial.write(vecSendPackColor, sizeof(vecSendPackColor));
          Serial.flush();
        }
        else if (digitalRead(BTN_COLOR) == HIGH && evWriteColorOn == true) {
          evWriteColorOn = false;
          evWriteColorOff = true;
          vecSendPackColor[0] = digitalRead(BTN_COLOR);
          Serial.write(vecSendPackColor, sizeof(vecSendPackColor));
          Serial.flush();

        }
        break;
      }
    default:
      break;
  }
}
