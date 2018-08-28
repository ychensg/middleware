#include <stdio.h>
#include <string.h>
#include "serverlist.h"
#include "list.h"
void free_server_list(ListHead *p_listhead){
	while(!listempty(p_listhead)){
		struct SERVER *pserver;
		pserver = list_entry(p_listhead->next, struct SERVER, list);
		listdel(&pserver->list);
		free(pserver);
	}
}
