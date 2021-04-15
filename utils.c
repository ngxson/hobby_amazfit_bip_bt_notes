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

char SIGNATURE[] = "BTN0TE";
char NOTE_DEMO_TITLE[] = "Welcome!";
char NOTE_DEMO_CONTENT[] = "This is your first note. You can modify this note by sending a notification from your phone. The title must start with @bip1. Use prefix @bip2 for editing 2nd note.";
saved_data_t *get_saved_data_ptr() {
  saved_data_t *saved_data;
  ElfReadSettings(ELF_INDEX_SELF, &saved_data, 0, 4);
  if (saved_data == NULL || m_strcmp(saved_data->sigature, SIGNATURE) != 0) {
    saved_data = (saved_data_t *) _pvPortMalloc(sizeof(saved_data_t));
    _memclr(saved_data, sizeof(saved_data_t));
    ElfWriteSettings(ELF_INDEX_SELF, &saved_data, 0, 4);
    m_strcpy(saved_data->sigature, SIGNATURE, 8);
    m_strcpy(saved_data->notes[0].title, NOTE_DEMO_TITLE, NOTE_TITLE_LEN);
    m_strcpy(saved_data->notes[0].msg, NOTE_DEMO_CONTENT, NOTE_MSG_LEN);
  }
  return saved_data;
}
