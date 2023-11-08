#ifndef Timestamp_H
#define Timestamp_H


typedef struct{
  int second;
  int minute;
  int hour;
  
  int day;
  int month;
  int year;
}time;


typedef struct recorded_time recorded_time;

struct recorded_time{
  int minute;
  int hour;
  
  int day;
  int month;
  int year;
};

extern time *timeStamp;
extern int fastmode_check;
extern int get_tempflag;

/*
int r_min;
int r_hour;
int r_day;
int r_month;
int r_year;*/


extern int light;

extern int sunCali_flag;
extern float temperature;

void array_to_date(time *date, int a[]);
void set_time(time *calender, int sec, int min, int h, int d, int mon, int y);
void set_date(time *calender, int d, int mon, int y);
void update_time(time *calender);
void print_attributes(time *calender);
char valid_time(time* calender);
void update_Rec_time ();

recorded_time get_time();
//recorded_time time_to_rectime(time *calender);
//recorded_time get_time();


#endif