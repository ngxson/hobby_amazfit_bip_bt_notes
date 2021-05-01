#include <libbip.h>
#include "bt_notes.h"
#include "notification.h"
#include "utils.h"

noti_list_t *get_notifications() {
  int fw = get_fw_version();
  int addr = 0;
  switch (fw) {
    case NOT_LATIN_1_1_2_05: addr = 0x20009788; break;
    case LATIN_1_1_5_36: addr = 0x2000103C; break;
  }
  return (noti_list_t *) addr;
}

int is_supported_fw() {
  return get_notifications() != NULL;
}

int check_prefix(char *t) {
  int part1 = t[0] == '@';
  int part2a = t[1] == 'b' && t[2] == 'i' && t[3] == 'p';
  int part2b = t[1] == 'B' && t[2] == 'I' && t[3] == 'P';
  int part3 = '1' <= t[4] && t[4] <= '6';
  return part1 && (part2a || part2b) && part3;
}

int get_note_id_from_noti(noti_t *noti) {
  char *t = noti->title;
  if (check_prefix(t)) {
    return (int) ((t[4] - '0') - 1); // char to index num
  } else {
    return -1;
  }
}

void save_note(struct app_data_ *app_data, int index, char *title, char *content) {
  notes_data_t *saved_data = app_data->nand_saved_data.notes_data;
  m_strcpy(saved_data->notes[index].title, title,   NOTE_TITLE_LEN);
  m_strcpy(saved_data->notes[index].msg,   content, NOTE_MSG_LEN);
}

void replace_with_last_noti(struct app_data_ *app_data, int index) {
  noti_list_t *noti_list = get_notifications();
  if (noti_list->count > 0) {
    noti_t *noti = noti_list->notifications[0];
    save_note(app_data, index, noti->title, noti->msg);
  }
}

void scan_notifications(struct app_data_ *app_data) {
  noti_list_t *noti_list = get_notifications();
  char *title;
  char *content;
  int i;
  char checked[] = {0, 0, 0, 0, 0, 0, 0, 0};
  for (i = 0; i < noti_list->count; i++) {
    int index = get_note_id_from_noti(noti_list->notifications[i]);

    // if it does not have @bipN on the title, move on
    if (index < 0 || index > 6) continue;

    // if this note has been checked, skip
    if (checked[index]) continue;

    // save data to RAM
    title = noti_list->notifications[i]->title + 5;
    if (title[0] == ' ') title++;
    content = noti_list->notifications[i]->msg;
    save_note(app_data, index, title, content);
    checked[index] = 1;
  }
}
