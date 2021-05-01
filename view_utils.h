#ifndef __VIEW_UTILS_H__
#define __VIEW_UTILS_H__

void _text_out_ln(char *txt, int y, int *line, int page_num);
void text_out_with_word_wraps(char *txt, int y, int page_num);
void render_switch(int y, int on);
void render_menu_arrow(int x, int y);
void render_clock();

#endif