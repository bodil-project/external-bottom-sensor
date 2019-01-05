#define DROPDOWN_STEP_SENSOR_INPUT_PIN      3
#define LEFT_SUPPORT_LEG_SENSOR_INPUT_PIN   4
#define RIGHT_SUPPORT_LEG_SENSOR_INPUT_PIN  5
#define GAS_DOOR_SENSOR_INPUT_PIN           6
#define GAS_VALVE_SENSOR_INPUT_PIN          7
#define OUTPUT_PIN                          13

class Runnable {
  static Runnable *headRunnable;
  Runnable *nextRunnable;
public:
  static int delayTime;
  Runnable() {
    nextRunnable = headRunnable;
    headRunnable = this;
  }

  virtual void setupRunnable() = 0;
  virtual void loopRunnable() = 0;

  static void setupAll() {
    for (Runnable *r = headRunnable; r; r = r->nextRunnable)
      r->setupRunnable();
  }

  static void loopAll() {
    for (Runnable *r = headRunnable; r; r = r->nextRunnable) {
      r->loopRunnable();
      delay(Runnable::delayTime);
    }
  }
};

Runnable *Runnable::headRunnable = NULL;

enum CircuitSwitchState {
  kUnknown = -1,
  kClosed = 0,
  kOpen = 1
};

class CircuitSwitchSensor : public Runnable {
  byte inputPin;
  byte outputPin;
  String id;
public:
  CircuitSwitchState state = kUnknown;
  CircuitSwitchSensor(String id, byte outputPin, byte inputPin) {
    this->id = id;
    this->outputPin = outputPin;
    this->inputPin = inputPin;
  }
  void setupRunnable() {
    pinMode(this->inputPin, INPUT_PULLUP);
  }
  void loopRunnable() {
    if (digitalRead(inputPin) == LOW) {
      state = kClosed;
    } else {
      state = kOpen;
    }

    switch(state) {
      case kUnknown: Serial.print(id + ": Unknown # "); break;
      case kClosed:  Serial.print(id + ": Closed  # "); break;
      case kOpen:    Serial.print(id + ": Open    # "); break;
    }
      
  }
};

CircuitSwitchSensor DropdownStepSensor   ("STP_DWN", OUTPUT_PIN, DROPDOWN_STEP_SENSOR_INPUT_PIN);
CircuitSwitchSensor LeftSupportLegSensor ("LFT_LEG", OUTPUT_PIN, LEFT_SUPPORT_LEG_SENSOR_INPUT_PIN);
CircuitSwitchSensor RightSupportLegSensor("RGT_LEG", OUTPUT_PIN, RIGHT_SUPPORT_LEG_SENSOR_INPUT_PIN);
CircuitSwitchSensor GasDoorSensor        ("GAS_DOR", OUTPUT_PIN, GAS_DOOR_SENSOR_INPUT_PIN);
CircuitSwitchSensor GasValveSensor       ("GAS_VLV", OUTPUT_PIN, GAS_VALVE_SENSOR_INPUT_PIN);

int Runnable::delayTime = 500;

void setup() {
  Runnable::setupAll();
  Serial.begin(9600);
}

void loop() {
  Runnable::loopAll();
  Serial.println();
}
