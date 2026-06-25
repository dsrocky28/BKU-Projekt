# ESP32-S3-N16R8 Joystick Auto

## Projekt-Dokumentation

**Steuerung per Joystick mit Servo-Lenkung, DC-Antrieb, Hindernis-Stopp und Rückwärts-Piepser**

---

## Kurzbeschreibung

Dieses Projekt beschreibt ein kleines Auto mit einem **ESP32-S3-N16R8**. Es wird mit einem Joystick gesteuert und nutzt einen Servo für die Lenkung, einen DC-Motor für den Antrieb, einen Ultraschallsensor für die Hinderniserkennung und ein **MAX98357A-Audiomodul** für den Rückwärts-Piepser.

Der Joystick steuert das Auto. Wenn der Joystick nach links bewegt wird, lenkt der Servo nach links. Wenn der Joystick nach rechts bewegt wird, lenkt der Servo nach rechts. Wenn der Joystick in der Mitte steht, stoppt der Servo. Der Joystick-Knopf wird als Gas-Taster benutzt. Wenn der Knopf gedrückt wird, fährt der DC-Motor vorwärts. Wenn der Knopf losgelassen wird, stoppt der Motor.

Der Ultraschallsensor erkennt Hindernisse vor dem Auto. Wenn der Abstand grösser als **3 cm** ist, kann das Auto normal fahren. Wenn der Abstand **3 cm oder weniger** beträgt, stoppt der Motor automatisch. Danach fährt das Auto **2 Sekunden rückwärts**. Während dieser Rückwärtsfahrt macht der Lautsprecher ein `peep peep`-Geräusch. Danach bleibt das Auto stehen. Es fährt erst wieder weiter, wenn der Joystick-Knopf losgelassen und erneut gedrückt wird.

---

## 1. Komponenten

| Bereich         | Komponente                                   |
| --------------- | -------------------------------------------- |
| Mikrocontroller | ESP32-S3-N16R8                               |
| Eingabe         | Joystick-Modul mit VRX und SW                |
| Lenkung         | Servo                                        |
| Antrieb         | DC-Motor über Motortreiber / Motor-Ausgang 1 |
| Abstandsmessung | HC-SR04 Ultraschallsensor                    |
| Audio           | MAX98357A I2S-Audiomodul mit Lautsprecher    |

---

## 2. Verhalten des Autos

### 2.1 Joystick-Steuerung

| Aktion                     | Reaktion                |
| -------------------------- | ----------------------- |
| Joystick nach links        | Servo lenkt nach links  |
| Joystick nach rechts       | Servo lenkt nach rechts |
| Joystick in der Mitte      | Servo stoppt            |
| Joystick-Knopf gedrückt    | DC-Motor fährt vorwärts |
| Joystick-Knopf losgelassen | DC-Motor stoppt         |

---

### 2.2 Hinderniserkennung

| Situation                  | Verhalten                                                                  |
| -------------------------- | -------------------------------------------------------------------------- |
| Abstand grösser als 3 cm   | Das Auto kann normal fahren                                                |
| Abstand 3 cm oder weniger  | Der Motor stoppt automatisch                                               |
| Nach dem Stopp             | Das Auto fährt 2 Sekunden rückwärts                                        |
| Während der Rückwärtsfahrt | Der Lautsprecher macht `peep peep`                                         |
| Nach der Rückwärtsfahrt    | Das Auto bleibt stehen                                                     |
| Weiterfahren               | Erst möglich, wenn der Joystick-Knopf losgelassen und erneut gedrückt wird |

---

### 2.3 Ablauf bei Hindernis

1. Der Ultraschallsensor misst den Abstand nach vorne.
2. Ist der Abstand grösser als **3 cm**, bleibt die normale Joystick-Steuerung aktiv.
3. Ist der Abstand **3 cm oder kleiner**, wird der Vorwärtsmotor gestoppt.
4. Das Auto fährt **2 Sekunden rückwärts**.
5. Währenddessen wird über das MAX98357A-Modul ein Rückwärts-Piepser ausgegeben.
6. Danach bleibt das Auto stehen.
7. Das Auto wird erst wieder freigegeben, wenn der Joystick-Knopf losgelassen und erneut gedrückt wird.

---

