/*
Farm PLC 
M-DUINO ETHERNET PLC ARDUINO 57R
*/

// REMOVE THIS START
// This is for GUI to not complain about missing definitions
#define uint8_t unsigned char
#define HIGH 1
#define LOW 0
#define pinMode(a, b)
#define PIN_R0_1                (23)
#define PIN_R0_2                (22)
#define PIN_R0_3                (25)
#define PIN_R0_4                (24)
#define PIN_R0_5                (40)
#define PIN_R0_6                (39)
#define PIN_R0_7                (38)
#define PIN_R0_8                (37)
#define PIN_R1_1                (28)
#define PIN_R1_2                (27)
#define PIN_R1_3                (30)
#define PIN_R1_4                (29)
#define PIN_R1_5                (45)
#define PIN_R1_6                (44)
#define PIN_R1_7                (43)
#define PIN_R1_8                (42)
#define PIN_R2_1                (33)
#define PIN_R2_2                (32)
#define PIN_R2_3                (35)
#define PIN_R2_4                (34)
#define PIN_R2_6                (49)
#define PIN_R2_7                (48)
#define PIN_R2_8                (47)
// REMOVE THIS END

#define DEBUG 1

/* Heater ID and equivalent power in kW */
uint8_t ElementPower[] = {45, 90, 90, 90, 45, 90, 45, 90, 90, 90,
                          45, 90, 90, 90, 45, 90, 45, 90, 90, 90};

/* Heater ID and equivalent pin number connected to the relay */
uint8_t ElementPinMap[] = {PIN_R0_1, PIN_R0_2, PIN_R0_3, PIN_R0_4, PIN_R0_5, PIN_R0_6, PIN_R0_7, PIN_R0_8, PIN_R1_1, PIN_R1_2,
                            PIN_R1_3, PIN_R1_4, PIN_R1_5, PIN_R1_6, PIN_R1_7, PIN_R1_8, PIN_R2_1, PIN_R2_2, PIN_R2_3, PIN_R2_4};

#define NUM_ELEMENTS (sizeof(ElementPower) / sizeof(ElementPower[0]))

#ifdef DEBUG
uint8_t ElementState[NUM_ELEMENTS] = {0};
#endif

/* Center element, moves over time for sake of rotation */
uint8_t currentStart = 0;
int currentPower = 0;

void onHourChange() {
    currentStart = (currentStart + 1) % ElementSize;
    setPower(currentPower);
}

/* If we need to set certain power level, we start from certain element.
    Start gradually moves every hour to rotate elements.
*/
void setPower(int powerdemand) {
    int powergot = 0;
    int idx = currentStart;
    do {
        if (powerdemand >= ElementPower[idx]) {
            digitalWrite(ElementPinMap[idx], HIGH);
#ifdef DEBUG
            ElementState[idx] = 1;
#endif            
            powergot += ElementPower[idx];
        } else {
            digitalWrite(ElementPinMap[idx], LOW);
#ifdef DEBUG
            ElementState[idx] = 0;
#endif            
        }
        idx = (idx + 1) % NUM_ELEMENTS;
    } while (powergot < powerdemand && idx != currentStart);
    currentPower = powerdemand;
#ifdef DEBUG
    Serial.print("Elements states: ");
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        Serial.print(ElementState[i]);
    }
    Serial.println();
#endif
}

void setup() {
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        pinMode(ElementPinMap[i], OUTPUT);
    }
    setPower(0);
}