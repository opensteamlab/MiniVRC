#include "Controller.h"
// You should not change this function, if only you use different board (If you really want to change, you can change the pins in Configs.h)
void setup(){
    initialize(); 
    setupPS2controller();
}

void loop()
{   
    ps2x.read_gamepad(false, false); // Read the gamepad state, It's compulsory if you want to use the PS2 controller
    controller();
    delay(50);
