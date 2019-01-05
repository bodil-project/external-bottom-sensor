6 circuit switch sensors for the underside of the RV

* DWN_STP 0b001 Dropdown step sensor. Reed sensor
* LFT_LEG 0b010 Left support leg sensor. Reed sensor
* RGT_LEG 0b011 Right support leg sensor. Reed sensor
* GAS_DOR 0b100 Gas door open sensor. Reed sensor
* GAS_VLV 0b101 Gas valve open sensor. Switch
* EMPTY__ 0b110 
* ALL     0b111

// Sensor input pins
D3 Dropdown step extended
D4 Left support leg down
D5 Right support leg down
D6 Gas door open
D7 Gas valve open
D8 Extra switch
 
Sensor address bit [D10][D11][D12]

Operation input pins
A0 Q - Query state. ALL not supported
A1 E - Enable sensor. ALL supported
A2 D - Disable sensor. ALL supported
A3 X - Reserved
A4 X - Reserved
A5 X - Reserved
A6 X - Reserved
A7 X - Reserved


Operation output pins
D13 Operation result

State
Closed = LOW|0
Open = HIGH|1

DWN_STP Open = Extended
DWN_STP Closed = Not extended ;)
LFT_LEG Open = Down
LFT_LEG Closed = Up
RGT_LEG Open = Down
RGT_LEG Closed = Up
GAS_DOR Open = Open
GAS_DOR Closed = Closed
GAS_Valve Open = Open
GAS_Valve Closed = Closed

Q DWN_STP|1
-1|0|1

DISABLE DWN_STP|1
1
ENABLE DWN_STP|1
1
