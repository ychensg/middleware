#ifndef __IPVS_H__
#define __IPVS_H__
#include "list.h"
int INIT_IPVS();
void UPDATE_IPVS(ListHead *p_list_server);
int set_weight_zero(char *IP);
#endif
