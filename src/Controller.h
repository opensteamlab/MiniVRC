#include <Configs.h>

int y = 128 ; //If it works well, leave it as it is
int x = 127 ; // Similar to y, leave it as it is
int pulse = 4095; // Pulse value for the motors, can be adjusted based on the board you are using (e.g., 4095 for PCA9685, 255 for Arduino PWM)
double power = 0.7; // Change this value to adjust speed of the motors

//ALWAYS USING THIS FUNCTION IN SETUP FUNCTION
void setupPS2controller()
{
    int error = -1 ;
    while(error != 0)
    {
        error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, true, true);
        Serial.println(error);
    }
}

//Set power and direction for DC motors, if you want to change the direction, just swap the pins in your main code
//Don't touch this function, just change the parameters in your main code
void setPower(int pin1,int pin2,int x)
{
    pwm.setPWM(pin1,0,x);
    pwm.setPWM(pin2,0,0);
}

// How to use this function to set the power and dirction of your own DC motors
// setPower(pin1_dc1,pin2_dc1,x); // For the first DC motor
// setPower(pin1_dc2,pin2_dc2,x); // For the second DC motor (Optional, if you have to control two DC Motors in your mechanism)
// You can add "setPower(pin1_dc1,pin2_dc1,x);" in your functions to make the code more readable and organized
// The example that you can see in the move_forward or other functions below


//Servo
// Functions to set the position of the position servo 
void setPosition(int pin, int pos)
{
    constrain(pos, 500, 2500);
    pwm.writeMicroseconds(pin,pos);
}
// Functions to set the power and direction of the contunious servo
void setServoPower(int pin, int x){
    constrain(x, 500, 2500); 
    pwm.writeMicroseconds(pin, x);
}

//Base
// x = pulse * power; 
// Only change x in your main code, it will set the speed of the motors 
void move_forward(int x)
{
    setPower(pin1_dc1,pin2_dc1,x);
    setPower(pin2_dc4,pin1_dc4,x);
}
void turn_left(int x)
{
    setPower(pin1_dc1,pin2_dc1,x);
    setPower(pin1_dc4,pin2_dc4,x);
}
void turn_right(int x)
{
    setPower(pin2_dc4,pin1_dc4,x);
    setPower(pin2_dc1,pin1_dc1,x);
}
void move_backward(int x)
{
    setPower(pin2_dc1,pin1_dc1,x);
    setPower(pin1_dc4,pin2_dc4,x);
}
void stop()
{
    setPower(pin1_dc1,pin2_dc1,0);
    setPower(pin2_dc4,pin1_dc4,0);
}

//Intake
// x = pulse * power; 
// Only change x in your main code, it will set the speed of the motors 
void intake_run(int x)
{
    setPower(pin1_dc3,pin2_dc3,x);
}
void intake_stop()
{
    setPower(pin1_dc3,pin2_dc3,0);
}
void intake_reverse(int x)
{
    setPower(pin2_dc3,pin1_dc3,x);
}

//Climber
// x = pulse * power; 
// Only change x in your main code, it will set the speed of the motors 
void climber_up(int x)
{
    setPower(pin1_dc2,pin2_dc2,x);
}
void climber_down(int x) 
{
    setPower(pin2_dc2,pin1_dc2,x);
}
void climber_stop()
{
    setPower(pin1_dc2,pin2_dc2,0);
}

//Control
// The main code to control the robot
void controller()
{   

    //Joystick Setting Up

    int ly = ps2x.Analog(PSS_LY);
    int rx = ps2x.Analog(PSS_RX);

    if(ps2x.Button(PSB_L2))
    {
        rx = ps2x.Analog(PSS_LX);
    }
    else if(ps2x.Button(PSB_R2))
    {
        rx = ps2x.Analog(PSS_RX);
    }
    
    int ly_raw = y - ly;
    int rx_raw = x - rx; 
    int ly_val = abs(ly_raw);
    int rx_val = abs(rx_raw);
    int ly_po = map(ly_val,0,128,0,4095);
    int rx_po = map(rx_val,0,128,0,4095);

    //Base Control
    if(ly_raw > 0)
    {
        move_forward(ly_po);
    }
    else if(ly_raw < 0)
    {
        move_backward(ly_po);
    }
    else if(rx_raw > 0)
    {
        turn_left(rx_po);
    }
    else if(rx_raw < 0)
    {
        turn_right(rx_po);
    }
    else
    {
        stop();
    }
    
    // The example of the main code to control intake using the PS2 controller
    // You can change the buttons to control the intake, climber, and base
    // You can also change the power of the motors by changing the power variable
    // You can follow this link https://via.makerviet.org/vi/docs/3_robotics-with-via/4_gamepad-with-via/ to find out the suitable buttons for your player
    //Intake Control
    if (ps2x.NewButtonState(PSB_TRIANGLE))
    {
        intake_reverse(pulse * power);
    }
    else if(ps2x.NewButtonState(PSB_CIRCLE))
    {
        intake_run(pulse * power); 
    }
    else if(ps2x.Button(PSB_L1))
    {
        intake_stop();
    }

    //Climber Control
    if(ps2x.Button(PSB_PAD_UP))
    {
        climber_up(pulse * power);
    }
    else if(ps2x.Button(PSB_PAD_DOWN))
    {
        climber_down(pulse * power);
    }
    else if(ps2x.Button(PSB_CROSS))
    {
        climber_up(pulse);
    }
    else if(ps2x.Button(PSB_SQUARE))
    {
        climber_down(pulse);
    }
    else
    {
        climber_stop();
    }

}
