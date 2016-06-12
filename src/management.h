#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mysql.h"
//#include <mosquitto_broker.h>
//#include "pub_client.c"
//#include "sub_client.c"
#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "root"
#define DB_NAME "masterBroker"
#define CHOP(x) x[strlen(x) - 1] = ' '
#define MAX_CLIENT 2
#include <mosquitto_broker.h>
//static int sCount=0;

//struct mosquitto *pMosq[100];
void deleteAllSlave();
void updateIPAddr(const char _name[], const char _ip[], const int port, const int sock);
int registSlave(const char _name[]);
int checkSlaveId(const char _name[]);
