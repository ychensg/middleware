#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <arpa/inet.h>
#include "database.h"
#include "list.h"
#include "serverlist.h"
#include "console.h"
#include "ipvs.h"
#include "instance.h"
//#define DEBUG
char MYIP[20];
char IP_MANAGER[20];
char tenantID[100];
char flavorid[50];
char imageid[50];
ListHead listhead_server;
int GetAvg_LoadFactor(ListHead *p_lisht_server,struct SERVER **pp_least){
	struct SERVER *pserver;
	ListHead *plist;
	int count = 0,sum = 0;
	list_foreach(plist, &listhead_server){
		pserver = list_entry(plist, struct SERVER, list);
		if(count == 0) *pp_least = pserver;
		if((*pp_least)->LoadFactor > pserver->LoadFactor)
			*pp_least = pserver;
		count++;
		sum += pserver->LoadFactor;
	}
	return count?sum/count:0;
}
int writelog(char *fname,char *s){
	FILE *fp;
	fp = fopen(fname, "a");
	if(!fp){
		perror("open file failed");
		return -1;
	}
	else{
		fprintf(fp, "%s\n", s);
		fclose(fp);
	}
	return 1;
}
void test_root(){
	FILE *fp;
	char s[50];
	fp = popen("echo ~","r");
	if(fgets(s, sizeof(s), fp)){
		pclose(fp);
		if(strcmp(s, "/root\n")){
			perror("need root");
		}
	}
	else{
		pclose(fp);
		perror("fgets failed");
	}
}
int main(int argc,char **argv){
	if(argc != 6){
		printf("wrong argument,the first argument should be listen port");
		return 0;
	}
	strcpy(MYIP, argv[1]);
	strcpy(IP_MANAGER, argv[2]);
	strcpy(tenantID, argv[3]);
	strcpy(flavorid, argv[4]);
	strcpy(imageid, argv[5]);
	test_root();
	if(INIT_IPVS() == -1) exit(-1);
	listinit(&listhead_server);
	pthread_t id_thread_console;
	int num_server;
	assert(pthread_create(&id_thread_console, NULL, Thread_console, NULL) == 0);
	while(1){
		free_server_list(&listhead_server);
		num_server = ReadDatabase(&listhead_server);
		struct SERVER *p_least;
		int avg = 0;
		avg = GetAvg_LoadFactor(&listhead_server, &p_least);
		if(avg >= 60 || num_server == 0){
			launch_a_instance(flavorid, imageid);
		}
		else if(avg >= 70){
			launch_a_instance(flavorid, imageid);
			launch_a_instance(flavorid, imageid);
		}
		else if(avg >= 80){
			launch_a_instance(flavorid, imageid);
			launch_a_instance(flavorid, imageid);
			launch_a_instance(flavorid, imageid);
		}
		else if(avg <= 10 && num_server > 1){
			void delete_a_instance(p_least);
		}
		UPDATE_IPVS(&listhead_server);
		sleep(180);
	}
	pthread_join(id_thread_console, NULL);
	return 0;
}