## 3. Pinbelegung

### 3.1 Joystick

| Joystick Pin | ESP32-S3-N16R8 / Anschluss |
| ------------ | -------------------------- |
| Joystick GND | GND                        |
| Joystick 5V  | 5V                         |
| Joystick VRX | IO4                        |
| Joystick SW  | IO2                        |
| Joystick VRY | Nicht benutzt              |

---

### 3.2 Servo

| Servo Pin    | ESP32-S3-N16R8 / Anschluss |
| ------------ | -------------------------- |
| Servo Signal | IO37                       |
| Servo 5V     | 5V                         |
| Servo GND    | GND                        |

---

### 3.3 DC-Motor 1

| Motortreiber Pin | ESP32-S3-N16R8 Pin |
| ---------------- | ------------------ |
| Motor 1 IN1      | IO15               |
| Motor 1 IN2      | IO16               |

---

### 3.4 Ultraschallsensor HC-SR04

| HC-SR04 Pin  | ESP32-S3-N16R8 / Anschluss |
| ------------ | -------------------------- |
| HC-SR04 VCC  | 5V                         |
| HC-SR04 GND  | GND                        |
| HC-SR04 TRIG | IO17                       |
| HC-SR04 ECHO | IO35                       |

---

### 3.5 MAX98357A Audiomodul

| MAX98357A Pin  | ESP32-S3-N16R8 / Anschluss |
| -------------- | -------------------------- |
| MAX98357A VIN  | 5V                         |
| MAX98357A GND  | GND                        |
| MAX98357A SD   | IO39                       |
| MAX98357A DIN  | IO40                       |
| MAX98357A BCLK | IO41                       |
| MAX98357A LRC  | IO42                       |
| MAX98357A GAIN | Frei lassen                |

---

### 3.6 Lautsprecher

| Lautsprecher-Kabel   | Anschluss                                      |
| -------------------- | ---------------------------------------------- |
| Lautsprecher rot     | Plus am grünen Anschluss des MAX98357A-Moduls  |
| Lautsprecher schwarz | Minus am grünen Anschluss des MAX98357A-Moduls |

---

## 4. Wichtige Werte im Code

| Konstante          |   Wert | Bedeutung                          |
| ------------------ | -----: | ---------------------------------- |
| `SERVO_PIN`        |   `37` | Signalpin für den Servo            |
| `JOYSTICK_X_PIN`   |    `4` | Joystick-X-Achse / VRX             |
| `JOYSTICK_SW_PIN`  |    `2` | Joystick-Knopf                     |
| `DRIVE_MOTOR`      |    `1` | Motor-Ausgang für den Antrieb      |
| `DRIVE_SPEED`      |  `100` | Geschwindigkeit vorwärts           |
| `REVERSE_SPEED`    |   `25` | Geschwindigkeit rückwärts          |
| `STOP_DISTANCE_CM` |  `3.0` | Stopp-Abstand vor Hindernissen     |
| `REVERSE_TIME_MS`  | `2000` | Rückwärtsfahrzeit in Millisekunden |

```cpp
#define SERVO_PIN 37
#define JOYSTICK_X_PIN 4
#define JOYSTICK_SW_PIN 2

#define DRIVE_MOTOR 1
#define DRIVE_SPEED 100
#define REVERSE_SPEED 25

#define STOP_DISTANCE_CM 3.0
#define REVERSE_TIME_MS 2000
```

---

## 5. Rückwärts-Piepser

| Konstante                     |  Wert | Bedeutung                      |
| ----------------------------- | ----: | ------------------------------ |
| `REVERSE_BEEP_FREQUENCY`      | `900` | Tonfrequenz des Piepsers in Hz |
| `REVERSE_BEEP_DURATION_MS`    | `180` | Dauer eines Piepsers           |
| `REVERSE_BEEP_SHORT_PAUSE_MS` | `120` | Kurze Pause zwischen Piepsen   |
| `REVERSE_BEEP_GROUP_PAUSE_MS` | `450` | Pause zwischen Piep-Gruppen    |

```cpp
#define REVERSE_BEEP_FREQUENCY 900
#define REVERSE_BEEP_DURATION_MS 180
#define REVERSE_BEEP_SHORT_PAUSE_MS 120
#define REVERSE_BEEP_GROUP_PAUSE_MS 450
```

