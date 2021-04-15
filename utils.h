#ifndef __UTILS_H__
#define __UTILS_H__

#include "bt_notes.h"

void m_strcpy(char *d, const char *s, int max_length);
int m_strcmp(const char *s1, const char *s2);
saved_data_t *get_saved_data_ptr();

#endif