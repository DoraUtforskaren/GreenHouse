#include "my_lib.c"

void Init_Display(void){
  *AT91C_PIOD_CODR = (1<< 0); //Clear reset
  Delay(20);
  *AT91C_PIOD_SODR = (1<< 0); //Set reset
  
  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x40);//Set text home address
  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x40);
  Write_Command_2_Display(0x42); //Set graphic home address
  Write_Data_2_Display(0x1E);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x41); // Set text area
  Write_Data_2_Display(0x1E);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x43); // Set graphic area
  Write_Command_2_Display(0x80); // text mode
  Write_Command_2_Display(0x94); // Text on graphic off 
}

void keypad_to_display(unsigned int button){
  if((button >= 0 ) && (button < 10)){
    Write_Data_2_Display(0x10 + button);
  }
  else if(button == 10){
    Write_Data_2_Display(0x0A);
  }else if(button == 11){
    Write_Data_2_Display(0x10); 
  }else if(button == 12){
    Write_Data_2_Display(0x03); 
  }
  Write_Command_2_Display(0xC0);
  button = 11;
  Delay(200000);
  
}

//Setting up display functions
unsigned char Read_Status_Display(void){
  unsigned char Temp;
  *AT91C_PIOC_ODR = 0x3FC;//make databus as input
  *AT91C_PIOC_SODR = (1 << 13);//Set dir as input (74chip, 1 = input) 
   *AT91C_PIOC_CODR = (1 << 12);//Clear/enable output, 
  *AT91C_PIOC_SODR = (1 << 14);//Set C/D 
  *AT91C_PIOC_CODR = (1 << 15);// clear chip
  *AT91C_PIOC_CODR = (1 << 16);//clear read display
  Delay(20);// make a delay
  Temp = (*AT91C_PIOC_PDSR & 0x3FC)>>2;//read databus and save in temp
  *AT91C_PIOC_SODR = (1 << 15);//set chip select display
  *AT91C_PIOC_SODR = (1 << 16);//set read display
  *AT91C_PIOC_SODR = (1 << 12); //disble output on chip
  *AT91C_PIOC_CODR = (1 << 13);//set dir as output
  return Temp;
}

void Write_Command_2_Display(unsigned char Command){
  while((Read_Status_Display() & 0x3) != 0x3); //Wait until read finished
  *AT91C_PIOC_CODR = 0x3FC; //Clear databus
  *AT91C_PIOC_SODR = ((unsigned int)Command) << 2 ; //Set command to databus
  *AT91C_PIOC_CODR = (1 << 13);//Set dir as output
  *AT91C_PIOC_CODR = (1 << 12);//enable output
  *AT91C_PIOC_OER = 0x3FC; // set databus as output
  *AT91C_PIOC_SODR = (1 << 14); //set c/d signal high (1= command)
  *AT91C_PIOC_CODR = (1 << 15);//Clear chip select display
  *AT91C_PIOC_CODR = (1 << 17); //set write display//clear write display
  Delay(20);
  *AT91C_PIOC_SODR = (1 << 15); //set chip select display
  *AT91C_PIOC_SODR = (1 << 17); //set write display
  *AT91C_PIOC_SODR = (1 << 12);//disable output
  *AT91C_PIOC_ODR = 0x3FC;//make databus as input
}

void Write_Data_2_Display(unsigned char Data){
  while((Read_Status_Display() & 0x3) != 0x3); //Wait until read finished
  *AT91C_PIOC_CODR = 0x3FC; //Clear databus
  *AT91C_PIOC_SODR = ((unsigned int)Data) << 2 ; //Set data to databus
  *AT91C_PIOC_CODR = (1 << 13); //Set dir as output
  *AT91C_PIOC_CODR = (1 << 12);//enable output
  *AT91C_PIOC_OER = 0x3FC;//set databusa as output
  *AT91C_PIOC_CODR = (1 << 14); //set c/d signal high (1= command)
  *AT91C_PIOC_CODR = (1 << 15); //clear chip select display
  *AT91C_PIOC_CODR = (1 << 17); //set write display
  Delay(20);
  *AT91C_PIOC_SODR = (1 << 15); //clear chip select display
  *AT91C_PIOC_SODR = (1 << 17); //set write display
  *AT91C_PIOC_SODR = (1 << 12);//disable output
  *AT91C_PIOC_ODR = 0x3FC;//make databus as input
}

