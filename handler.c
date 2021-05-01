#include <libbip.h>
#include "bt_notes.h"
#include "notification.h"
#include "utils.h"

void go_back_to_home(struct app_data_ *app_data, int id) {
  app_data->current_scr = (id < 3) ? SCREEN_HOME_1 : SCREEN_HOME_2;
  app_data->current_page_num = 0;
}

int dispatch_screen(void *param) {
  struct app_data_ **app_data_p = get_ptr_temp_buf_2(); //  pointer to a pointer to screen data 
  struct app_data_ *app_data = *app_data_p; //  pointer to screen data
  struct gesture_ *gest = param;
  int id;
  int tx = gest->touch_pos_x;
  int ty = gest->touch_pos_y;
  int *edit_action = &app_data->current_edit_action;


  if (app_data->current_scr == SCREEN_HOME_1) {
    switch (gest->gesture) {
      case GESTURE_SWIPE_UP:
        app_data->current_scr = SCREEN_HOME_2;
        draw_screen(app_data, NULL);
        break;
      case GESTURE_CLICK:
        if (ty < 58)       { app_data->current_scr = 0; }
        else if (ty < 116) { app_data->current_scr = 1; }
        else                              { app_data->current_scr = 2; }
        draw_screen(app_data, NULL);
        break;
      case GESTURE_SWIPE_RIGHT:
        show_menu_animate(app_data->ret_f, (unsigned int) show_watchface, ANIMATE_RIGHT);
        break;
    }


  } else if (app_data->current_scr == SCREEN_HOME_2) {
    switch (gest->gesture) {
      case GESTURE_SWIPE_DOWN:
        app_data->current_scr = SCREEN_HOME_1;
        draw_screen(app_data, NULL);
        break;
      case GESTURE_SWIPE_UP:
        app_data->current_scr = SCREEN_SETTINGS;
        draw_screen(app_data, NULL);
        break;
      case GESTURE_CLICK:
        if (ty < 58)       { app_data->current_scr = 3; }
        else if (ty < 116) { app_data->current_scr = 4; }
        else                              { app_data->current_scr = 5; }
        draw_screen(app_data, NULL);
        break;
      case GESTURE_SWIPE_RIGHT:
        show_menu_animate(app_data->ret_f, (unsigned int) show_watchface, ANIMATE_RIGHT);
        break;
    }


  } else if (app_data->current_scr == SCREEN_SETTINGS) {
    switch (gest->gesture) {
      case GESTURE_SWIPE_DOWN:
        app_data->current_scr = SCREEN_HOME_2;
        draw_screen(app_data, NULL);
        break;
      case GESTURE_CLICK:
        if (tx < 118) {
          if (58 < ty && ty < 116) { app_data->current_scr = SCREEN_HELP; }
        } else {
          if (ty < 58)       flip_switch(app_data, SWITCH_BRIGHT_THEME);
          else if (ty < 116) flip_switch(app_data, SWITCH_BIP_PREFIX);
        }
        draw_screen(app_data, NULL);
        break;
      case GESTURE_SWIPE_RIGHT:
        show_menu_animate(app_data->ret_f, (unsigned int) show_watchface, ANIMATE_RIGHT);
        break;
    }



  } else if (app_data->current_scr == SCREEN_HELP) {
    if (gest->gesture == GESTURE_CLICK) {
      app_data->current_scr = SCREEN_SETTINGS;
      draw_screen(app_data, NULL);
    }



  } else if (app_data->current_scr == SCREEN_EDIT_NOTE) {
    switch (gest->gesture) {
      case GESTURE_CLICK:
        if (ty < 58) break;
        if (*edit_action == EDIT_ACTION_NONE) {
          if (ty < 116) { *edit_action = EDIT_ACTION_REPLACE; }
          else { *edit_action = EDIT_ACTION_DELETE; }

        } else if (*edit_action == EDIT_ACTION_REPLACE) {
          if (ty < 116) { *edit_action = EDIT_ACTION_NONE; }
          else { // REPLACE
            replace_with_last_noti(app_data, app_data->current_edit_note);
            app_data->current_scr = app_data->current_edit_note;
            app_data->current_page_num = 0;
          }
          
        } else if (*edit_action == EDIT_ACTION_DELETE) {
          if (ty < 116) { *edit_action = EDIT_ACTION_NONE; }
          else { // DELETE
            save_note(app_data, app_data->current_edit_note, "", "");
            go_back_to_home(app_data, app_data->current_edit_note);
          }
        }
        
        draw_screen(app_data, NULL);
        break;
      case GESTURE_SWIPE_RIGHT:
        app_data->current_scr = app_data->current_edit_note;
        draw_screen(app_data, NULL);
        break;
    }



  } else {
    // Note view screen
    id = app_data->current_scr;
    switch (gest->gesture) {
      case GESTURE_SWIPE_RIGHT:
        go_back_to_home(app_data, id);
        draw_screen(app_data, NULL);
        break;
      case GESTURE_SWIPE_LEFT:
        app_data->current_edit_note = id;
        app_data->current_scr = SCREEN_EDIT_NOTE;
        app_data->current_edit_action = EDIT_ACTION_NONE;
        draw_screen(app_data, NULL);
        break;
      case GESTURE_SWIPE_UP:
        if (app_data->current_page_num < 2) {
          app_data->current_page_num++;
          draw_screen(app_data, NULL);
        }
        break;
      case GESTURE_SWIPE_DOWN:
        if (app_data->current_page_num > 0) {
          app_data->current_page_num--;
          draw_screen(app_data, NULL);
        }
        break;
    }
    
  }


  return 0;
};