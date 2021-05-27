#include <libbip.h>
#include "utils.h"
#include "bt_notes.h"

// https://stackoverflow.com/questions/14723381/how-strcpy-works-behind-the-scenes
void m_strcpy(char *d, const char *s, int max_length) {
  int i = 0;
  while (*s) {
    i++; if (i >= max_length) break;
    *d++ = *s++;
  }
  *d = 0;
}

int m_strcmp(const char *s1, const char *s2) {
	while (*s1 == *s2++)
		if (*s1++ == 0)
			return (0);
	return (*(unsigned char *)s1 - *(unsigned char *)--s2);
}

void save_nand_data(struct app_data_ *app_data, int mode) {
  nand_data_t *nand_saved_data = &app_data->nand_saved_data;
  ElfWriteSettings(
    ELF_INDEX_SELF,
    nand_saved_data,
    0,
    mode == SAVE_ALL_DATA ? sizeof(nand_data_t) : 16
  );
}

void save_note(struct app_data_ *app_data, int index, char *title, char *content) {
  nand_data_t *nand_saved_data = &app_data->nand_saved_data;
  m_strcpy(nand_saved_data->notes[index].title, title,   NOTE_TITLE_LEN);
  m_strcpy(nand_saved_data->notes[index].msg,   content, NOTE_MSG_LEN);
}

char SIGNATURE[] = "BTN0v3";

void get_nand_data(struct app_data_ *app_data) {
  nand_data_t *nand_saved_data = &app_data->nand_saved_data;
  ElfReadSettings(
    ELF_INDEX_SELF,
    nand_saved_data,
    0,
    sizeof(nand_data_t)
  );
}

void flip_switch(struct app_data_ *app_data, int sw) {
  nand_data_t* nand_saved_data = &app_data->nand_saved_data;
  switch (sw) {
    case SWITCH_BRIGHT_THEME:
      nand_saved_data->bright_theme = !nand_saved_data->bright_theme;
      save_nand_data(app_data, SAVE_ONLY_SETTINGS);
      break;
    case SWITCH_BIP_PREFIX:
      nand_saved_data->use_bip_prefix = !nand_saved_data->use_bip_prefix;
      save_nand_data(app_data, SAVE_ONLY_SETTINGS);
      break;
  }
}
