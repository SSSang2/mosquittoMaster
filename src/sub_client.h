#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef WIN32
#include <unistd.h>
#else
#include <process.h>
#include <winsock2.h>
#define snprintf sprintf_s
#endif

#include <pthread.h>
#include <mosquitto.h>
#include "client_shared.h"

pthread_t tid_sub;
bool process_messages = true;
int msg_count = 0;
void my_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message);
void my_connect_callback(struct mosquitto *mosq, void *obj, int result);
void my_subscribe_callback(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos);
void my_log_callback(struct mosquitto *mosq, void *obj, int level, const char *str);
void print_usage(void);
void subscribeFromMaster(struct client_config *s_cfg);
void subscribeThread(struct client_config *s_cfg);
