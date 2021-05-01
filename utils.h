#ifndef __UTILS_H__
#define __UTILS_H__

#include "bt_notes.h"

#define SWITCH_BRIGHT_THEME 0
#define SWITCH_BIP_PREFIX   1

void m_strcpy(char *d, const char *s, int max_length);
int m_strcmp(const char *s1, const char *s2);
void get_nand_data(struct app_data_ *app_data);
void flip_switch(struct app_data_ *app_data, int sw);

#endif