void printDD(int num){
  if(num > 99){
    int y1digit = num / 1000;
    int y2digit = (num % 1000) / 100;
    int y3digit = (num % 100) / 10;
    int y4digit = num % 10;
    Write_Data_2_Display(0x10 + y1digit);
    Write_Command_2_Display(0xC0);
    Write_Data_2_Display(0x10 + y2digit);
    Write_Command_2_Display(0xC0);
    Write_Data_2_Display(0x10 + y3digit);
    Write_Command_2_Display(0xC0);
    Write_Data_2_Display(0x10 + y4digit);
    Write_Command_2_Display(0xC0);
  }else{
    int fdigit = num / 10;
    int ldigit = num % 10;
    Write_Data_2_Display(0x10 + fdigit);
    Write_Command_2_Display(0xC0);
    Write_Data_2_Display(0x10 + ldigit);
    Write_Command_2_Display(0xC0);
  }
}

void date_display(time *timeStamp){
  print_a_string("DATE: ");
  printDD(timeStamp->day);
  Write_Data_2_Display(0x0F);
  Write_Command_2_Display(0xC0);
  printDD(timeStamp->month);
  Write_Data_2_Display(0x0D);
  Write_Command_2_Display(0xC0);
  printDD(timeStamp->year);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0xC0);
  
  set_display_pointer(30, 0);
  
  
  print_a_string("TIME: ");
  printDD(timeStamp->hour);
  Write_Data_2_Display(0x1A);
  Write_Command_2_Display(0xC0);
  printDD(timeStamp->minute);
  Write_Data_2_Display(0x1A);
  Write_Command_2_Display(0xC0);
  printDD(timeStamp->second);
  
  

}

void reset_display_pointer(void){
  Write_Data_2_Display(0x0);
  Write_Data_2_Display(0x0);
  Write_Command_2_Display(0x24);
  
}

void set_display_pointer(char x, char y){
  Write_Data_2_Display(x);
  Write_Data_2_Display(y);
  Write_Command_2_Display(0x24);
  
}

void clear_display(void){
  reset_display_pointer();
  for(int i = 0; i < 640; i++){//640 total icons 
    Write_Data_2_Display(0x00);
    Write_Command_2_Display(0xC0); 
  }
  reset_display_pointer();
}

void print_character(char c){
  Write_Data_2_Display(c - 0x20);
  Write_Command_2_Display(0xC0);
}

void print_a_string(char c[]){
  for(int i = 0; c[i] != '\0'; i++){
    print_character(c[i]);
  }
}

void print_a_string_threedigit(char c[], float limit){
  int x;
  if(limit < 1000 && limit > -1000){
    x = 5;
    if(limit < 0)
      x += 1;
    if(limit < 100 && limit > -100){
      x = 4;
      if(limit < 0)
        x += 1;
      if(limit < 10 && limit > -10){
        x = 3;
        if(limit < 0)
          x += 1;
      }
    }
    
  }
  for(int i = 0; i < x; i++){
    print_character(c[i]);
  }
}

void write_display_pos_threedigit(char *text, char x, char y, float check){
  Write_Data_2_Display(x);
  Write_Data_2_Display(y);
  Write_Command_2_Display(0x24);
  
  print_a_string_threedigit(text, check);
  
}

void write_display_pos(char *text, char x, char y){
  Write_Data_2_Display(x);
  Write_Data_2_Display(y);
  Write_Command_2_Display(0x24);
  
  print_a_string(text);
  
}



