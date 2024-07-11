#ifndef MAIN
#define MAIN

#define DASHBOARD               0
#define PASSWORD                1
#define MENU                    2
#define MENU_ENTER              3

#define VIEWLOG                 0
#define DOWNLOADLOG             2
#define CLEARLOG                1
#define SETTIME                 3
#define CHANGEPASS              4

#define WAIT_TIME 60000 // count value for 3 seconds
#define BLOCK_TIME 10

// gear
char *gear[9] = {"ON", "GN", "GR", "G1", "G2", "G3", "G4", "G5", "C "};
unsigned char index;

// speed
unsigned char speed;


// string
char str[17];

/* Timer variables */
unsigned char flag;
unsigned long int sec_delay;
unsigned long int wait;

/* logs variables */
char s,p,full_f;
char e; 

/* clock time variables */
unsigned char time[9];
unsigned char clock_reg[3];
unsigned char calender_reg[4];
unsigned char hr, min, sec;

/* menu variables */
unsigned char back_f;
char main_f, menu_f;

/* Password variables */
#define LEN 4 
unsigned char pw[LEN];

/* Function declarations */
void dashboard(char key);               
void store_event(char *time,unsigned char index,unsigned char speed);
void password(char key);
void menu(char key);
void view_log(char key);
void download_log();
void clear_log(char key);
void settime(char key);
void change_pass(char key);


void get_time(void);


#endif