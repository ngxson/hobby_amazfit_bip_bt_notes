#ifndef __NOTIFICATION_H__
#define __NOTIFICATION_H__

#define NOTIFICATIONS_LIST_LEN 0xA

typedef struct {
  int unknown1;
  int unknown2;
  int timestamp;
  char *title;
  char *msg;
  char *app_name;
} noti_t;

typedef struct {
  int count;
  noti_t *last_noti;
} noti_list_t;

int is_supported_fw();
void scan_notifications();
noti_list_t *get_notifications();

#endif