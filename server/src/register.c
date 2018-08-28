#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <mysql/mysql.h>
#include "calculate.h"
#include "monitor.h"
#define DEBUG
extern char IP_MANAGER[];
extern MYLOADINFO  this_server;
extern char ServerID[50];
char my_ip[20] = "0.0.0.0";
void GetMyIP(char *IP){
	FILE *fp;
	char buf[50];
	fp = popen("ifconfig eth0 | awk '/inet 地址/ {printf $2} '","r");
	fgets(buf, sizeof(buf), fp);
	pclose(fp);
	if(buf[4] == ':')
		strcpy(IP,buf+5);//english+5,chinese+7 
	else
		strcpy(IP,buf+7);//english+5,chinese+7
}
void Register(){
	Update_Load_Info();
	GetMyIP(my_ip);
	//serverid = GetServerID();
	Get_serverID_from_IP("10.0.0.3", ServerID);
	MYSQL my_connection;  
  	int res;  
  	
  	mysql_init(&my_connection);  
  
  	if(mysql_real_connect(&my_connection,IP_MANAGER,"greenlight","greenlight","greenlight",0,NULL,0))  
  	{  
    		//printf("Connection success\n");  
  		char s_sql[300];
		//SQL delete
		strcpy(s_sql,"DELETE FROM servers WHERE ServerID='");
		strcat(s_sql,ServerID);
		strcat(s_sql,"'");
		res = mysql_query(&my_connection,s_sql);  
		/*if(!res)  
      			printf("Deleted %lu rows\n",(unsigned long)mysql_affected_rows(&my_connection));  
    		else  
      			fprintf(stderr,"Insert error %d : %s \n",mysql_errno(&my_connection),mysql_error(&my_connection));*/
    		//SQL insert
		int LoadFactor = (10*this_server.CPU + 10*this_server.MEM + this_server.NET \
+ this_server.Link/100)/22;
		sprintf(s_sql,"INSERT INTO servers(ServerID,IP,created_at,updated_at,CPU,MEM,NET,Link,LoadFactor) \
VALUES('%s','%s',now(),now(),%d,%d,%d,%d,%d)", ServerID, my_ip, this_server.CPU, \
this_server.MEM, this_server.NET, this_server.Link, LoadFactor);
    		res = mysql_query(&my_connection,s_sql);  
    		if(!res)  
      			printf("Inserted %lu rows\n",(unsigned long)mysql_affected_rows(&my_connection));  
    		else  
      			fprintf(stderr,"Insert error %d : %s \n",mysql_errno(&my_connection),mysql_error(&my_connection));
    		mysql_close(&my_connection);  
  	}  
  	else{  
    		fprintf(stderr,"Connection failed\n");  
    		if(mysql_errno(&my_connection))  
      			fprintf(stderr,"Connection error %d : %s\n",mysql_errno(&my_connection),mysql_error(&my_connection));
		exit(-1);
  	} 
}
