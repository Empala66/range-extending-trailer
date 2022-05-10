// define the pins for input and output
const int pinin_accelerator = A0;
const int pinin_brake = 13;
const int pinout_accelerator_pwm = 3;
const int pinout_brake_relay = 4;

// declare variables for reading input states
int pinstate_accelerator = 0;
int pinstate_brake = 0;

//declare variables for outputting pwm values (mapping abalog pins (0 - 1023) to pwm outputs (0 - 255)
int pwmmap_accelerator = 0;


// setup is run once, before the main loop
void setup() {
  //enable serial for debugging
  Serial.begin(9600);
  
  //setup the input pins
  pinMode(pinin_accelerator, INPUT);
  pinMode(pinin_brake, INPUT);

  //setup the output pins
  pinMode(pinout_accelerator_pwm, OUTPUT);
  pinMode(pinout_brake_relay, OUTPUT);
}



// main loop is run continuously until system powers off
void loop() {
  readInputPins();
  outputPWM();
  delay(1);
}



// add all input pins to be read every loop here
void readInputPins(){
  //read the input pins
  pinstate_accelerator = analogRead(pinin_accelerator);
  pinstate_brake = digitalRead(pinin_brake);
  
  //output debug values for accelerator pressure (analog)
  Serial.println(pinstate_accelerator);
}




// This function decides output required for pwm, and outputs it to the appropriate pinouts
void outputPWM(){
  // map the analog value of the pressure sensor (between 0 and 1023) to pwm values (between 0 and 255)
  pwmmap_accelerator = map(pinstate_accelerator, 0, 1023, 0, 255);

  // Engage or disengage the contactors for battery regen, based on whether the brake is engaged or not
  digitalWrite(pinout_brake_relay, pinstate_brake);

  // output the pulse width modulated signal for the motor - this will go to IGBT or heavy mosfet to 
  // engage power between battery and  motor.  Duty cycle will increase as pressure on accelerator increases
  // note: ignore noise (small values for the accelerator pressure sensor)
  // and don't accelerate if the brake is engaged
  if ((pwmmap_accelerator > 20)&&(pinstate_brake==0)) {
    analogWrite(pinout_accelerator_pwm, pwmmap_accelerator);
  } else {
    analogWrite(pinout_accelerator_pwm, 0);
  }

}
