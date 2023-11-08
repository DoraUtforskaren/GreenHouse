#include "my_lib.c"


float volt = 0;


float sunrise;
float sundown;
int sunflag = 0;
int prev_sunflag = 0;
int prev_sunflag_2 = 0;
int night_flag = 0;




float measADC(void){
  *AT91C_ADCC_CHER = (1 << 1);//channel select
  *AT91C_ADCC_CR = (1 << 1);
  
  float temp;
  while(1){
    if((*AT91C_ADCC_SR & (1 << 24)) == (1 << 24)){
      temp = (*AT91C_ADCC_LCDR & 0xFFF)*(3.3/4096); //read the value in ADC_LCDR
      volt = (*AT91C_ADCC_LCDR & 0xFFF)*(3.3/4096);
      return temp;
    }
  }
}

void measSun(){
  float sens = measADC();
  degree_sun(sens);
  //prev_sens = sens;
  
  if((sens < 1.500) && (light <= tot_light)){
    light++;
    Set_Led(0);
  }
  
  else if((sens > 1.500 && light <= tot_light && light > 100) || darkness >= tot_darkness){
    Set_Led(1);
    light++;
    //led_light++;
  }
  
   else{
    if(light >= tot_light)
      Set_Led(0);
    darkness++;
   }
}


void Init_ADC(void){
  *AT91C_PMC_PCER1 = (1 << 5); //Enable peripheral for ADC
  *AT91C_ADCC_MR = (2 << 8); //set ADCC_MR to 2
} 

void calibration_sun(){
  if(max_sun == 0)
    max_sun = measADC();
  if(measADC() < max_sun)
    max_sun = measADC();
  
  
}

void degree_sun(float sensor){
  
  if(sensor > (1.5 +max_sun) - max_sun){
    if(sunflag == 1){
      sunflag = 0;
      turn(18);
    }
    else
      turn(0);
  }
  if(sensor > variable && sunflag == 0 && sensor < 1.500 ){
    turn(2);
  }
  if(sensor >  variable && sunflag == 1 && sensor < 1.500){
    turn(15);
  }
  if(sensor < variable  && sensor >= (max_sun + 0.1) && sunflag == 0){
    turn(5);
  }
 
  if(sensor < variable && sensor >= (max_sun + 0.1) && sunflag == 1){
    turn(12);
  }
  
  if(sensor <= (max_sun+0.1)){ // || prev_sens > sensor){
    sunflag = 1;
    turn(9);
  }
}


