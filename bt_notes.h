/*
  Extended functions for DND
  (C) Maxim Volkov     2019 <Maxim.N.Volkov@ya.ru>
  (C) Xuan Son NGUYEN  2021 <contact@ngxson.com>
*/


#ifndef __BT_NOTES_H__
#define __BT_NOTES_H__

#define DISPLAY_UPDATE_PERIOD  1000
#define NOTE_COUNT             6
#define NOTE_MSG_LEN           256
#define NOTE_TITLE_LEN         32

#define SCREEN_EDIT_NOTE      -120
#define SCREEN_SETTINGS       -100
#define SCREEN_HOME_1         -10
#define SCREEN_HOME_2         -11
#define SCREEN_HELP_PREFIX    -500
#define SCREEN_BK_BEGIN       -501
#define SCREEN_BK_DONE        -502

#define EDIT_ACTION_NONE      0
#define EDIT_ACTION_REPLACE   1
#define EDIT_ACTION_DELETE    2

#define TRUE  1
#define FALSE 0

#define ELF_INDEX_SELF	((int)0xFFFFFFFF)

typedef struct {
  char title[NOTE_TITLE_LEN];
  char msg[NOTE_MSG_LEN];
} note;

typedef struct {
  char signature[8];
  char bright_theme;
  char use_bip_prefix;
  char _[6]; // reserved bits for future features
  note notes[6];
} nand_data_t;

struct app_data_ {
  void*         ret_f;          //  the address of the return function
	Elf_proc_*    proc;

  // UI states
  int           current_scr;
  int           current_edit_note;
  int           current_edit_action;
  int           current_page_num;

  // for updating clock
  int           is_update_clock;
  int           last_clock_minute;

  // copy of data saved in NAND
  nand_data_t   nand_saved_data;
};

void  show_screen (void *return_screen);
void  key_press_screen();
int   dispatch_screen (void *param);
void  screen_job();
void  draw_screen(struct app_data_ *a, char *msg);

#endif