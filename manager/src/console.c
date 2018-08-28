#include <stdio.h>
#include <string.h>
#include "list.h"
#include "serverlist.h"
extern ListHead listhead_server;
static int prefix(char str1[],char str2[]){//is str1 the prefix of the str2?
	int len = strlen(str1);
	int i;
	for(i = 0;i < len; i++){
		if(str1[i] != str2[i])
			return 0;
	}
	return 1;
}
static void init(){
	printf(">");
}
static void print_help(){
	printf("show server state\n");
	//printf("set refresh time int\n");
	printf("exit\n");
}
static void print_state(){
	struct SERVER *pserver;
	ListHead *plist;
	printf("ServerID\tIP\t\t\tCPU\tMEM\tNET\tLink\tLoadFactor\n");
	list_foreach(plist, &listhead_server){
		pserver = list_entry(plist, struct SERVER, list);
		printf("%s\t\t%s\t\t%d\t%d\t%d\t%d\t%d\n", pserver->ServerID, pserver->IP, pserver->CPU, pserver->MEM, pserver->NET, pserver->Link, pserver->LoadFactor);
	}	
}
/*static void set_refresh_time(int time){
	refresh_time = time;
}*/
void *Thread_console(){
	char cmd[30];
	init();
	while(1){
		scanf("%[^\n]",cmd);
		getchar();
		if(strcmp(cmd, "help") == 0){
			print_help();
		}
		else if(strcmp(cmd, "show server state") == 0){
			print_state();
		}
		/*else if(prefix("set refresh time", cmd) == 1){
			int time;
			sscanf(cmd,"set refresh time %d",&time);
			set_refresh_time(time);
		}*/
		else if(strcmp(cmd, "exit") == 0){
			//pthread_exit(0);
			exit(-1);
		}
		else{
			printf("Unkown Command.please input help to see the command\n");
		}
		init();
	}
}
