// See Z90Run for the program
class VirtualThread {
  static VirtualThread *headVirtualThread;
  VirtualThread *nextVirtualThread;

  bool isLoopEnabled;
public:
  VirtualThread(bool loopEnabled = true) {
    nextVirtualThread = headVirtualThread;
    headVirtualThread = this;
    isLoopEnabled = loopEnabled;
    Serial.print("VirtualThread(");
    Serial.print(isLoopEnabled);
    Serial.println(")");
  }

  virtual void initVirtualThread() = 0;
  virtual void runVirtualThread() = 0;
  virtual void printConfig() = 0;
  virtual void disableLoop() {
    isLoopEnabled = false;
  }

  static void initAll() {
    Serial.print("initAll()");
    headVirtualThread->printConfig();
    for (VirtualThread *r = headVirtualThread; r; r = r->nextVirtualThread) {
      r->printConfig();
      r->initVirtualThread();
    }
    Serial.println();
  }

  static void loopAll() {
    Serial.print("loopAll()");
    for (VirtualThread *r = headVirtualThread; r; r = r->nextVirtualThread) {
      Serial.print("*");
      if (r->isLoopEnabled) {
        r->runVirtualThread();
        delay(1000 / VirtualThread::Hz);
      }
    }
    Serial.println();
  }

  static int Hz;
};

VirtualThread *VirtualThread::headVirtualThread = NULL;

struct CircuitSwitchSensorConfig {
  String id;
  int   sensorId;
  int   inputPin;
  bool   isLoopEnabled;
};

enum CircuitSwitchSensorState {
  kUnknown = -1,
  kClosed  =  0,
  kOpen    =  1
};


class CircuitSwitchSensor : public VirtualThread {
  CircuitSwitchSensorConfig sensorConfig;
  CircuitSwitchSensorState state = kUnknown;
  void (*pCircuitSwitchChange)(CircuitSwitchSensorConfig, CircuitSwitchSensorState) = NULL;
  void setState(CircuitSwitchSensorState newState) {
    if (state != newState) {
      state = newState;
      Serial.println();
      Serial.println("INTERRUPT");
      printConfig();
      if (pCircuitSwitchChange)
        pCircuitSwitchChange(sensorConfig, state);
    }
  }
public:
  CircuitSwitchSensor(String id, int sensorId, int inputPin, bool loopEnabled = true, void p(CircuitSwitchSensorConfig, CircuitSwitchSensorState) = NULL) : VirtualThread(loopEnabled) { 
    sensorConfig.id = id;
    sensorConfig.sensorId = sensorId;
    sensorConfig.inputPin = inputPin;
    sensorConfig.isLoopEnabled = loopEnabled;
    pCircuitSwitchChange = p;
  }
  
  CircuitSwitchSensor(CircuitSwitchSensorConfig init, int index, void p(CircuitSwitchSensorConfig, CircuitSwitchSensorState) = NULL) : CircuitSwitchSensor(init.id, init.sensorId, init.inputPin, init.isLoopEnabled, p) {
    Serial.println("CircuitSwitchSensor()");    
    if (sensorConfig.sensorId != (index + 1) || (sensorConfig.inputPin != (index + 3))) {
      sensorConfig.id = init.id + "[" + init.sensorId + "][" + init.inputPin + "][" + init.isLoopEnabled + "]";
      sensorConfig.sensorId = index + 1;
      sensorConfig.inputPin = index + 3;
    }
    printConfig();
  }
  
  void initVirtualThread() {
    if (sensorConfig.sensorId > 0)
      pinMode(sensorConfig.inputPin, INPUT_PULLUP);
  }
  
  void runVirtualThread() {
    printConfig();
    if (digitalRead(sensorConfig.inputPin) == LOW)
      setState(kClosed);
    else
      setState(kOpen);
    switch(getState()) {
      case kOpen: Serial.print("1"); break;
      case kClosed: Serial.print("0"); break;
      case kUnknown: Serial.print("?"); break;
    }
  }

  void printConfig() {
    Serial.print(sensorConfig.id);
    Serial.print(", ");
    Serial.print(sensorConfig.sensorId);
    Serial.print(", ");
    Serial.print(sensorConfig.inputPin);
    Serial.print(", ");
    Serial.println(sensorConfig.isLoopEnabled);
  }

  void setStateChangeEvent(void p(CircuitSwitchSensorConfig, CircuitSwitchSensorState)) {
    pCircuitSwitchChange = p;    
  }
  
  CircuitSwitchSensorState getState() {
    return state;
  }
};

CircuitSwitchSensorConfig CircuitSwitchSensors[6] {
  {"DRP_DWN", 1, 3, true}, // Dropdown step extended sensor
  {"LFT_LEG", 2, 4, true}, // Left support down leg sensor
  {"RGT_LEG", 3, 5, true}, // Right support down leg sensor
  {"GAS_DOR", 4, 6, true}, // Gas door open sensor
  {"GAS_VLV", 5, 7, true}, // Gas valve open sensor
  {"NOT_USD", 6, 8, false} // Not used
};



int VirtualThread::Hz = 5;

class StateChanger {
  public:
  static void StateChange(CircuitSwitchSensorConfig sensorConfig, CircuitSwitchSensorState state) {
    Serial.print(sensorConfig.id + " = ");
    switch(state) {
      case kOpen: Serial.print("Open"); break;
      case kClosed: Serial.print("Closed"); break;
      case kUnknown: Serial.print("Unknown"); break;
    }
    Serial.print(" [");
    Serial.print(sensorConfig.sensorId + ", ");
    Serial.print(sensorConfig.inputPin + ", ");
    Serial.println(sensorConfig.isLoopEnabled + "]");
    
  }
};


void setup() {
  //StateChanger stateChanger = StateChanger();
  Serial.begin(9600);
  Serial.println("bodil-external-bottom-sensor!");
  //for (int i = 0; i < 5; i++) {
  CircuitSwitchSensor s0 = CircuitSwitchSensor(CircuitSwitchSensors[0], 0, StateChanger::StateChange);
  CircuitSwitchSensor s1 = CircuitSwitchSensor(CircuitSwitchSensors[1], 1, StateChanger::StateChange);
  CircuitSwitchSensor s2 = CircuitSwitchSensor(CircuitSwitchSensors[2], 2, StateChanger::StateChange);
  CircuitSwitchSensor s3 = CircuitSwitchSensor(CircuitSwitchSensors[3], 3, StateChanger::StateChange);
  CircuitSwitchSensor s4 = CircuitSwitchSensor(CircuitSwitchSensors[4], 4, StateChanger::StateChange);
  //}
  VirtualThread::initAll();
}

void loop() {
  
  //VirtualThread::loopAll();
}
