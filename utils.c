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

void save_nand_data(struct app_data_ *app_data) {
  nand_data_t *nand_saved_data = &app_data->nand_saved_data;
  ElfWriteSettings(ELF_INDEX_SELF, nand_saved_data, 0, sizeof(nand_data_t));
}

char SIGNATURE[] = "BTN0TE";
char NOTE_DEMO_TITLE[] = "Welcome!";
char NOTE_DEMO_CONTENT[] = "This is your first note. You can modify this note by sending a notification from your phone. The title must start with @bip1. Use prefix @bip2 for editing 2nd note.";

void get_nand_data(struct app_data_ *app_data) {
  notes_data_t *notes_data;
  nand_data_t *nand_saved_data = &app_data->nand_saved_data;
  ElfReadSettings(ELF_INDEX_SELF, nand_saved_data, 0, sizeof(nand_data_t));
  notes_data = nand_saved_data->notes_data;

  // check for signature
  if (notes_data == NULL || m_strcmp(notes_data->sigature, SIGNATURE) != 0) {
    // allocate memory and set default data
    notes_data = (notes_data_t *) _pvPortMalloc(sizeof(notes_data_t));
    _memclr(notes_data, sizeof(notes_data_t));
    m_strcpy(notes_data->sigature, SIGNATURE, 8);
    m_strcpy(notes_data->notes[0].title, NOTE_DEMO_TITLE, NOTE_TITLE_LEN);
    m_strcpy(notes_data->notes[0].msg, NOTE_DEMO_CONTENT, NOTE_MSG_LEN);
    nand_saved_data->notes_data = notes_data;
    nand_saved_data->bright_theme = 0;
    nand_saved_data->use_bip_prefix = 1;
    save_nand_data(app_data);
  }
}

void flip_switch(struct app_data_ *app_data, int sw) {
  nand_data_t* nand_saved_data = &app_data->nand_saved_data;
  switch (sw) {
    case SWITCH_BRIGHT_THEME:
      nand_saved_data->bright_theme = !nand_saved_data->bright_theme;
      save_nand_data(app_data);
      break;
    case SWITCH_BIP_PREFIX:
      nand_saved_data->use_bip_prefix = !nand_saved_data->use_bip_prefix;
      save_nand_data(app_data);
      break;
  }
}
