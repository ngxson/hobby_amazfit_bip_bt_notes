/*
  Extended functions for DND
  (C) Maxim Volkov     2019 <Maxim.N.Volkov@ya.ru>
  (C) Xuan Son NGUYEN  2021 <contact@ngxson.com>
*/

#include <libbip.h>

#include "bt_notes.h"
#include "utils.h"
#include "notification.h"
#include "view.h"

//  screen menu structure - each screen has its own
struct regmenu_ screen_data = {
  55, //  main screen number, value 0-255, for custom windows it is better to take from 50 and above
  1, //  auxiliary screen number (usually 1)
  0, //  0
  dispatch_screen, //  pointer to the function handling touch, swipe, long press
  key_press_screen, //  pointer to the function handling pressing the button
  screen_job, //  pointer to the callback function of the timer  
  0, //  0
  show_screen, //  pointer to the screen display function
  0, //  
  0 //  long press of the button
};
int main(int param0, char **argv) { //  here the variable argv is not defined
  show_screen((void *) param0);
}

void show_screen(void *proc) {
  struct app_data_ **app_data_p = get_ptr_temp_buf_2();
  struct app_data_ *app_data;

  if ((proc == *app_data_p)) {
    app_data = *app_data_p;
    *app_data_p = NULL;
    reg_menu( &screen_data, 0); 
    *app_data_p = app_data;

  } else {
    reg_menu( & screen_data, 0);
    *app_data_p = (struct app_data_ *) pvPortMalloc(sizeof(struct app_data_));
    app_data = *app_data_p;
    _memclr(app_data, sizeof(struct app_data_));
    app_data->proc = get_proc_by_addr(main);
    if (proc && app_data->proc->elf_finish) 
      app_data->ret_f = app_data->proc->elf_finish;
    else 
      app_data->ret_f = show_watchface;
    app_data->current_note = SCREEN_HOME_1;
    app_data->current_page_num = 0;
  }

  set_display_state_value(8, 1);
  set_display_state_value(2, 1);

  if (is_supported_fw()) {
    draw_screen(NULL, "Scanning\nnotifications...");
    scan_notifications(app_data);
    set_update_period(1, 100);
  } else {
    draw_screen(NULL, "FW version\nnot supported!");
  }
}

void key_press_screen() {
  struct app_data_ **app_data_p = get_ptr_temp_buf_2();
  struct app_data_ *app_data = *app_data_p;
  show_menu_animate(app_data->ret_f, (unsigned int) show_watchface, ANIMATE_UP);
};

void screen_job() {
  struct app_data_ **app_data_p = get_ptr_temp_buf_2();
  struct app_data_ *app_data = *app_data_p;
  draw_screen(app_data, NULL);
}

/*
void print_debug(struct app_data_ *app_data) {
  set_bg_color(COLOR_BLACK);
  fill_screen_bg();
  set_graph_callback_to_ram_1();
  load_font();
  set_fg_color(COLOR_WHITE);
  noti_list_t *noti_list = get_notifications();
  char buf[32];
  _sprintf(buf, "count %d", noti_list->count);
  text_out(buf, 5, 5);
  text_out(noti_list->notifications[0]->title, 5, 5+20);
  text_out(noti_list->notifications[0]->msg, 5, 5+40);
  repaint_screen_lines(0, 176);
  vTaskDelay(3000);
  stm32_soft_reset();
}
*/

int dispatch_screen(void *param) {
  struct app_data_ **app_data_p = get_ptr_temp_buf_2(); //  pointer to a pointer to screen data 
  struct app_data_ *app_data = *app_data_p; //  pointer to screen data
  struct gesture_ *gest = param;
  int id;


  if (app_data->current_note == SCREEN_HOME_1) {
    switch (gest->gesture) {
      case GESTURE_SWIPE_DOWN:
        //print_debug(app_data);
        break;
      case GESTURE_SWIPE_UP:
        app_data->current_note = SCREEN_HOME_2;
        draw_screen(app_data, NULL);
        break;
      case GESTURE_CLICK:
        if (gest->touch_pos_y < 58)       { app_data->current_note = 0; }
        else if (gest->touch_pos_y < 116) { app_data->current_note = 1; }
        else                              { app_data->current_note = 2; }
        draw_screen(app_data, NULL);
        break;
      case GESTURE_SWIPE_RIGHT:
        show_menu_animate(app_data->ret_f, (unsigned int) show_watchface, ANIMATE_RIGHT);
        break;
    }


  } else if (app_data->current_note == SCREEN_HOME_2) {
    switch (gest->gesture) {
      case GESTURE_SWIPE_DOWN:
        app_data->current_note = SCREEN_HOME_1;
        draw_screen(app_data, NULL);
        break;
      case GESTURE_SWIPE_UP:
        app_data->current_note = SCREEN_HELP;
        draw_screen(app_data, NULL);
        break;
      case GESTURE_CLICK:
        if (gest->touch_pos_y < 58)       { app_data->current_note = 3; }
        else if (gest->touch_pos_y < 116) { app_data->current_note = 4; }
        else                              { app_data->current_note = 5; }
        draw_screen(app_data, NULL);
        break;
      case GESTURE_SWIPE_RIGHT:
        show_menu_animate(app_data->ret_f, (unsigned int) show_watchface, ANIMATE_RIGHT);
        break;
    }

  } else if (app_data->current_note == SCREEN_HELP) {
    switch (gest->gesture) {
      case GESTURE_SWIPE_DOWN:
        app_data->current_note = SCREEN_HOME_2;
        draw_screen(app_data, NULL);
        break;
      case GESTURE_SWIPE_RIGHT:
        show_menu_animate(app_data->ret_f, (unsigned int) show_watchface, ANIMATE_RIGHT);
        break;
    }


  } else {
    switch (gest->gesture) {
      case GESTURE_SWIPE_RIGHT:
        id = app_data->current_note;
        app_data->current_note = (id < 3) ? SCREEN_HOME_1 : SCREEN_HOME_2;
        app_data->current_page_num = 0;
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

// custom function
void draw_screen(struct app_data_ *app_data, char *msg) {
  set_bg_color(COLOR_BLACK);
  fill_screen_bg();
  set_graph_callback_to_ram_1();
  load_font();
  set_fg_color(COLOR_WHITE);

  if (msg != NULL) {
    text_out_center(msg, 88, 68);
  } else {
    render(app_data);
  }

  repaint_screen_lines(0, 176);
  set_update_period(0, 0);
};