Das Piep-Muster klingt ungefähr so:

```text
peep peep ... peep peep ... peep peep
```

---

## 6. Hinweis zur Pinwahl

Der Joystick-Knopf ist auf **IO2** gelegt, weil **IO41** vom MAX98357A-Audiomodul als **BCLK** benutzt wird.

Dadurch gibt es keinen Konflikt zwischen Joystick und Lautsprecher.

---

## 7. Checkliste vor dem Test

| Prüfung                                                                             | Status |
| ----------------------------------------------------------------------------------- | ------ |
| Alle GND-Leitungen sind miteinander verbunden                                       | ☐      |
| Der HC-SR04-ECHO-Pin ist mit IO35 verbunden                                         | ☐      |
| Servo und Motor sind korrekt mit 5V und GND verbunden                               | ☐      |
| Der Joystick-Knopf hängt an IO2 und nicht an IO41                                   | ☐      |
| MAX98357A BCLK liegt auf IO41                                                       | ☐      |
| MAX98357A DIN liegt auf IO40                                                        | ☐      |
| MAX98357A LRC liegt auf IO42                                                        | ☐      |
| Der Lautsprecher ist korrekt am grünen Anschluss des MAX98357A-Moduls angeschlossen | ☐      |

---

## 8. Arbeit während dem Unterricht

Während dem Unterricht haben wir an unserem ESP32-S3 gearbeitet. Wir haben den Aufbau geplant, die Bauteile angeschlossen, den Code getestet und Fehler gesucht.

Zu den wichtigsten Arbeiten gehörten:

| Aufgabe        | Beschreibung                                                                    |
| -------------- | ------------------------------------------------------------------------------- |
| Planung        | Überlegen, welche Bauteile gebraucht werden und wie das Auto funktionieren soll |
| Aufbau         | ESP32-S3-N16R8, Joystick, Servo, Motor, Sensor und Audiomodul anschliessen      |
| Programmierung | Code verstehen, anpassen und testen                                             |
| Fehlersuche    | Pins kontrollieren, Verkabelung prüfen und Logikfehler suchen                   |
| Dokumentation  | Projekt beschreiben und technische Informationen festhalten                     |

Spannend war vor allem, dass der Code direkt echte Hardware steuert. Besonders interessant war die Hinderniserkennung, weil das Auto dadurch automatisch reagieren konnte. Es konnte ein Hindernis erkennen, stoppen, rückwärts fahren und dabei ein Peep-Peep-Geräusch ausgeben.

---

## 9. Hilfsmittel für das Projekt

Für das Projekt wurden verschiedene Hilfsmittel verwendet. Dazu gehörten Software, Hardware und weitere Ressourcen.

| Bereich            | Verwendete Hilfsmittel                                                        |
| ------------------ | ----------------------------------------------------------------------------- |
| Software           | C++ Einführung, Blockly und Software-Paket für den ESP32-S3-N16R8             |
| Hardware           | ESP32-S3-N16R8, Platine von der BWS, Kabel und elektronische Bauteile         |
| Wissen             | Erklärungen der Lehrperson, Unterrichtsmaterial, Beispiele und eigene Notizen |
| Weitere Ressourcen | Teamarbeit und Internetrecherche                                              |

Die Hardware-Bauteile selbst sind in der Komponentenliste weiter oben bereits beschrieben. Deshalb werden sie hier nicht nochmals vollständig wiederholt.

---

## 10. Projekt- und Code-Struktur

Das Projekt wurde mit **C++** für den **ESP32-S3-N16R8** umgesetzt. Der Code ist so aufgebaut, dass zuerst wichtige Pins und Werte definiert werden. Danach werden im Setup-Bereich die Bauteile vorbereitet. Im Loop-Bereich läuft die Hauptlogik immer wieder durch.

Die grobe Struktur sieht so aus:

| Codeteil         | Aufgabe                                                               |
| ---------------- | --------------------------------------------------------------------- |
| Konstanten       | Speichern Pins, Geschwindigkeiten, Abstände und Zeiten                |
| Setup            | Bereitet Pins, Servo, Motor, Sensor und Audio vor                     |
| Loop             | Führt die Hauptlogik dauerhaft aus                                    |
| Steuerlogik      | Entscheidet, ob das Auto fahren, stoppen oder rückwärts fahren soll   |
| Sicherheitslogik | Verhindert, dass das Auto nach einem Hindernis sofort wieder losfährt |

