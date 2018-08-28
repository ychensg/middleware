#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "serverlist.h"
#include "instance.h"
#include "curl.h"
void launch_a_instance(int flavorid, int imageid){
	Create_machine(flavorid, imageid);
}
void *Thread_terminate_a_instance(void *arg){
	char ServerID[100];
	strcpy(ServerID, (char*)arg);
	sleep(300);
	Delete_machine(ServerID);
}
void delete_a_instance(struct SERVER * pserver){
	pthread_t id_thread_terminate;
	SetServerDeleted(pserver->ServerID);
	set_weight_zero(pserver->IP);
	assert(pthread_create(&id_thread_terminate, NULL, Thread_terminate_a_instance, (void*)(pserver->ServerID)) == 0);
}
