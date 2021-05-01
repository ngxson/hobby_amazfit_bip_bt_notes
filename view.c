#include <libbip.h>
#include "bt_notes.h"
#include "notification.h"
#include "view.h"
#include "view_utils.h"
#include "utils.h"

void render(struct app_data_ *app_data);

void draw_screen(struct app_data_ *app_data, char *msg) {
  char bright_theme = app_data->nand_saved_data.bright_theme && app_data->current_scr >= 0;
  set_bg_color(bright_theme ? COLOR_WHITE : COLOR_BLACK);
  fill_screen_bg();
  set_graph_callback_to_ram_1();
  load_font();
  set_fg_color(bright_theme ? COLOR_BLACK : COLOR_WHITE);

  if (msg != NULL) {
    text_out_center(msg, 88, 68);
  } else {
    render(app_data);
  }

  if (!bright_theme) render_clock();

  repaint_screen_lines(0, 176);
  set_update_period(0, 0);
};

void render_home_view(struct app_data_ *app_data) {
  int i, id;
  notes_data_t *saved_data = app_data->nand_saved_data.notes_data;
  note *n;
  char buf[40];

  for (i=0; i<3; i++) {
    id = app_data->current_scr == SCREEN_HOME_1 ? i : (i+3);
    n = &saved_data->notes[id];
    _sprintf(buf, "%d. %s", id + 1, n->title[0] ? n->title : "(empty)");
    text_out(buf, 5, 58 * i + 29 - FONT_HEIGHT_HALF);
  }
}

void render_help() {
  char buf[256];
  m_strcpy(buf, "HOW TO USE: Send a notification from your phone.\nThe title must start with @bip1, @bip2,...", 256);
  text_out_with_word_wraps(buf, 25, 0);
}

void render_scroll_bar(int page, int total) {
  int h = 176 / total;
  draw_filled_rect(
    176 - 2, page * h,
    176    , (page+1) * h
  );
}

void render_note(struct app_data_ *app_data) {
  int id = app_data->current_scr;
  notes_data_t *saved_data = app_data->nand_saved_data.notes_data;
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

void render_settings(struct app_data_ *app_data) {
  text_out("White", 5, 29 - FONT_HEIGHT);
  text_out("background", 5, 29 + 2);
  render_switch(16, app_data->nand_saved_data.bright_theme);
  text_out("Scan @bip", 5, 58 + 29 - FONT_HEIGHT);
  text_out("prefix [?]", 5, 58 + 29 + 2);
  render_switch(74, app_data->nand_saved_data.use_bip_prefix);

  text_out("BT Notes V2.0", 5, 176 - 36);
  char buf[256];
  int notes_data_ptr = (int) app_data->nand_saved_data.notes_data;
  _sprintf(buf, "debug:0x%08x", notes_data_ptr);
  text_out(buf, 5, 176 - 18);
}

void render_edit_note(struct app_data_ *app_data) {
  char buf[60];
  if (app_data->current_edit_action == EDIT_ACTION_NONE) {
    _sprintf(buf, "Edit note #%d", app_data->current_edit_note + 1);
    text_out(buf,                   5, 29 - FONT_HEIGHT_HALF);

    text_out("Replace with",        5, 58 + 29 - FONT_HEIGHT);
    text_out("latest notification", 5, 58 + 29 + 2);
    render_menu_arrow(0, 58 + 29-7);

    text_out("DELETE",              5, 58*2 + 29 - FONT_HEIGHT_HALF);
    render_menu_arrow(0, 58*2 + 29-7);

  } else {
    if (app_data->current_edit_action == EDIT_ACTION_REPLACE)
      text_out("Confirm replace?", 5, 29 - FONT_HEIGHT_HALF);
    else if (app_data->current_edit_action == EDIT_ACTION_DELETE)
      text_out("Confirm delete?", 5, 29 - FONT_HEIGHT_HALF);

    text_out("Cancel", 6+7+6, 58 + 29 - FONT_HEIGHT_HALF);
    render_menu_arrow(6, 58 + 29-7);

    text_out("YES", 6+7+6, 58*2 + 29 - FONT_HEIGHT_HALF);
    render_menu_arrow(6, 58*2 + 29-7);
  }
}

void render(struct app_data_ *app_data) {
  if (app_data->current_scr == SCREEN_SETTINGS) {
    render_settings(app_data);
    render_scroll_bar(2, 3);
  } else if (app_data->current_scr == SCREEN_HELP) {
    render_help();
  } else if (
    app_data->current_scr == SCREEN_HOME_1 ||
    app_data->current_scr == SCREEN_HOME_2
  ) {
    render_home_view(app_data);
    render_scroll_bar(app_data->current_scr == SCREEN_HOME_1 ? 0 : 1, 3);
  } else if (app_data->current_scr == SCREEN_EDIT_NOTE) {
    render_edit_note(app_data);
  } else if (app_data->current_scr >= 0) {
    render_note(app_data);
  }
}
