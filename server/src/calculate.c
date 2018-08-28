#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "monitor.h"
#include "calculate.h"
//#define DEBUG
extern MYLOADINFO  this_server;
#define SLEEP_TIME 1
static int Cal_Memoccupy(MEM_OCCUPY *mem)
{
    //printf("mem free %lu mem total %lu\n",mem->free,mem->total);
    unsigned mem_use=(mem->total-mem->free)*100/mem->total;
    #ifdef DEBUG
    //printf("mem use:%u%%\n",mem_use);
    #endif
    return mem_use;
}

static void Get_Memoccupy (MEM_OCCUPY *mem) //对无类型get函数含有一个形参结构体类弄的指针O
{
    FILE *fd;          
    int n;             
    char buff[256];   
    MEM_OCCUPY *m;
    m=mem;
                                                                                                             
    fd = fopen ("/proc/meminfo", "r"); 
    fgets (buff, sizeof(buff), fd); 
    sscanf (buff, "%s %lu %s", m->name, &m->total, m->name2); //MemTotal
    
    fgets (buff, sizeof(buff), fd); //从fd文件中读取长度为buff的字符串再存到起始地址为buff这个空间里 
    sscanf (buff, "%s %lu", m->name2, &m->free); //MemFree
    
    fclose(fd);     //关闭文件fd
}




static int Cal_Cpuoccupy (CPU_OCCUPY *o, CPU_OCCUPY *n)
{   
    unsigned long od, nd;    
    unsigned long id, sd;
    int cpu_use = 0;   
    
    od = (unsigned long) (o->user + o->nice + o->system +o->idle);//第一次(用户+优先级+系统+空闲)的时间再赋给od
    nd = (unsigned long) (n->user + n->nice + n->system +n->idle);//第二次(用户+优先级+系统+空闲)的时间再赋给od
      
    id = (unsigned long) (n->user - o->user);    //用户第一次和第二次的时间之差再赋给id
    sd = (unsigned long) (n->system - o->system);//系统第一次和第二次的时间之差再赋给sd
    if((nd-od) != 0)
    cpu_use = (int)((sd+id)*100)/(nd-od); //((用户+系统)乘100)除(第一次和第二次的时间差)再赋给g_cpu_used
    else cpu_use = 0;
    #ifdef DEBUG
    //printf("cpu_use: %u%%\n",cpu_use);
    #endif
    return cpu_use;
}

static void Get_Cpuoccupy (CPU_OCCUPY *cpust) //对无类型get函数含有一个形参结构体类弄的指针O
{   
    FILE *fd;         
    int n;            
    char buff[256]; 
    CPU_OCCUPY *cpu_occupy;
    cpu_occupy=cpust;                                                                                       
    fd = fopen ("/proc/stat", "r"); 
    fgets (buff, sizeof(buff), fd);
    sscanf (buff, "%s %u %u %u %u", cpu_occupy->name, &cpu_occupy->user, &cpu_occupy->nice,&cpu_occupy->system, &cpu_occupy->idle);
    fclose(fd);     
}


static int Cal_Netoccupy (NET_OCCUPY *o, NET_OCCUPY *n)
{   
    unsigned long od, nd;    
    unsigned long average_bytes;
    od = (unsigned long) (o->receive_bytes+o->transmit_bytes);
    nd = (unsigned long) (n->receive_bytes+n->transmit_bytes);
    average_bytes=(nd-od)/SLEEP_TIME;
    return 8*average_bytes/10000;//10Mbps
}


static void Get_Netoccupy (NET_OCCUPY *net) //对无类型get函数含有一个形参结构体类弄的指针O
{
    FILE *fd;          
    int n;             
    char buff[256];   
    NET_OCCUPY *m;
    m=net;
    unsigned long num;                                                                                                         
    fd = fopen ("/proc/net/dev", "r"); 
    char word[10];  
    fgets (buff, sizeof(buff), fd); 
    fgets (buff, sizeof(buff), fd); 
    fgets (buff, sizeof(buff), fd); 
    fgets (buff, sizeof(buff), fd); //从fd文件中读取长度为buff的字符串再存到起始地址为buff这个空间里 4
    //printf("net %s\n",buff);
    sscanf (buff, "%s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",word,\
    &m->receive_bytes,&m->receive_packets,&num,&num,&num,&num,&num,&num, \
    &m->transmit_bytes,&m->transmit_packets,&num,&num,&num,&num,&num,&num); 
    //printf("net: %lu %lu %lu %lu \n",m->receive_bytes,m->receive_packets,m->transmit_bytes,m->transmit_packets);    
    fclose(fd);     //关闭文件fd
}
static int Get_CurrentLink(){
	FILE * fp;
	char buffer[80];
	fp=popen("netstat -an | grep ESTABLISHED | wc -l", "r");
	fgets(buffer,sizeof(buffer),fp);
	//printf("%s",buffer);
	pclose(fp);
	int i=0;
	while(buffer[i]!='\r' && buffer[i] != '\n'){
		i++;
	}
	buffer[i]=0;
	return atoi(buffer);
}
void Update_Load_Info(){
	CPU_OCCUPY cpu_stat1;
	CPU_OCCUPY cpu_stat2;
	MEM_OCCUPY mem_stat;
	NET_OCCUPY net_stat1;
	NET_OCCUPY net_stat2;
	int cpu;
	unsigned long net_packets;
	//获取内存
	Get_Memoccupy ((MEM_OCCUPY *)&mem_stat);
	this_server.MEM = Cal_Memoccupy(&mem_stat);
	//第一次获取cpu使用情况
	Get_Cpuoccupy((CPU_OCCUPY *)&cpu_stat1);
	Get_Netoccupy(&net_stat1);
	sleep(SLEEP_TIME);
	//第二次获取cpu使用情况
	Get_Cpuoccupy((CPU_OCCUPY *)&cpu_stat2);
	Get_Netoccupy(&net_stat2);
	//计算cpu使用率
	this_server.CPU = Cal_Cpuoccupy ((CPU_OCCUPY *)&cpu_stat1, (CPU_OCCUPY *)&cpu_stat2);
	this_server.NET = Cal_Netoccupy ((NET_OCCUPY *)&net_stat1, (NET_OCCUPY *)&net_stat2);
	this_server.Link = 0;
	this_server.Link = Get_CurrentLink();
}