void write_menu (int pos) {
  write_display_pos("MENU", 122 + pos, 0);
  write_display_pos("----------------------", 152 + pos, 0);
  write_display_pos("| 1. Setup Date      |", 182 + pos, 0);
  write_display_pos("| 2. Record of Temps |", 212 + pos, 0);
  write_display_pos("| 3. Light Setup     |", 242 +pos, 0);
  write_display_pos("| 4. Temp Limits     |", 16 + pos, 1);
  write_display_pos("| 5. Enable Fastmode |", 46 + pos, 1);
  write_display_pos("----------------------", 76 + pos, 1);
                   
}

void menu_tempLimit(int pos){
  write_display_pos("Upper Limit:", 134 + pos, 1);
  write_display_pos_threedigit(float_to_char(upp_limit), 146 + pos, 1, upp_limit);
   write_display_pos("Lower Limit: ", 164 + pos, 1);
  write_display_pos_threedigit(float_to_char(low_limit), 176 + pos, 1, low_limit);
}
void write_setup_date_layout(){
   write_display_pos("Setup Date", 10, 0);
   write_display_pos("------------------------------", 30, 0);
   write_display_pos("New Date: DD/MM-YYYY", 60, 0);
   write_display_pos("New Time: HH:MM:SS", 90, 0);
   write_display_pos("Press * to Reset.", 74, 1);
   write_display_pos("Press # to Quit.", 104, 1);
}

char char_arrayf[1000];

char *float_to_char(float num){
    sprintf(char_arrayf, "%f", num);
    return char_arrayf;
}

char char_arrayi[1000];

char *int_to_char(int num){
    sprintf(char_arrayi, "%d", num);
    return char_arrayi;
}

char char_arrayu[1000];

char *uint_to_char(uint64_t num){
    sprintf(char_arrayu, "%llu", num);
    return char_arrayu;
}

void display_records(int i){
  write_display_pos("DAILY RECORD OF TEMPS", 0,0);
  //update_Rec_time(first);

  
    write_display_pos("------------------------------", 30, 0);
    set_display_pointer(60, 0);
    print_a_string("DATE: ");
    printDD(weeklystats[i].date.day);
    Write_Data_2_Display(0x0F);
    Write_Command_2_Display(0xC0);
    printDD(weeklystats[i].date.month);
    Write_Data_2_Display(0x0D);
    Write_Command_2_Display(0xC0);
    printDD(weeklystats[i].date.year);
    Write_Data_2_Display(0x00);
    Write_Command_2_Display(0xC0);
    
    //Minimum
    write_display_pos("Minimum: ", 90, 0);
    write_display_pos(" - Temp: ", 120, 0);
    write_display_pos(float_to_char(weeklystats[i].min), 129, 0);
    write_display_pos(" - Time: ", 150, 0);
    set_display_pointer(159, 0);
    printDD(weeklystats[i].minTime.hour);
    //write_display_pos(uint_to_char(weeklystats[i].minTime.hour), 159, 0);
    write_display_pos(":", 161, 0);
    set_display_pointer(162, 0);
    printDD(weeklystats[i].minTime.minute);
    //write_display_pos(uint_to_char(weeklystats[i].minTime.minute), 162, 0);
    
    //Maximum
    write_display_pos("Maximum: ", 210, 0);
    write_display_pos(" - Temp: ", 240, 0);
    write_display_pos(float_to_char(weeklystats[i].max), 249, 0);
    write_display_pos(" - Time: ", 14, 1);
    set_display_pointer(23, 1);
    printDD(weeklystats[i].maxTime.hour);
    //write_display_pos(uint_to_char(weeklystats[i].maxTime.hour), 23, 1);
    write_display_pos(":", 25, 1);
    set_display_pointer(26, 1);
    printDD(weeklystats[i].maxTime.minute);
    //write_display_pos(uint_to_char(weeklystats[i].maxTime.minute), 26, 1);
    
    //Average
    write_display_pos("Average Temp: ", 74, 1);
    
    write_display_pos(float_to_char(weeklystats[i].avg), 88, 1);
    
    //first = first->next;
    //194 start
    write_display_pos("PREV", 170, 1);
    write_display_pos("[4]" , 201, 1);
    write_display_pos("[" ,206 , 1);
    write_display_pos(int_to_char(i+1), 207, 1);
    write_display_pos("/7]" ,208, 1);
    write_display_pos("NEXT", 183, 1);
    write_display_pos("[6]", 213, 1);
    write_display_pos("QUIT", 190, 1);
    write_display_pos("[#]", 220,1);
  
}

