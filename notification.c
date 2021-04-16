#include <libbip.h>

#include "notification.h"
#include "bt_notes.h"
#include "utils.h"

noti_list_t *get_notifications() {
  int fw = get_fw_version();
  int addr = 0;
  switch (fw) {
    case NOT_LATIN_1_1_2_05: addr = 0x20009788; break;
  }
  return (noti_list_t *) addr;
}

int is_supported_fw() {
  return get_notifications() != NULL;
}

int get_note_id_from_noti(noti_t *noti) {
  char *t = noti->title;
  // "@bipN "
  if (
    t[0] == '@'
    && t[1] == 'b'
    && t[2] == 'i'
    && t[3] == 'p'
    && ('1' <= t[4] && t[4] <= '6')
  ) {
    return (int) ((t[4] - '0') - 1); // char to index num
  } else {
    return -1;
  }
}

void scan_notifications(struct app_data_ *app_data) {
  noti_list_t *noti_list = get_notifications();
  saved_data_t *saved_data = get_saved_data_ptr();
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
    m_strcpy(saved_data->notes[index].title, title,   NOTE_TITLE_LEN);
    m_strcpy(saved_data->notes[index].msg,   content, NOTE_MSG_LEN);
    checked[index] = 1;
  }
}
