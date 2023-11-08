#ifndef Temperature_H
#define Temperature_H


void Init_TimerTemp();
void startMeas1();
void startMeas2();

float get_Temperature();

extern float temperature;

extern int dely;
extern int e_dely;
extern int mesdly;
extern int get_tempflag;

extern float upp_limit;
extern float low_limit;

int temp_alarm(float temperature);
void check_alarm(float check_temp);


extern int delyset;
//extern void temperature_recording(time*calender , float data);


#endif