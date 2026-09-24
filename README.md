# Smart Gate System (HC-SR04 + Servo + I2C LCD)

An Arduino-based automatic barrier gate. An **HC-SR04 ultrasonic sensor** detects approaching objects, a **servo motor** opens and closes the gate, and a **16x2 I2C LCD** shows the live distance and gate status.

## Features

- **Automatic gate** – the servo rotates to 90° (OPEN) when an object comes within 15 cm, then returns to 0° (CLOSED) after a short delay.
- **Real-time distance sensing** – measures object distance continuously in centimeters.
- **I2C LCD display** – shows the distance and gate status (`Gate: OPEN` / `Gate: CLOSED`) using only 2 data wires.
- **Out-of-range guard** – readings below 2 cm or above 400 cm are shown as `Out Range`.

## Hardware Components

| Component             | Qty | Description                              |
|-----------------------|:---:|------------------------------------------|
| Arduino Uno / Nano    | 1   | Microcontroller board                    |
| HC-SR04 sensor        | 1   | Ultrasonic distance sensor               |
| SG90 servo motor      | 1   | Micro servo that drives the gate barrier |
| 16x2 I2C LCD display  | 1   | LCD module with PCF8574 I2C adapter      |
| Breadboard & wires    | –   | Jumper wires for connections             |

## Wiring

### Circuit Diagram

![Smart Gate System pictorial wiring diagram](servo-get-system-Arduino-ultrasonic%20Public%20pictorial%20wiring%20diagram.png)

The diagram shows the system powered by a 2-cell battery pack through an on/off switch and a buck converter set to 5V.

### Pin Summary

```
                 +-------------------------------+
                 |       Arduino Uno / Nano      |
                 |  5V  GND  D9  D8  D6  A4  A5  |
                 +--+----+----+---+---+---+---+--+
                    |    |    |   |   |   |   |
   HC-SR04  VCC ----+    |    |   |   |   |   |
            GND ---------+    |   |   |   |   |
            TRIG -------------+   |   |   |   |
            ECHO -----------------+   |   |   |
   SG90     SIG ----------------------+   |   |
   LCD      SDA --------------------------+   |
            SCL ------------------------------+

   (VCC of the servo and LCD -> 5V, GND of all modules -> GND)
```

### 1. HC-SR04 Ultrasonic Sensor

| Sensor Pin | Arduino Pin |
|------------|-------------|
| VCC        | 5V          |
| GND        | GND         |
| Trig       | D9          |
| Echo       | D8          |

### 2. SG90 Servo Motor

| Wire Color      | Function | Arduino Pin                                   |
|-----------------|----------|-----------------------------------------------|
| Red             | VCC      | 5V (use an external 5V supply for bigger servos) |
| Brown / Black   | GND      | GND                                           |
| Orange / Yellow | Signal   | D6                                            |

### 3. 16x2 I2C LCD

| LCD Pin | Arduino Pin |
|---------|-------------|
| VCC     | 5V          |
| GND     | GND         |
| SDA     | A4          |
| SCL     | A5          |

## How It Works

1. **Distance measurement** – the Arduino sends a 10 µs pulse to the sensor's Trig pin and measures how long the echo takes to come back (`t`, in µs). The distance is then:

   ```
   distance (cm) = t × 0.0343 / 2
   ```

   (0.0343 cm/µs is the speed of sound; divided by 2 because the sound travels there and back.)

2. **Gate logic**
   - If distance ≤ 15 cm → the servo rotates to **90°** and the LCD shows `Gate: OPEN`.
   - If distance > 15 cm → the gate stays open for **1.5 s**, then the servo returns to **0°** and the LCD shows `Gate: CLOSED`.

## Code

The sketch is in [smart_gate/smart_gate.ino](smart_gate/smart_gate.ino).

Settings you can change at the top of the sketch:

| Setting                 | Default | Meaning                          |
|-------------------------|---------|----------------------------------|
| `lcd(0x27, 16, 2)`      | `0x27`  | LCD I2C address (try `0x3F`)     |
| `trigPin`               | `9`     | Ultrasonic Trig pin              |
| `echoPin`               | `8`     | Ultrasonic Echo pin              |
| `servoPin`              | `6`     | Servo signal pin                 |
| `openDistance`          | `15`    | Distance in cm that opens the gate |

## How to Run

1. Open the Arduino IDE.
2. Install the **LiquidCrystal_I2C** library (*Sketch → Include Library → Manage Libraries…*). `Wire` and `Servo` come with the IDE.
3. Wire all components as shown in the [Wiring](#wiring) section.
4. Open `smart_gate/smart_gate.ino`.
5. Connect the Arduino by USB, select your board and COM port, then click **Upload**.

## Troubleshooting

- **Servo jitters or the board resets** – the servo is drawing too much current. Power it from an external 5V source and connect its GND to the Arduino GND.
- **LCD is blank or shows white blocks** – turn the contrast potentiometer on the back of the I2C module. If it's still blank, change the address in the code from `0x27` to `0x3F`.
- **Gate doesn't open** – make sure the object is closer than 15 cm, and check the Trig (D9) and Echo (D8) wiring.
