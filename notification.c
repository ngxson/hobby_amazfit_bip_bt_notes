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

typedef struct {
  char sigature[NOTE_OFFSET];
  note notes[6];
} saved_data_t;

void scan_notifications(struct app_data_ *app_data) {
  noti_list_t *noti_list = get_notifications();
  Elf_proc_* proc = app_data->proc;
  char *title;
  if (noti_list->count == 0) return;
  int index = get_note_id_from_noti(noti_list->last_noti);
  // if it does not have @bipN on the title, move on
  if (index < 0 || index > 6) return; // continue;
  // else, save the notification to memory
  title = noti_list->last_noti->title + 5;
  if (title[0] == ' ') title++;
  // read -> edit -> save
  saved_data_t *saved_data = (saved_data_t *) _pvPortMalloc(sizeof(saved_data_t));
  char *tmp = (char *) saved_data;
  ElfReadSettings(proc->index_listed, tmp, 0, sizeof(saved_data_t));
  m_strcpy(saved_data->notes[index].title, title,                     NOTE_TITLE_LEN);
  m_strcpy(saved_data->notes[index].msg,   noti_list->last_noti->msg, NOTE_MSG_LEN);
  ElfWriteSettings(proc->index_listed, tmp, 0, sizeof(saved_data_t));
  vPortFree(saved_data);
}
