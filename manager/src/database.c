#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>
#include "database.h"
#include "serverlist.h"
#include "list.h"
int writelog(char *fname,char *s);
void SetServerDeleted(char *ServerID){
	MYSQL my_connection;
	char s_sql[100];
	int res;
	mysql_init(&my_connection);
	if(mysql_real_connect(&my_connection,"114.212.84.28","greenlight","greenlight","greenlight",0,NULL,0)){
		sprintf(s_sql, "UPDATE servers SET deleted_at = now() WHERE ServerID = %s", ServerID);
		res = mysql_query(&my_connection,s_sql);
		if(!res){
      			//printf("Deleted %lu rows\n",(unsigned long)mysql_affected_rows(&my_connection));
			writelog("manager.log","set deleted_at success");
		}
    		else
			writelog("manager.log","set deleted_at error");
      			//fprintf(stderr,"Insert error %d : %s \n",mysql_errno(&my_connection),mysql_error(&my_connection));
    		mysql_close(&my_connection); 
	}
	else{
		writelog("manager.log","Connection failed");  
    		if(mysql_errno(&my_connection))  
      			fprintf(stderr,"Connection error %d : %s\n",mysql_errno(&my_connection),mysql_error(&my_connection));
	}
}
int ReadDatabase(ListHead* listhead_server){
	int num_server = 0;
	MYSQL my_connection;
	MYSQL_RES *result;
	MYSQL_ROW sql_row;  
	MYSQL_FIELD *fd;
  	int res;
	mysql_init(&my_connection);
	if(mysql_real_connect(&my_connection,"114.212.84.28","greenlight","greenlight","greenlight",0,NULL,0))  
  	{
    		writelog("manager.log","Connection success");
  		char s_sql[200];
		//SQL select
		strcpy(s_sql,"SELECT ServerID,IP,CPU,MEM,NET,Link,LoadFactor FROM servers WHERE now()-updated_at<500 and deleted_at is NULL");
		res = mysql_query(&my_connection,s_sql);  
		if(!res){
			result = mysql_store_result(&my_connection);
			if(result){
				printf("the result number is %lu\n ",(unsigned long)mysql_num_rows(result));
				num_server = mysql_num_rows(result);
				while(sql_row = mysql_fetch_row(result)){
					SERVER *p = malloc(sizeof(SERVER));
					strcpy(p->ServerID, sql_row[0]);
					strcpy(p->IP, sql_row[1]);
					p->CPU = atoi(sql_row[2]);
					p->MEM = atoi(sql_row[3]);
					p->NET = atoi(sql_row[4]);
					p->Link = atoi(sql_row[5]);
					p->LoadFactor = atoi(sql_row[6]);
					listaddbefore(listhead_server, &p->list);
				}
			}
		}
    		else  
      			fprintf(stderr,"select error %d : %s \n",mysql_errno(&my_connection),mysql_error(&my_connection));
    		mysql_close(&my_connection);  
  	}  
  	else{
    		fprintf(stderr,"Connection failed\n");  
    		if(mysql_errno(&my_connection))  
      			fprintf(stderr,"Connection error %d : %s\n",mysql_errno(&my_connection),mysql_error(&my_connection));
  	}
	return num_server;
}