void display_tempLimit(){
  write_display_pos("Setup Temperature Limits", 0, 0);
  write_display_pos("------------------------------", 30, 0);
  write_display_pos("PRESS", 60, 0);
  write_display_pos("[1]", 120, 0);
  write_display_pos("Upper Limit: ", 126, 0);
  write_display_pos_threedigit(float_to_char(upp_limit), 139, 0, upp_limit);
  write_display_pos("[2]", 180, 0);
  write_display_pos("Lower Limit: ", 186, 0);
  write_display_pos_threedigit(float_to_char(low_limit), 199, 0, low_limit);
  write_display_pos("QUIT", 190, 1);
  write_display_pos("[#]", 220,1);
}

void tempLimit_instructions(int pos){
  write_display_pos("[1]+10", 20 + pos, 1);
  write_display_pos("[2]+1", 27 + pos, 1);
  write_display_pos("[3]+0.1", 33 + pos, 1);
  
  write_display_pos("[4]-10", 50 + pos, 1);
  write_display_pos("[5]-1", 57 + pos, 1);
  write_display_pos("[6]-0.1", 63 + pos, 1);
  
  write_display_pos("[*]RESET", 110 + pos, 1);
  write_display_pos("[0] SET", 123 + pos, 1);
  
}

void dis_tempLimit_instructions(int pos){
  write_display_pos("   ", 178 + pos, 1);
  write_display_pos("   ", 208 + pos, 1);
  write_display_pos("      ", 20+ pos, 1);
  write_display_pos("     ", 27 + pos, 1);
  write_display_pos("       ", 33 + pos, 1);
  
  write_display_pos("      ", 50 + pos, 1);
  write_display_pos("     ", 57 + pos, 1);
  write_display_pos("       ", 63 + pos, 1);
}

void blinking_alarm(int alarm){
  clear_display();
  while(func() != 12){
    write_display_pos("Press # to exit.", 51, 1);
    if(alarm == 1)
      write_display_pos("Temperature to High!!!", 184, 0);  
    if(alarm == 2)
      write_display_pos("Temperature to Low!!!", 184, 0);
    alarmdely = 0;
    while(alarmdely < 600){
      if(func() == 12)
        break;
    }
    write_display_pos("                      ", 184, 0);
    alarmdely = 0;
    while(alarmdely < 300){
      if(func() == 12)
        break;
    }
  }
  while(func() == 12);
  

  clear_display();
}

void write_menu_light(){
  write_display_pos("Display of all light and dark", 0 ,0);
  write_display_pos("------------------------------", 30, 0);
  write_display_pos("Hours of sunlight: ", 90, 0);
  write_display_pos("Hours of darkness: ", 150, 0);
  write_display_pos("Hours of led:" , 210, 0);
  if(fastmode_check == 1){
    write_display_pos(int_to_char(light/60), 110, 0);
    write_display_pos(int_to_char(darkness/60), 170, 0);
  }
  if(fastmode_check == 0){
    write_display_pos(int_to_char(light/3600), 110, 0);
    write_display_pos(int_to_char(darkness/3600), 170, 0);
  }
  write_display_pos("Press [1] to calibrate Sun", 74, 1); 
  write_display_pos("Max Sun Value: ", 44, 1);
  write_display_pos(float_to_char(max_sun), 60, 1);
  write_display_pos("QUIT", 190, 1);
  write_display_pos("[#]", 220,1);
}

void display_config(){
  *AT91C_PIOD_PER = (1 << 0);//enable pin 25
  *AT91C_PIOD_OER = (1 << 0);//enable output
  *AT91C_PIOC_PER = 0xFF040;//enable pins c6, c12-c19
  *AT91C_PIOC_OER = 0xFF000;//output control pins
  *AT91C_PIOC_PPUDR= 0xFF040;
}

//End of setting up display