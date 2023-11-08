#include "my_lib.c"

time *timeStamp;

void clock(){

}


int days_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int m;

void array_to_date(time *date, int a[]){
  int day = 0;
  int month = 0;
  int year = 0;
  int sec = 0;
  int min = 0;
  int hour = 0;
  for(int i = 1; i < 7; i++){
    switch (i){
    case 1:
      day = (a[0] * 10) + a[1];
    case 2:
      month = a[2]* 10 + a[3];
    case 3:
      year = a[4]*1000 + a[5] *100 + a[6]*10 + a[7];
    case 4: 
      hour = a[8] * 10 + a[9];
    case 5:
      min = a[10] * 10 + a[11];
    case 6:
      sec = a[12] * 10 + a[13];
    }
  }
  set_time(date, sec, min, hour, day, month, year);
}

void set_time(time *calender, int sec, int min, int h, int d, int mon, int y){
    calender->second = sec;
    calender->minute = min;
    calender->hour = h;

    calender->day = d;
    calender->month = mon;
    calender->year = y;
    m = (mon-1);
}

void set_date(time *calender, int d, int mon, int y){
    calender->day = d;
    calender->month = mon;
    calender->year = y;
    m = (mon-1);
}

void update_time(time *calender){
  if(fastmode_check == 1){
      temperature_recording(get_Temperature(), get_time());
      calender->minute++;
      
      if(sunCali_flag == 0)
        measSun();
      if (calender->minute > 59){
        calender->hour++;
        calender->minute = 0;
        if (calender->hour > 23){

          calender->day++;
          calender->hour = 0;
          light = 0;
          darkness = 0;
          if (calender->day > days_month[m]){
            calender->month++;
            calender->day = 1;
            m++;
            if (m > 11){m = 0;}
            if (calender->month > 12){
              calender->year++;
              calender->month = 1;
            }
                    
          }
                
        } 
      }
    
  }else{
    calender->second++;
    if(sunCali_flag == 0)
      measSun();
    if(calender->second > 59){
      calender->minute++;
      calender-> second = 0;
      
      temperature_recording(get_Temperature(), get_time());
      if (calender->minute > 59){
        calender->hour++;
        calender->minute = 0;
        if (calender->hour > 23){

          calender->day++;
          calender->hour = 0;
          light = 0;
          darkness = 0;
          if (calender->day > days_month[m]){
            calender->month++;
            calender->day = 1;
            m++;
            if (m > 11){m = 0;}
            if (calender->month > 12){
              calender->year++;
              calender->month = 1;
            }
                    
          }
                
        } 
      }
    }     
  }
}




char valid_time(time* calender){
  if(calender->month > 12 || calender->month < 1){
    return 0;
  }
  if(calender->month < 13){
    m = (calender->month-1);
  }
  if(calender->day > days_month[m] || calender->day < 1){
    return 0;
  }
  if(calender->second > 59){
    return 0;
  }
   if(calender->minute > 59){
    return 0;
  }
  if(calender->hour > 23){
    return 0;
  }
  else{
    return 1;
  }
   
}


struct recorded_time get_time(){
  struct recorded_time temp;// = (recorded_time*)malloc(sizeof(recorded_time));
  temp.minute = timeStamp->minute;
  temp.hour = timeStamp->hour;
  temp.day = timeStamp->day;
  temp.month = timeStamp->month;
  temp.year = timeStamp->year;
  return temp;
}

