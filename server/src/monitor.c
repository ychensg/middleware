#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <mysql/mysql.h>
#include "monitor.h"
#include "calculate.h"
extern MYLOADINFO  this_server;
extern const char IP_MANAGER[];
extern char ServerID[50];
void Monitor(){
	Update_Load_Info();
	MYSQL my_connection;  
  	int res;  
  	mysql_init(&my_connection);  

  	if(mysql_real_connect(&my_connection,IP_MANAGER,"greenlight","greenlight","greenlight",0,NULL,0)){  
    		//printf("Connection success\n");  
  		char s_sql[200];
    		//SQL insert
		int LoadFactor = (10*this_server.CPU + 10*this_server.MEM + this_server.NET \
+ this_server.Link/100)/22;
		sprintf(s_sql,"UPDATE servers SET updated_at=now(),CPU=%d,MEM=%d,NET=%d,Link=%d,\
LoadFactor=%d WHERE ServerID = '%s'",this_server.CPU, this_server.MEM,this_server.NET,this_server.Link, LoadFactor, ServerID);
    		res = mysql_query(&my_connection,s_sql);  
    		if(!res)  
      			printf("update %lu rows\n",(unsigned long)mysql_affected_rows(&my_connection));  
    		else  
      			fprintf(stderr,"update error %d : %s \n",mysql_errno(&my_connection),mysql_error(&my_connection));
    		mysql_close(&my_connection);  
  	}  
  	else{  
    		fprintf(stderr,"Connection failed\n");  
    		if(mysql_errno(&my_connection))  
      			fprintf(stderr,"Connection error %d : %s\n",mysql_errno(&my_connection),mysql_error(&my_connection));
  	}
}
