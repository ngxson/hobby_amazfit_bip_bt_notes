#include "utils.h"

// https://stackoverflow.com/questions/14723381/how-strcpy-works-behind-the-scenes
void m_strcpy(char *d, const char *s, int max_length) {
  int i = 0;
  while (*s) {
    i++; if (i >= max_length) break;
    *d++ = *s++;
  }
  *d = 0;
}
