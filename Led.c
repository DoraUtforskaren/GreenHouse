#include "my_lib.c"

int reading = 0;

void Set_Led(int nLed){
  if(nLed == 0)
    *AT91C_PIOD_CODR = (1 << 3);
  else if(nLed == 1)
    *AT91C_PIOD_SODR = (1 << 3);
}

void readButton(unsigned int *nButton){
  if((*AT91C_PIOD_PDSR &(1 << 1)) == (1 << 1)){   
    *nButton = 0;
  }else{
    *nButton = 1;
  }
}

void ledconfig(){
  *AT91C_PMC_PCER = (1 << 14);
  *AT91C_PIOD_PER = (1 << 3);
  *AT91C_PIOD_OER = (1 << 3);
  *AT91C_PIOD_PPUDR = (1 << 3);
  *AT91C_PIOD_ODR = (1 << 1);
  *AT91C_PIO_SCDR = (10);
  *AT91C_PIOD_ISR;
  *AT91C_PIOD_AIMER = (1 << 1);
  *AT91C_PIO_DIFSR = (1 << 1);
  *AT91C_PIO_IFER = (1 << 1);
  *AT91C_PIOD_IER = (1 << 1);
  
  NVIC_ClearPendingIRQ(PIOD_IRQn);
  NVIC_SetPriority(PIOD_IRQn, 0);
  NVIC_EnableIRQ(PIOD_IRQn);
}