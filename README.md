# Joystick Servo & DC Motor Steuerung

Dieses Projekt steuert einen Servo und einen DC-Motor mit einem Joystick.

## Funktion

Der Joystick übernimmt zwei Aufgaben:

- Die X-Achse steuert den Servo.
- Der Joystick-Knopf steuert den DC-Motor.

Wenn der Joystick nach links oder rechts bewegt wird, dreht sich der Servo in die jeweilige Richtung. In der Mittelstellung bleibt der Servo stehen.

Wenn der Joystick-Knopf gedrückt wird, läuft der DC-Motor. Sobald der Knopf losgelassen wird, stoppt der Motor wieder.

## Einsatz

Der Code kann zum Beispiel für ein kleines Fahrzeug oder ein Testmodell verwendet werden:

- Servo für die Lenkung
- DC-Motor für den Antrieb
- Joystick für die Steuerung

## Verhalten

- Joystick links: Servo dreht in eine Richtung
- Joystick rechts: Servo dreht in die andere Richtung
- Joystick mittig: Servo stoppt
- Joystick-Knopf gedrückt: Motor läuft
- Joystick-Knopf losgelassen: Motor stoppt

## Hinweis

Beim Start wird die Mittelstellung des Joysticks automatisch kalibriert. Deshalb sollte der Joystick beim Einschalten nicht bewegt werden.
