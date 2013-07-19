#ifndef _STARTUP_VALUES_H_
#define _STARTUP_VALUES_H_

extern unsigned char arc_device[64];
extern unsigned char arc_group[16];

void load_startup_values();	
void save_startup_values();
void send_startup_values();
void clear_startup_values();

#endif
