#ifndef  _Photosensor_H
#define  _Photosensor_H

float measADC(void);
void Init_ADC(void);
void measSun();
void calibration_sun();
void degree_sun(float sensor);

extern int light;
extern int tot_light;
extern int darkness;
extern int tot_darkness;
extern float sens;
extern float max_sun;
extern float prev_sens;
extern float variable;
#endif