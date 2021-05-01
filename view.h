#ifndef __VIEW_H__
#define __VIEW_H__

#define FONT_HEIGHT 16
#define FONT_HEIGHT_HALF 8
#define RENDER_LINE_HEIGHT 20
#define RENDER_MAX_CHAR_PER_LINE 22
#define RENDER_MAX_LINES_PER_SCREEN 7

void draw_screen(struct app_data_ *app_data, char *msg);

#endif