#include "my_lib.c"

void Init_Servo(void){
  *AT91C_PMC_PCER1 = (1 << 4);//periphrial enable PWM
  *AT91C_PMC_PCER = (1 << 12);// -||- PIOB
  *AT91C_PIOB_PDR = (1 << 17);//disable B.17
  *AT91C_PIOB_ABMR = (1 << 17);
  *AT91C_PWMC_ENA = (1 << 1);
  *AT91C_PWMC_CH1_CMR = 0x5;
  
  *AT91C_PWMC_CH1_CPRDR = (int)(20.0 * 2625); //20 ms
  *AT91C_PWMC_CH1_CDTYR = (int)(0.7 * 2625); //1 ms
}

void turn(int key){
  int degree = 2625 * (((((key-1) * 10) * 1.933) / 180)+ 0.62);//pulse width modulation formula
 (*AT91C_PWMC_CH1_CDTYR) = degree;
}

void turn_Servo(){  
  int key = func();
  switch(key){
    case 0:
      //(*AT91C_PWMC_CH1_CDTYR) = (int)(2625.0 * 0.7);
      (*AT91C_PWMC_CH1_CDTYR) = (int)(2625.0 * 0.55);
      return;
    case 1:
      turn(1);
      break;
    case 2:
      turn(2);
      break;
  case 3:
    turn(3);
    break;
  case 4:
    turn(4);
    break;
  case 5:
    turn(5);
    break;
  case 6:
    turn(6);
    break;
  case 7:
    turn(7);
    break;
  case 8:
    turn(8);
    break;
  case 9:
    turn(9);
    break;
  case 10:
    turn(10);
    break;
  case 11:
    turn(11);
    break;
  case 12:
    turn(12);
    break; 
  }
}