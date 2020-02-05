static const uint8_t pin = 2;
static const unsigned long tick = 100UL;

#ifdef ARDUINO_AVR_NANO
#define IRRECEIVER_1_GND 9U
#define IRRECEIVER_1_VCC 10U
#endif

#define BAUD 115200

static volatile uint16_t cnt = 0;
static unsigned long next;

static void irqHandler() {
    cnt++;
}

void setup() {
#ifdef IRRECEIVER_1_GND
    pinMode(IRRECEIVER_1_GND, OUTPUT);
    digitalWrite(IRRECEIVER_1_GND, LOW);
#endif

#ifdef IRRECEIVER_1_VCC
    pinMode(IRRECEIVER_1_VCC, OUTPUT);
    digitalWrite(IRRECEIVER_1_VCC, HIGH);
#endif
    pinMode(pin, INPUT);
    Serial.begin(BAUD);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB
    }
    attachInterrupt(digitalPinToInterrupt(pin), irqHandler, RISING);

    next = micros() + tick;
}

void loop() {
    if (micros() >= next) {
        //int32_t toDelay = next - micros();
        //uint16_t del = toDelay % UINT16_MAX;
        //delayMicroseconds(del);
        noInterrupts();
        uint8_t c = (uint8_t) (cnt % 256U);
        cnt = 0;
        interrupts();
        Serial.write(c);
        Serial.flush();
        next += tick;
    }
}
