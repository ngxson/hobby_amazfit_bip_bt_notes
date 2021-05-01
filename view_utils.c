#include <libbip.h>
#include "bt_notes.h"
#include "notification.h"
#include "view.h"
#include "view_utils.h"

void draw_button(char* name, int from_x, int from_y, int to_x, int to_y, long text_color, long bg_color) {
  set_bg_color(bg_color);
  set_fg_color(text_color);
  draw_rect(from_x, from_y, to_x, to_y);
  draw_filled_rect_bg(from_x+1, from_y+1, to_x-1, to_y-1);
  text_out_center(name, (from_x + to_x) / 2, (from_y + to_y) / 2 - 8);
}

void _text_out_ln(char *txt, int y, int *line, int page_num) {
  int start_line = page_num * RENDER_MAX_LINES_PER_SCREEN;
  int end_line = start_line + RENDER_MAX_LINES_PER_SCREEN;
  int pos = (*line) - start_line;
  if (pos >= 0 && (*line) < end_line) {
    text_out(txt, 5, y + pos * RENDER_LINE_HEIGHT);
  }
  (*line)++;
}

void text_out_with_word_wraps(char *txt, int y, int page_num) {
  char *c = txt;
  char *start = txt;
  char *last_space = NULL;
  char *end = NULL;
  char tmp;
  int line = 0;
  
  while (*c) {
    if (*c == '\n') {
      *c = '\0';
      _text_out_ln(start, y, &line, page_num);
      start = c + 1;
      last_space = NULL;
    }
    
    else if (c - start > RENDER_MAX_CHAR_PER_LINE) {
      if (last_space != NULL) *last_space = '\0';
      end = start + RENDER_MAX_CHAR_PER_LINE + 1;
      tmp = *end;
      *end = '\0';
      _text_out_ln(start, y, &line, page_num);
      *end = tmp;
      start = (last_space != NULL) ? (last_space + 1) : (c + 1);
      last_space = NULL;
    }

    else if (*c == ' ') last_space = c;
    
    c++;
    
    if (*c == 0) {
      _text_out_ln(start, y, &line, page_num);
    }
  }
}

void render_switch(int y, int on) {
  show_res_by_id(on ? ICON_TOGGLE_ON : ICON_TOGGLE_OFF, 176-53-5, y);
}

void render_menu_arrow(int x, int y) {
  show_res_by_id(MENU_ITEM_ARROW, x == 0 ? (176-12) : x, y);
}

#define FONT_DIGIT_TINY_14  14
void render_clock() {
  char buf[8];
  struct datetime_ dt;
  get_current_date_time(&dt);
  _sprintf(buf, "%02d:%02d", dt.hour, dt.min);
  text_out_font(FONT_DIGIT_TINY_14, buf, 176 - (8*4 + 3) - 5, 8, 1);
}
