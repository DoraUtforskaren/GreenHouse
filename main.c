#include "my_lib.c"

#define AT91C_PIOD_AIMER (AT91_CAST(AT91_REG *) 0x400E14B0)
#define AT91C_PIOD_DIFSR (AT91_CAST(AT91_REG *) 0x400E1484)
#define AT91C_PIOD_SCDR (AT91_CAST(AT91_REG *) 0x400E148C)
#define AT91C_PWMC_CMRx (AT91_CAST(AT91_REG *) 0x4009422C) 

node *first;
int dely = 0;
int e_dely = 0;
int alarmdely = 0;

int delyset = 0;
int delycount = 0;

int mesdly = 0;
int mesdly2 = 0;
int incr;
int rec_flag;
int fastmode_check = 0;
time *checktime;
int get_tempflag = 0;
int get_tempflag2 = 0;
int check_incr;

float track_temp;

int light;
int tot_light = 57600;
int darkness;
int tot_darkness = 28800;

float upp_limit = 30.0;
float low_limit = 10.0;
float temp_upp_limit;
float temp_low_limit;
int tempLimit_check;
int back_2_menu;

float temperature;
float current_temperature = 20;
//Getting keypad inputs

//End of keypad inputs

float max_sun = 0;
int incr = 0;
float sens;
int sunCali_flag = 0;
float prev_sens;
float variable = 0;

int calibrate_loop;

void SysTick_Handler(void){
  dely++;
  incr++;
  alarmdely++;
  
  //manipulating systick delay, between the temp measures.
  //in order to continue updating the time in the systick.
  if(get_tempflag == 1) {
    startMeas1();
    get_tempflag = 0;
    get_tempflag2 = 1;
    dely = 0;
  }
  if(get_tempflag2 == 1 && dely > 15){
    startMeas2();
    get_tempflag2 = 0;
  }
  
  if(dely >= 1500)
    dely = 0;
  if(alarmdely >= 1000)
    alarmdely = 0;
  
  //update_time() includes a flag that triggers the temp meas
  //everytime a minute increases
  
  if(fastmode_check == 1){
      if(incr >= 33){
        update_time(timeStamp);
        get_tempflag = 1;
        incr = 0;
      }
    }else if(fastmode_check == 0){
      if(incr >= 1000){
        update_time(timeStamp);
        get_tempflag = 1;
        incr = 0;
      }
    }
}


void Systick_fastmode(){
  if(fastmode_check == 0){
    fastmode_check = 1;
    light /= 60;
    darkness /= 60;
    tot_light = 960;
    tot_darkness = 480;
    write_display_pos("enabled", 66, 1);
    while(func() != 0);
  }else{
    fastmode_check = 0;
    light *= 60;
    darkness *= 60;
    tot_light = 57600;
    tot_darkness = 28800;
    write_display_pos("       ", 66, 1);
    while(func() != 0);
  }
}


