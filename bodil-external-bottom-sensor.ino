#define DROPDOWN_STEP      3
#define LEFT_SUPPORT_LEG   4
#define RIGHT_SUPPORT_LEG  5
#define GAS_DOOR           6
#define GAS_VALVE          7
#define INIT_HZ            5
#define DEBUG

class Runnable {
  static Runnable *headRunnable;
  Runnable *nextRunnable;
public:
  static int delayTime;
  bool enabled;
#ifdef DEBUG
  String outputString;
#endif
  Runnable() {
    nextRunnable = headRunnable;
    headRunnable = this;
    enabled = true;
  }

  virtual void setupRunnable() = 0;
  virtual void loopRunnable() = 0;

  static void setupAll() {
    for (Runnable *r = headRunnable; r; r = r->nextRunnable)
      r->setupRunnable();
  }

  static void loopAll() {
    for (Runnable *r = headRunnable; r; r = r->nextRunnable) {
      if (r->enabled) {
        r->loopRunnable();
        delay(Runnable::delayTime);
      }
    }
#ifdef DEBUG
    String outputstring = "";
    for (Runnable *r = headRunnable; r; r = r->nextRunnable) {
      outputstring += r->outputString;
    }
    Serial.println(outputstring);
#endif
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
  CircuitSwitchSensor(String id, byte inputPin) {
    this->id = id;
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

#ifdef DEBUG
    switch(state) {
      case kUnknown: outputString = id + ": Unknown # "; break;
      case kClosed:  outputString = id + ": Closed  # "; break;
      case kOpen:    outputString = id + ": Open    # "; break;
    }
#endif      
  }
};

CircuitSwitchSensor DropdownStepSensor   ("STP_DWN", DROPDOWN_STEP);
CircuitSwitchSensor LeftSupportLegSensor ("LFT_LEG", LEFT_SUPPORT_LEG);
CircuitSwitchSensor RightSupportLegSensor("RGT_LEG", RIGHT_SUPPORT_LEG);
CircuitSwitchSensor GasDoorSensor        ("GAS_DOR", GAS_DOOR);
CircuitSwitchSensor GasValveSensor       ("GAS_VLV", GAS_VALVE);

int Runnable::delayTime = 1000 / INIT_HZ;

void setup() {
  Runnable::setupAll();
  Serial.begin(38400);
}

void loop() {
  Runnable::loopAll();
}
