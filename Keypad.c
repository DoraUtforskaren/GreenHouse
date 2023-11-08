
#include "my_lib.c"


int row[] = {2, 3, 4, 5};
int col[] = {7, 8, 9};

int func(void){
  int value = 0;
  *AT91C_PIOD_CODR = (1 << 2);
  *AT91C_PIOC_OER = 0x380;
  *AT91C_PIOC_SODR = 0x380;
  for(int i = 0; i < 3; i++){
    
    *AT91C_PIOC_CODR = (1 << col[i]);
    for(int j = 0; j < 4; j++){
      if((*AT91C_PIOC_PDSR & (1 << row[j])) != (1 << row[j]))
        value = j*3 + i+1;
        
    }
    *AT91C_PIOC_SODR = (1 << col[i]);
  }
  *AT91C_PIOC_ODR = 0x380;
  return value;
}

void keypad_config(){
  *AT91C_PMC_PCER = (1 << 13);
  *AT91C_PMC_PCER = (1 << 14);
  *AT91C_PIOD_PER = (1 << 2);//enable pin 27
  *AT91C_PIOD_OER = (1 << 2);//enable output
  *AT91C_PIOC_PER = 0x3BC;
  *AT91C_PIOC_PPUDR= 0x3BC;
  
  NVIC_ClearPendingIRQ(PIOD_IRQn);
  NVIC_SetPriority(PIOD_IRQn, 1);
  NVIC_EnableIRQ(PIOD_IRQn);
}