#include "my_lib.c"


unsigned int diff_time;
int flag = 0;
int temp_flag = 0;



void TC0_Handler(){
   (*AT91C_TC0_IDR) = 0x40; //Disable interrupt for LDRBS 0x40h = 1000000b (bit 6)
   temp_flag = 1;       
}

void Init_TimerTemp(){
  *AT91C_PMC_PCER = (1 << 12);//PMC
  *AT91C_PMC_PCER = (1 << 5);//PMC
  *AT91C_PMC_PCER = (1 << 27);//PMC
  
  *AT91C_TC0_CMR = (*AT91C_TC0_CMR & 0xFFF0FFF8)|(1 << 17)|(1<<18);
  *AT91C_TC0_CCR = 0x5; 
  //Puls
  
  *AT91C_PIOB_OER = (1 << 25);
  *AT91C_PIOB_SODR = (1 << 25);
  *AT91C_PIOB_CODR = (1 << 25);
  //Delay(15);
  dely = 0;
  while(dely >= 15);
  *AT91C_PIOB_SODR = (1 << 25);
  NVIC_ClearPendingIRQ(TC0_IRQn);//interrupts for temp sensor
  NVIC_SetPriority(TC0_IRQn, 2);
  NVIC_EnableIRQ(TC0_IRQn);
  
  *AT91C_TC0_SR;
}


void startMeas1(){
  
  //Reset pulse
  *AT91C_PIOB_OER = (1 << 25);
  //*AT91C_PIOB_SODR = (1 << 25);
  *AT91C_PIOB_CODR = (1 << 25); 
}

void startMeas2(){
  
  //Delay(15);
  *AT91C_PIOB_SODR = (1 << 25);
  *AT91C_PIOB_ODR = (1 << 25);
  
  (*AT91C_TC0_CCR) = 0x4;  //Making a sw_reset in TC0_CCR0 0x4h = 100b (bit 2)
  *AT91C_TC0_SR;
  (*AT91C_TC0_IER) = 0x40; //Enable interrupt for LDRBS 0x40h = 1000000b (bit 6)
  
  //Start pulse
  *AT91C_PIOB_OER = (1 << 25);
  *AT91C_PIOB_CODR = (1 << 25);
  Delay(25);
  *AT91C_PIOB_SODR = (1 << 25);
  *AT91C_PIOB_ODR = (1 << 25);
}


float get_Temperature(){
    //flags in order to read the TC0_Handler first before moving on
  if(temp_flag == 1){
    temp_flag = 0;
    unsigned int RB = (*AT91C_TC0_RB);//same for B
    unsigned int RA = (*AT91C_TC0_RA);//capture register A
    unsigned int time = RB-RA;
    Delay(15);
    temperature = (time/(42.0*5.0)-273.15); //temperature calc

    return temperature;
  }
  return 0;
    
}

void check_alarm(float check_temp){
  if(temp_alarm(check_temp) == 1)
    blinking_alarm(1);
  if(temp_alarm(check_temp) == 2)
    blinking_alarm(2); 
  current_temperature = (upp_limit + low_limit) / 2;
}

int temp_alarm(float temperature){
  if(temperature > upp_limit){
    return 1;
  }
  if(temperature < low_limit){
    return 2;
  }
  return 0;
}

