#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include "monitor.h"
#include "register.h"
#include "calculate.h"
MYLOADINFO  this_server;
char IP_MANAGER[20] = "192.168.1.1";
int time_interval = 9;
char ServerID[50] = "1234";
void Read_config(){
	FILE *pfile = NULL;
	pfile = fopen("server.conf","r");
	if(pfile == NULL){
		printf("no server.conf.\For example,the format of server.conf is\t IP_MANAGER = 192.168.2.1\n");
		exit(0);
	}
	char oneline[100], label[50], option[50];
	while(fgets(oneline,40,pfile)){
		if(oneline[0] == '#') continue;
		char *find = strchr(oneline,'=');
		strncpy(label, oneline, find-oneline);
		label[find-oneline] = '\0';
		strcpy(option, find + 1);
		int len = strlen(option);
		option[len-1] = '\0';//drop '\n'
		//printf("%s = %s \n", label, option);
		if(strcmp(label, "IP_MANAGER") == 0){
			strcpy(IP_MANAGER, option);
		}
		if(strcmp(label, "time_interval") == 0){
			time_interval = atoi(option);
		}
	}
	fclose(pfile);
}
void UpdateWeb(int option){
	char filename[2][50];
	strcpy(filename[0],"update_full.tar.gz");
	strcpy(filename[1],"update_patch.tar.gz");

	char download[100] = "wget ftp://";
	strcat(download, IP_MANAGER);
	strcat(download, "/");
	strcat(download, filename[option]);

	char tar[100] = "tar -xf ";
	strcat(tar, filename[option]);	

	char total[200] = "cd /var/www && rm -f ";
	strcat(total, filename[option]);
	strcat(total, " && ");
	strcat(total, download);
	strcat(total, " && ");
	strcat(total, tar);
	printf("%s\n",total);
	system(total);
}
void test_root(){
	FILE *fp;
	char s[50];
	fp = popen("echo ~","r");
	if(fgets(s, sizeof(s), fp)){
		pclose(fp);
		if(strcmp(s, "/root\n")){
			perror("not run with root");
			exit(-1);
		}
		printf("run with root");
	}
	else{
		pclose(fp);
		perror("fgets failed");
		exit(-1);
	}
}
int main(){
	test_root();
	Read_config();
	UpdateWeb(0);
	char ServerID[100];
	Register();
	while(1){
		Monitor();
		printf("sleep for %d seconds\n", time_interval);
		sleep(time_interval);
	}
	return 0;
}
