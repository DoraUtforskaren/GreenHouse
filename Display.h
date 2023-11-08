#ifndef  _DISPLAY_H
#define  _DISPLAY_H

void Init_Display();
void keypad_to_display(unsigned int button);
unsigned char Read_Status_Display();
void Write_Command_2_Display(unsigned char Command);
void Write_Data_2_Display(unsigned char Data);
void printDD(int num);



void date_display(time *timeStamp);
void print_character(char c);
void print_a_string(char c[]);
void write_display_pos(char *text, char x, char y);
void reset_display_pointer();
void write_menu (int pos);
void menu_tempLimit(int pos);
void clear_display();
void write_display_pos(char *text, char x, char y);
void display_config();
void set_display_pointer(char x, char y);
void write_setup_date_layout();
char *float_to_char(float num);
void display_records(int i);
char *uint_to_char(uint64_t num);
char *int_to_char(int num);
void display_tempLimit();
void tempLimit_instructions();
void dis_tempLimit_instructions();
void write_display_pos_threedigit(char *text, char x, char y, float check);
void print_a_string_threedigit(char c[], float limit);
void blinking_alarm(int alarm);
void write_menu_light();
extern float upp_limit;
extern float low_limit;
extern int alarmdely;
extern int light;
extern int darkness;
extern int fastmode_check;

#endif