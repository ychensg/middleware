#ifndef __DATABASE_H__
#define __DATABASE_H__
#include "list.h"
void SetServerDeleted(char *ServerID);
int ReadDatabase(ListHead* list_server);
#endif
