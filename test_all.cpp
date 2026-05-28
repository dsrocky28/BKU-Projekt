// Made for playground-brumbrum-esp32-s3-devkitc1
#include "test_all.h"
#include "serial.h"
#include "buttons.h"
#include "dcmotor.h"
#include "ultrasonic.h"
#include "sdcard.h"
#include "max98357a.h"
#include "internalLED.h"
#include "servos.h"

static bool serialInitialized = false;

static void ensureSerialInit() {
    if (!serialInitialized) {
        initSerial(115200);
        serialInitialized = true;
        serialPrintln("\n=== BrumBrum Test System ===");
        serialPrintln("Tests initialized. Use Serial Monitor to view output.");
    }
}

typedef struct {
    const char* name;
    uint16_t bit;
    void (*testFunc)();
} TestItem;

static void waitForButtonRelease(int pin) {
    delay(20);
    while (digitalRead(pin) == LOW) {
        delay(5);
    }
    delay(20);
}

static bool readButton(int pin) {
    return digitalRead(pin) == LOW;
}

static bool checkHoldExit(int pin, unsigned long holdStart) {
    while (digitalRead(pin) == LOW) {
        if (millis() - holdStart >= 1000) {
            waitForButtonRelease(pin);
            return true;
        }
        delay(10);
    }
    return false;
}

static void testDCMotorsWrapper() {
    testDCMotors(0x0F);
}

void runTestMenu(uint16_t testMask) {
    ensureSerialInit();

    pinMode(SW1_PIN, INPUT_PULLUP);
    pinMode(SW2_PIN, INPUT_PULLUP);

    TestItem tests[] = {
        {"SG90 Servo", TEST_SG90_SERVO, testServos}
    };

    int numTests = sizeof(tests) / sizeof(tests[0]);
    int activeTests[9];
    int numActive = 0;

    for (int i = 0; i < numTests; i++) {
        if (testMask & tests[i].bit) {
            activeTests[numActive++] = i;
        }
    }

    if (numActive == 0) {
        serialPrintln("\n[ERROR] No tests selected!");
        delay(2000);
        return;
    }

    int selectedIndex = 0;
    bool running = true;
    unsigned long sw1HoldStart = 0;
    bool sw1WasPressed = false;
    bool menuNeedsRedraw = true;

    while (running) {
        if (menuNeedsRedraw) {
            serialPrintln("\n========================================");
            serialPrintln("          TEST MENU (Serial)");
            serialPrintln("========================================");

            for (int i = 0; i < numActive; i++) {
                int testIdx = activeTests[i];
                if (i == selectedIndex) {
                    serialPrint(" > ");
                } else {
                    serialPrint("   ");
                }
                serialPrintln(tests[testIdx].name);
            }

            serialPrintln("----------------------------------------");
            serialPrintln("Controls:");
            serialPrintln("  SW1 (short): Next test");
            serialPrintln("  SW1 (hold 1s): Exit menu");
            serialPrintln("  SW2: Run selected test");
            serialPrintln("========================================");
            menuNeedsRedraw = false;
        }

        delay(20);

        if (readButton(SW1_PIN)) {
            if (!sw1WasPressed) {
                sw1HoldStart = millis();
                sw1WasPressed = true;

                if (checkHoldExit(SW1_PIN, sw1HoldStart)) {
                    serialPrintln("\n[Exit] Exiting test menu...");
                    running = false;
                    continue;
                }

                waitForButtonRelease(SW1_PIN);
                selectedIndex++;
                if (selectedIndex >= numActive) selectedIndex = 0;
                menuNeedsRedraw = true;
            }
        } else {
            sw1WasPressed = false;
        }

        if (readButton(SW2_PIN)) {
            waitForButtonRelease(SW2_PIN);

            int testIdx = activeTests[selectedIndex];
            serialPrintln("\n========================================");
            serialPrint("[RUN] Starting test: ");
            serialPrintln(tests[testIdx].name);
            serialPrintln("========================================");
            delay(500);

            tests[testIdx].testFunc();

            serialPrintln("\n[OK] Test complete!");
            serialPrintln("Press SW2 to return to menu...");

            while (!readButton(SW2_PIN)) {
                delay(10);
            }
            waitForButtonRelease(SW2_PIN);

            menuNeedsRedraw = true;
        }
    }

    serialPrintln("\n[Exit] Test menu closed.");
}
