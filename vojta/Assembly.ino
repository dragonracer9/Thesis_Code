//#define ASTERISK

#define VRX_PIN  A1
#define VRY_PIN  A0
#define SW_PIN 2
#define TONE_PIN 3

#define MOTOR0 0
#define MOTOR0_PIN0 11
#define MOTOR0_PIN1 10
#define MOTOR0_PIN2 9
#define MOTOR0_PIN3 8

#define MOTOR1 1
#define MOTOR1_PIN0 7
#define MOTOR1_PIN1 6
#define MOTOR1_PIN2 5
#define MOTOR1_PIN3 4

#define CUTOFF 400 //cutoff for joystick

typedef enum Direction {
  FORWARDS,
  NONE,
  BACKWARDS,
};

class MotorReg {  
  public:
#ifdef ASTERISK
    unsigned char bytes = 0b11001000;
#else
    unsigned char bytes = 0b1000;
#endif

    MotorReg(unsigned char bytes) : bytes(bytes) {}

    void step_forwards(){
#ifdef ASTERISK
      bytes = ((bytes & 0x0E) << 3) | ((bytes & 0xF0) >> 4) | ((bytes & 0x01) << 7) & 0xFF;
#else
      bytes = ((bytes & 0xE) >> 1) | ((bytes & 0x1) << 3) & 0xF;
#endif
    }

    void step_backwards(){
#ifdef ASTERISK
      bytes = ((bytes & 0x07) << 5) | ((bytes & 0xF0) >> 4) | ((bytes & 0x08) << 1) & 0xFF;
#else
      bytes = ((bytes & 0x7) << 1) | ((bytes & 0x8) >> 3) & 0xF;
#endif
    }

};

class Motor {
  private:
    pin_size_t pin0;
    pin_size_t pin1;
    pin_size_t pin2;
    pin_size_t pin3;
  
  public:
    Motor() {}
    
    void __init__(pin_size_t p0, pin_size_t p1, pin_size_t p2, pin_size_t p3) {
      pin0 = p0;
      pin1 = p1;
      pin2 = p2;
      pin3 = p3;

      pinMode(pin0, OUTPUT);
      pinMode(pin1, OUTPUT);
      pinMode(pin2, OUTPUT);
      pinMode(pin3, OUTPUT);
    }
    
    void write(unsigned int bytes) {
      digitalWrite(pin0, bytes>>3 & 1);
      digitalWrite(pin1, bytes>>2 & 1);
      digitalWrite(pin2, bytes>>1 & 1);
      digitalWrite(pin3, bytes    & 1);
    }

    void clean() {
      this->write(0);
    }
};

class Assembly {
  private:
    MotorReg motor_reg0 = MotorReg(0b1100);
    MotorReg motor_reg1 = MotorReg(0b1100);

    Motor motor0;
    Motor motor1;

    unsigned int wait_time; //in ms (3 is optimal for noise reduction)
    int clean_time; //in ms (0 this is pretty good for cooling (we didnt actually do all the tests lolws))
  
  public:
    Assembly() {}

    void __init__(Motor m0, Motor m1, unsigned int wt, int ct) {
      motor0 = m0;
      motor1 = m1;
      wait_time = wt;
      clean_time = ct;
    }
    void clean() {
      motor0.clean();
      motor1.clean();
    }

    void step(Direction dir0, Direction dir1) {
      if (dir0 != Direction::NONE) {
        if (dir0 == Direction::FORWARDS) {
          motor_reg0.step_forwards();
        } else if (dir0 == Direction::BACKWARDS) {
          motor_reg0.step_backwards();
        } 

        motor0.write(motor_reg0.bytes);
      }
      
      if (dir1 != Direction::NONE) {
        if (dir1 == Direction::FORWARDS) {
          motor_reg1.step_forwards();
        } else if (dir1 == Direction::BACKWARDS) {
          motor_reg1.step_backwards();
        } 
        
        motor1.write(motor_reg1.bytes);
      }

      delay(wait_time);
      if (clean_time >= 0) clean();
      delay(clean_time);
    }

    void step_n(unsigned int n, Direction dir0, Direction dir1){
      for (unsigned int i=0; i<n; i++)
        step(dir0, dir1);
      
      clean();
    }
};

class Joystick {
  private: 
    pin_size_t x_pin;
    pin_size_t y_pin;

  public:
    int x_value = 0; // To store value of the X axis
    int y_value = 0; // To store value of the Y axis
  
    Joystick() {}

    void __init__(pin_size_t xp, pin_size_t yp) {
      x_pin = xp;
      y_pin = yp;

      pinMode(x_pin, INPUT);
      pinMode(y_pin, INPUT);
    }

    void update() {
      x_value = analogRead(x_pin);
      y_value = analogRead(y_pin);
    }
};

Assembly assembly;
Motor __motor0;
Motor __motor1;

Joystick joystick;

unsigned int freq = 73000;
unsigned int tone_on = false;

void __t() {
  if (!tone_on) {
    tone(TONE_PIN, freq);
    tone_on = true;
  }
  else {
    noTone(TONE_PIN);
    tone_on = false;
  }
}

void setup() {
  pinMode(SW_PIN, INPUT_PULLUP);
  attachInterrupt(SW_PIN, __t, RISING);

  pinMode(TONE_PIN, OUTPUT);
  digitalWrite(TONE_PIN, LOW);

  __motor0.__init__(MOTOR0_PIN0, MOTOR0_PIN1, MOTOR0_PIN2, MOTOR0_PIN3);
  __motor1.__init__(MOTOR1_PIN0, MOTOR1_PIN1, MOTOR1_PIN2, MOTOR1_PIN3);

  assembly.__init__(__motor0, __motor1, 2, -1);

  joystick.__init__(VRX_PIN, VRY_PIN);

  Serial.begin(9600);
}

void loop() {
  joystick.update();

  Direction dir0 = Direction::NONE;
  Direction dir1 = Direction::NONE;
  if (joystick.x_value < CUTOFF) {
    dir0 = Direction::BACKWARDS;
  }
  else if (joystick.x_value > 1023 - CUTOFF) {
    dir0 = Direction::FORWARDS;
  }

  if (joystick.y_value < CUTOFF) {
    dir1 = Direction::FORWARDS;
  }
  else if (joystick.y_value > 1023 - CUTOFF) {
    dir1 = Direction::BACKWARDS;
  }

  assembly.step_n(10, dir0, dir1);
}
