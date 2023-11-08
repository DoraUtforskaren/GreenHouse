#ifndef Records_H
#define Records_H

typedef struct stats stats;
struct stats{
  recorded_time date;
  recorded_time maxTime;
  recorded_time minTime;
  float min;
  float max;
  float avg;
  struct stats *next;
};

void temperature_recording(float data, recorded_time date);
void compare_values(node *value);
void day_stats(node check);
void min(node min_check);
void max(node max_check);
void avg(node avg_check);
void print_weeklystats(int i);
void addStats_WhenFull();

extern stats weeklystats[7];

void init_valuesforweekly();

extern stats *head;
extern node *first;

extern float current_temperature;
//extern time* timeStamp;

extern int llsize;



#endif