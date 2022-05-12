#define LED_LIVE 25
#define LED_COM 26
#define BTN_COLOR 32
#define BTN_SIZE 33

static bool evPrintSizeOn = true;
static bool evPrintSizeOff = true;
static bool evPrintColorOn = true;
static bool evPrintColorOff = true;

static uint32_t sizeCube = 0;
static uint8_t ledReqState = LOW;

void serialComTask();
void beatTask();
void serialReqTask();

void setup() {
  pinMode(LED_COM, OUTPUT);
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
        switch (Serial.read())
        {
          case 'S': {
              serialReqState = SerialReqStates::LED_ON;
              break;
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
  enum class SerialComStates {INIT, PRESS_BUTTON};
  static SerialComStates serialComState = SerialComStates::INIT;

  switch (serialComState) {
    case SerialComStates::INIT: {
        pinMode(BTN_SIZE, INPUT_PULLUP);
        pinMode(BTN_COLOR, INPUT_PULLUP);
        serialComState = SerialComStates::PRESS_BUTTON;
        break;
      }
    case SerialComStates::PRESS_BUTTON: {

        if (digitalRead(BTN_SIZE) == LOW && evPrintSizeOff == true) {
          sizeCube = 3;
          String message;
          message.concat(digitalRead(BTN_SIZE));
          message.concat(',');
          message.concat(sizeCube);
          Serial.println(message);
          evPrintSizeOff = false;
          evPrintSizeOn = true;
        }
        else if (digitalRead(BTN_SIZE) == HIGH && evPrintSizeOn == true) {
          sizeCube = 1;
          String message;
          message.concat(digitalRead(BTN_SIZE));
          message.concat(',');
          message.concat(sizeCube);
          Serial.println(message);
          evPrintSizeOff = true;
          evPrintSizeOn = false;
        }

        //delay(100);

        if (digitalRead(BTN_COLOR) == LOW && evPrintColorOff == true) {
          Serial.println(digitalRead(BTN_COLOR));
          evPrintColorOff = false;
          evPrintColorOn = true;
        }
        else if (digitalRead(BTN_COLOR) == HIGH && evPrintColorOn == true) {
          Serial.println(digitalRead(BTN_COLOR));
          evPrintColorOff = true;
          evPrintColorOn = false;
        }


        //delay(100);
        break;
      }
    default:
      break;
  }
}