Der Code verbindet Eingabe, Bewegung, Sensorik und Audio zu einem gemeinsamen Ablauf.

---

## 11. Funktionsweise des Codes

Der Code liest zuerst die Werte des Joysticks. Dadurch erkennt das Programm, ob nach links, nach rechts oder geradeaus gelenkt werden soll. Der Servo wird entsprechend angesteuert.

Der Joystick-Knopf wird als Gas-Taster benutzt. Wenn der Knopf gedrückt wird, darf der Motor vorwärts fahren. Wenn der Knopf losgelassen wird, stoppt der Motor.

Gleichzeitig misst der Ultraschallsensor den Abstand nach vorne. Wenn kein Hindernis zu nah ist, bleibt die normale Steuerung aktiv. Wenn ein Hindernis erkannt wird, stoppt der Code den Motor automatisch. Danach wird eine Rückwärtsfahrt ausgelöst. Während dieser Rückwärtsfahrt wird über das Audiomodul ein Peep-Peep-Signal ausgegeben.

Nach der Rückwärtsfahrt bleibt das Auto stehen. Damit es nicht sofort wieder losfährt, muss der Joystick-Knopf zuerst losgelassen und danach erneut gedrückt werden.

---

## 12. Hauptfunktionen und Schwierigkeiten

Die Hauptfunktionen vom Code sind die Lenkung, die Motorsteuerung, die Hinderniserkennung und der Rückwärts-Piepser.

| Hauptfunktion    | Erklärung                                        |
| ---------------- | ------------------------------------------------ |
| Joystick lesen   | Erkennt die Eingabe für Lenkung und Gas          |
| Servo steuern    | Bewegt die Lenkung nach links, rechts oder Mitte |
| Motor steuern    | Startet, stoppt oder fährt rückwärts             |
| Abstand messen   | Erkennt Hindernisse vor dem Auto                 |
| Sicherheitsstopp | Stoppt das Auto automatisch bei einem Hindernis  |
| Rückwärtsfahrt   | Fährt nach dem Stopp kurz rückwärts              |
| Piepser          | Gibt beim Rückwärtsfahren ein Warnsignal aus     |

Schwierig war vor allem die richtige Verbindung der Pins. Wenn ein Pin falsch angeschlossen ist oder doppelt benutzt wird, funktioniert das Projekt nicht richtig. Ein Problem war auch, dass bestimmte Pins bereits vom Audiomodul gebraucht wurden. Deshalb musste der Joystick-Knopf auf einen anderen Pin gelegt werden.

Auch die Kombination aus Motorsteuerung, Abstandsmessung und Sicherheitslogik war anspruchsvoll. Der Code musste so funktionieren, dass das Auto bei einem Hindernis stoppt, rückwärts fährt und danach nicht sofort wieder automatisch weiterfährt.

---

## 13. Fazit

Durch dieses Projekt haben wir gelernt, wie man einen ESP32-S3-N16R8 mit verschiedenen Bauteilen verbindet und programmiert. Wir haben gesehen, wie ein Joystick als Eingabe benutzt wird, wie ein Servo die Lenkung übernimmt, wie ein Motor angesteuert wird und wie ein Ultraschallsensor Hindernisse erkennen kann.

Wichtig war auch die Erkenntnis, dass Hardware-Projekte nicht nur aus Code bestehen. Die Verkabelung, die Stromversorgung und die richtige Pinbelegung sind genauso wichtig. Schon ein falscher Pin oder ein falsch angeschlossenes Kabel kann dazu führen, dass das ganze Projekt nicht funktioniert.

Schwierig war, dass C++ am Anfang nicht einfach zu verstehen war.

Trotzdem war das Projekt sinnvoll, weil man direkt sehen konnte, wie Programmierung und Elektronik zusammenarbeiten. Das Auto war ein gutes Beispiel dafür, wie aus Code, Sensoren und Motoren ein funktionierendes System entsteht.

---

## Autoren

* Dion
* Kuzey
* Baran
