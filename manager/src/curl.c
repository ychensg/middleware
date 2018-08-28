#include <stdio.h>
#include <string.h>
#include "curl.h"
extern char IP_MANAGER[];
extern char tenantID[];
void Get_tokenid(char* temp_tokenid)
{
	FILE * fp;
	char commond[500] = "curl http://";
	char buffer[500];
	strcat(commond, IP_MANAGER);
	strcat(commond, ":5000/v2.0/tokens -d '{\"auth\":{\"passwordCredentials\":{\"username\":\"admin\",\"password\":\"admin\"},\"tenantId\":\"");
	strcat(commond, tenantID);
	strcat(commond, "\"}}' -H 'Content-type: application/json'");
	fp = popen(commond, "r");
	fgets(buffer, sizeof(buffer), fp);
	char * start1 = strstr(buffer, "\"id\": \"");
	start1 += 7;
	char * start2 = strstr(start1, "\"");
	strncpy(temp_tokenid, start1, start2 - start1);
	temp_tokenid[start2 - start1] = '\0';
	pclose(fp);
}

void Get_commondpre(char* commondpre)
{
	char tokenid[100];
        Get_tokenid(tokenid);
	sprintf(commondpre, "curl -H \"X-Auth-Token:%s\" -H \"Content-type: application/json\" http://%s:8774/v2/%s/", tokenid, IP_MANAGER, tenantID);
}

void Get_server(char* serverlist)
{
	char server[500], commondpre[500], buffer[1000];
	FILE* fp;
	Get_commondpre(commondpre);
	strcpy(server, commondpre);
	strcat(server, "servers");
	fp = popen(server, "r");
	fgets(buffer, sizeof(buffer), fp);
	strcpy(serverlist, buffer);
	pclose(fp);
}

void Get_image()
{
	char server[500], commondpre[500], buffer[500];
	FILE* fp;
	Get_commondpre(commondpre);
	strcpy(server, commondpre);
	strcat(server, "images");
	fp = popen(server, "r");
	fgets(buffer, sizeof(buffer), fp);
	pclose(fp);
}

void Get_flavors()
{
	char server[500], commondpre[500], buffer[500];
	FILE* fp;
	Get_commondpre(commondpre);
	strcpy(server, commondpre);
	strcat(server, "flavors");
	fp = popen(server, "r");
	fgets(buffer, sizeof(buffer), fp);
	pclose(fp);
}

void Create_machine(char flavorid[], char imageid[])
{
	char server[500], commondpre[500], buffer[500];
	FILE* fp;
	Get_commondpre(commondpre);
	strcpy(server, commondpre);
	strcat(server, "servers -d '{ \"server\":{ \"flavorRef\":\"");
	strcat(server, flavorid);
	strcat(server, "\",\"imageRef\":\"");
	strcat(server, imageid);
	strcat(server, "\",\"metadata\":{\"My Server Name\":\"server\" }, \"name\":\"new-server-test4\",\"source_type\":\"image\", \"personality\":[{ \"contents\":\"\", \"path\":\"/etc/banner.txt\" }] }}'");
	fp = popen(server, "r");
	fgets(buffer, sizeof(buffer), fp);
	pclose(fp);
}

void Delete_machine(char* serverID)
{
	char server[500];
	char tokenid[500];
	char buffer[500];
	FILE* fp;
        Get_tokenid(tokenid);
	sprintf(server, "curl -X DELETE -H \"X-Auth-Token:%s\" http://%s:8774/v2/%s/servers/%s", tokenid, IP_MANAGER, tenantID, serverID);
	fp = popen(server, "r");
	fgets(buffer, sizeof(buffer), fp);
	pclose(fp);
}

void Get_IP_from_serverID(char* serverID, char* temp_IP)
{
	char server[500];
	char tokenid[100];
	char buffer[500];
	FILE* fp;
	Get_tokenid(tokenid);
	sprintf(server, "curl -H \"X-Auth-Token:%s\" http://%s:8774/v2/%s/servers/%s/ips", tokenid, IP_MANAGER, tenantID, serverID);
	fp = popen(server, "r");
	fgets(buffer, sizeof(buffer), fp);
	char * start1 = strstr(buffer, "\"addr\": \"");
	start1 += 9;
	char * start2 = strstr(start1, "\"");
	if (start1 == NULL || start2 == NULL) perror("error: Get_IP_from_serverID");
	strncpy(temp_IP, start1, start2 - start1);
	temp_IP[start2 - start1] = '\0';
	pclose(fp);
}
void Get_serverID_from_IP(char* IP, char* serverID)
{
	char buffer[1000];
	Get_server(buffer);
	char* start1;
	char* start2 = buffer;
	char temp_IP[500];
	int flag = 0;
	while ((start1 = strstr(start2, "\"id\": \"")) != NULL)
	{
		start1 += 7;
		start2 = strstr(start1, "\"");
		if (start1 == NULL || start2 == NULL) perror("error: Get_serverID_from_IP");
		strncpy(serverID, start1, start2 - start1);
		serverID[start2 - start1] = '\0';
		Get_IP_from_serverID(serverID, temp_IP);
		if (strcmp(IP, temp_IP) == 0)
		{
			flag = 1; break;
		}
	}
	if (!flag) perror("error: No serverID match given IP");
}
