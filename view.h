#ifndef __VIEW_H__
#define __VIEW_H__

#include <libbip.h>
#include "bt_notes.h"
#include "notification.h"
#include "utils.h"

#define FONT_HEIGHT 16
#define FONT_HEIGHT_HALF 8
#define RENDER_LINE_HEIGHT 20
#define RENDER_MAX_CHAR_PER_LINE 22
#define RENDER_MAX_LINES_PER_SCREEN 7

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

void render_home_view(struct app_data_ *app_data) {
  int i, id;
  saved_data_t *saved_data = get_saved_data_ptr();
  note *n;
  char buf[40];

  for (i=0; i<3; i++) {
    id = app_data->current_note == SCREEN_HOME_1 ? i : (i+3);
    n = &saved_data->notes[id];
    _sprintf(buf, "%d. %s", id + 1, n->title[0] ? n->title : "(empty)");
    text_out(buf, 5, 58 * i + 29 - FONT_HEIGHT_HALF);
  }
}


void render_help() {
  char buf[256];
  m_strcpy(buf, "BT Notes V1.1\n(c) ngxson.com\nHOW TO USE: Send a notification from your phone.\nThe title must start with @bip1, @bip2,...", 256);
  text_out_with_word_wraps(buf, 5, 0);
  // print debug
  int ptr = (int) get_saved_data_ptr();
  _sprintf(buf, "0x%08x", ptr);
  text_out(buf, 5, 176 - 18);
}

void render_scroll_bar(int page, int total) {
  int h = 176 / total;
  draw_filled_rect(
    176 - 2, page * h,
    176    , (page+1) * h
  );
}

void render_note(struct app_data_ *app_data) {
  int id = app_data->current_note;
  saved_data_t *saved_data = get_saved_data_ptr();
  note *n = &saved_data->notes[id];
  char msg[NOTE_MSG_LEN];
  m_strcpy(msg, n->msg, NOTE_MSG_LEN);

  text_out_center("Loading...", 88, 88 - FONT_HEIGHT_HALF);
  repaint_screen_lines(0, 176);
  fill_screen_bg();

  // render content
  text_out(n->title, 5, 5);
  draw_horizontal_line(5 + 20, 0, 176);
  text_out_with_word_wraps(msg, 5 + 20 + 5, app_data->current_page_num);
  render_scroll_bar(app_data->current_page_num, 3);
}

#define FONT_DIGIT_TINY_14  14
void render_clock() {
  char buf[8];
  struct datetime_ dt;
  get_current_date_time(&dt);
  _sprintf(buf, "%02d:%02d", dt.hour, dt.min);
  text_out_font(FONT_DIGIT_TINY_14, buf, 176 - (8*4 + 3) - 5, 8, 1);
}

void render(struct app_data_ *app_data) {
  if (app_data->current_note == SCREEN_HELP) {
    render_help();
    render_scroll_bar(2, 3);
  } else if (app_data->current_note <= SCREEN_HOME_1) {
    render_home_view(app_data);
    render_scroll_bar(app_data->current_note == SCREEN_HOME_1 ? 0 : 1, 3);
  } else if (app_data->current_note >= 0) {
    render_note(app_data);
  }
  render_clock();
}

#endif