int main(){
  SystemInit();
  SysTick_Config(84000-1);
  keypad_config();
  display_config();
  Init_Display();
  clear_display();
  reset_display_pointer();
  init_valuesforweekly();
  Init_TimerTemp();
  Init_ADC();
  Init_Servo();
  ledconfig();
  get_tempflag = 1;
  
  timeStamp = (time*)malloc(sizeof(time));
  checktime = (time*)malloc(sizeof(time));
  set_time(timeStamp,30,37,16,29,12,2022);
  
  reset_display_pointer();
  int setup_date_nicely[] = {0,1,3,4,6,7,8,9,30,31,33,34,36,37};
  
  while(1){
    reset_display_pointer();
    
    date_display(timeStamp);
    write_menu(-2);
    menu_tempLimit(0);
    if(fastmode_check == 1)
      write_display_pos("enabled", 66, 1);
    
    check_alarm(current_temperature);

    int date[14];

    int i;
    int p;
    int key = func();
    if(key > 0  && key < 7){
      switch(key){
      case 1:
        while(func() != 0);
        i = 0;
        int temp;
        clear_display();
        //set_display_pointer(150, 0);
        write_display_pos("Setup Date", 10, 0);
        write_display_pos("------------------------------", 30, 0);
        write_display_pos("New Date: DD/MM-YYYY", 60, 0);
        write_display_pos("New Time: HH:MM:SS", 90, 0);
        write_display_pos("Press * to Reset.", 74, 1);
        write_display_pos("Press # to Quit.", 104, 1);
        
        while(i != 15){
          if(func() > 0 && func() != 10 && func() != 12){
            
            if(func() == 11)
              temp = 0;
            else
              temp = func();
              while(func() != 0);
            date[i] = temp;
            set_display_pointer(70+setup_date_nicely[i], 0);
            keypad_to_display(temp);
            i++;
            
          }
          
          if (func() == 10) {
            i = 0;
            write_display_pos("New Date: DD/MM-YYYY", 60, 0);
            write_display_pos("New Time: HH:MM:SS", 90, 0);
          }
          
          if (func() == 12) {
            i = 15;
          }
          //date_display(timeStamp);  
          if (i == 14){
            array_to_date(checktime, date);
            if(valid_time(checktime) == 0){
              i = 0;
              clear_display();
              date_display(checktime);
              write_display_pos("****INVALID DATE****", 184, 0);
              dely = 0;
              while(dely < 900);
              clear_display();
              write_setup_date_layout();
            }
            else{
              array_to_date(timeStamp, date);
              i++;
            }
            
          }
        }
        clear_display();
        break;
        
        case 2:
          p = 0;
          clear_display();
          while(func() != 12){
            if(func() == 6){
              while(func() == 6){}
              p++;
              if(p > 6)
                p = 0;
              clear_display();
            }
            if(func() == 4){
              while(func() == 4){}
              p--;
              if(p < 0)
                p = 6;
              clear_display();
            }
          display_records(p);
          check_alarm(current_temperature);

          }
          clear_display();
          
        break; 
        case 3: 
          clear_display();
          calibrate_loop = 0;
          while(func() != 12){
            float temp_maxSun = max_sun;
            write_menu_light();
            if(func() == 1){
              sunCali_flag = 1;
              calibrate_loop = 1; 
              max_sun = 0;
              while(calibrate_loop == 1){
                write_display_pos("SET", 178, 1);
                write_display_pos("[0]", 208, 1);
                write_display_pos("RESET", 164, 1);
                write_display_pos("[*]", 195, 1);
                calibration_sun();
                write_display_pos("Max Sun Value: ", 44, 1);
                write_display_pos(float_to_char(max_sun), 60, 1); 
                
                
                if(func() >= 10){
                  if(func() == 10 || func() == 12)
                    max_sun = temp_maxSun;
                  calibrate_loop = 0;
                  write_display_pos("   ", 178, 1);
                  write_display_pos("   ", 208, 1);
                  write_display_pos("     ", 164, 1);
                  write_display_pos("   ", 195, 1);
                }
              }
              sunCali_flag = 0;
              variable = (1.500-(1.5 - max_sun)*0.5);
            }
          }

        case 4:
          clear_display();
          
          
          back_2_menu = 1;
          while(back_2_menu == 1){
            
            display_tempLimit();
            i = 0;
            check_alarm(current_temperature);
            if(func() == 12)
              back_2_menu = 0;
            if(func() == 1 || func() == 2){
              temp_upp_limit = upp_limit;
              temp_low_limit = low_limit;
              if(func() == 1){
                i = 1;
                write_display_pos("*", 124, 0);
              }
              else if(func() == 2){
                i = 2;
                write_display_pos("*", 184, 0);
              }
              while(func() != 0){}
              tempLimit_check = 1;
              while(tempLimit_check == 1){
                
                //write_display_pos(float_to_char(upp_limit), 139, 0);
                //write_display_pos("     ", 143, 0);
                tempLimit_instructions(-2);
                if(func() == 1){
                  if(i == 1  && upp_limit < 264){
                    upp_limit += 10;
                    write_display_pos("      ", 139, 0);
                    write_display_pos_threedigit(float_to_char(upp_limit), 139, 0, upp_limit);
                  }
                  if(i == 2  && low_limit < 264 && (low_limit+10) < upp_limit){
                    low_limit += 10;
                    write_display_pos("      ", 199, 0);
                    write_display_pos_threedigit(float_to_char(low_limit), 199, 0, low_limit);
                  }
                  while(func() == 1){}
                }
                if(func() == 4){
                  if(i == 1  && upp_limit > -264 && (upp_limit-10) > low_limit){
                    upp_limit -= 10;
                    write_display_pos("       ", 139, 0);
                    write_display_pos_threedigit(float_to_char(upp_limit), 139, 0, upp_limit);
                  }
                  if(i == 2  && low_limit > -264){
                    low_limit -= 10;
                    write_display_pos("       ", 199, 0);
                    write_display_pos_threedigit(float_to_char(low_limit), 199, 0, low_limit);
                  }
                  while(func() == 4){}
                }
                if(func() == 2){
                  if(i == 1  && upp_limit < 273){
                    upp_limit += 1;
                    write_display_pos("       ", 139, 0);
                    write_display_pos_threedigit(float_to_char(upp_limit), 139, 0, upp_limit);
                  }
                  if(i == 2  && low_limit < 273 && (low_limit+1) < upp_limit){
                    low_limit += 1;
                    write_display_pos("       ", 199, 0);
                    write_display_pos_threedigit(float_to_char(low_limit), 199, 0, low_limit);
                  }
                  while(func() == 2){}
                }
                if(func() == 5){
                  if(i == 1  && upp_limit > -273 && (upp_limit-1) > low_limit){
                    upp_limit -= 1;
                    write_display_pos("       ", 139, 0);
                    write_display_pos_threedigit(float_to_char(upp_limit), 139, 0, upp_limit);
                  }
                  if(i == 2  && low_limit > -273){
                    low_limit -= 1;
                    write_display_pos("       ", 199, 0);
                    write_display_pos_threedigit(float_to_char(low_limit), 199, 0, low_limit);
                  }
                  while(func() == 5){}
                }
                if(func() == 3){
                  if(i == 1  && upp_limit < 273.3 ){
                    upp_limit += 0.1;
                    write_display_pos("       ", 139, 0);
                    write_display_pos_threedigit(float_to_char(upp_limit), 139, 0, upp_limit);
                  }
                  if(i == 2  && low_limit < 273.3 && (low_limit+0.1) < upp_limit){
                    low_limit += 0.1;
                    write_display_pos("       ", 199, 0);
                    write_display_pos_threedigit(float_to_char(low_limit), 199, 0, low_limit);
                  }
                  while(func() == 3){}
                }
                if(func() == 6){
                  if(i == 1 && upp_limit > -273.3 && (upp_limit-0.1) > low_limit){
                    upp_limit -= 0.1;
                    write_display_pos("       ", 139, 0);
                    write_display_pos_threedigit(float_to_char(upp_limit), 139, 0, upp_limit);
                  }
                  if(i == 2 && low_limit > -273.3){
                    low_limit -= 0.1;
                    write_display_pos("       ", 199, 0);
                    write_display_pos_threedigit(float_to_char(low_limit), 199, 0, low_limit);
                  }
                  while(func() == 6){}
               }
               if(func() >= 10){
                 if(func() == 10 && i == 1)
                    upp_limit = temp_upp_limit;
                 if(func() == 10 && i == 2)
                   low_limit = temp_low_limit;
                 if(func() == 12){
                    back_2_menu = 0;
                    if(i == 1)
                      upp_limit = temp_upp_limit;
                    if(i == 2)
                      low_limit = temp_low_limit; 
                 }
                 tempLimit_check = 0;
               }
              } 
               write_display_pos(" ", 124, 0);
               write_display_pos(" ", 184, 0);
               if(func() == 10){
                 upp_limit = temp_upp_limit;
               }
               dis_tempLimit_instructions(-2);
            }      
          }
          clear_display();
        
        break;
        
        case 5:
          Systick_fastmode();

        break;
      }        
    }    
  }
}






