#include <stdio.h>
#include <string.h>
#include "ipvs.h"
#include "list.h"
#include "serverlist.h"
extern char MYIP[20];
int writelog(char *fname,char *s);
int INIT_IPVS(){
	FILE *fp;
	char sh[100] = "ipvsadm -C && ipvsadm -A -t ";
	char s[100];
	strcat(sh, MYIP);
	strcat(sh, ":8080 -s wrr");
	fp = popen(sh,"r");
	if(fgets(s, sizeof(s), fp)){
		writelog("manager.log","ipvs_init failed");
		pclose(fp);
		return -1;
	}
	else{
		pclose(fp);
		return 1;
	}
}
int set_weight_zero(char *IP){
	FILE *fp;
	char sh[100], s[100];
	sprintf(sh, "ipvsadm -e -t %s:8080 -r %s:80 -m -w 0",MYIP,IP);
	fp = popen(sh, "r");
	if(fgets(s, sizeof(s), fp)){
		writelog("manager.log", "ipvs set weight zero failed");
		pclose(fp);
		return -1;
	}
	else{
		pclose(fp);
		return 1;
	}
}
void UPDATE_IPVS(ListHead *listhead_server){
	if(INIT_IPVS() == -1) exit(-1);
	FILE *fp;
	char sh[100];
	char s[100];
	struct SERVER *pserver;
	ListHead *plist;
	list_foreach(plist, listhead_server){
		pserver = list_entry(plist, struct SERVER, list);
		sprintf(sh, "ipvsadm -a -t %s:8080 -r %s:80 -m -w %d", MYIP, pserver->IP, 150-pserver->LoadFactor);
		fp = popen(sh, "r");
		if(fgets(s, sizeof(s), fp)){
			writelog("manager.log","ipvs_update failed");
			pclose(fp);
			exit(-1);
		}
		else{
			pclose(fp);
		}
	}
}
