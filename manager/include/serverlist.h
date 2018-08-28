#ifndef __SERVERLIST_H__
#define __SERVERLIST_H__
#include "list.h"
typedef struct SERVER{
	char ServerID[100];
	char IP[20];
	int CPU;
	int MEM;
	int NET;
	int Link;
	int LoadFactor;
	ListHead list;
}SERVER;
void free_server_list(ListHead *p_listhead);
#endif
