#ifndef __CALCULATE_H__
#define __CALCULATE_H__
typedef struct PACKED1         //定义一个cpu occupy的结构体
{
char name[20];      //定义一个char类型的数组名name有20个元素
unsigned int user; //定义一个无符号的int类型的user
unsigned int nice; //定义一个无符号的int类型的nice
unsigned int system;//定义一个无符号的int类型的system
unsigned int idle; //定义一个无符号的int类型的idle
}CPU_OCCUPY;

typedef struct PACKED2         //定义一个mem occupy的结构体
{
char name[20];      //定义一个char类型的数组名name有20个元素
unsigned long total; 
char name2[20];
unsigned long free;                       
}MEM_OCCUPY;

typedef struct PACKED3         //定义一个net occupy的结构体
{
unsigned long receive_bytes;
unsigned long receive_packets;  
unsigned long transmit_bytes;
unsigned long transmit_packets;           
}NET_OCCUPY;

typedef struct MYLOADINFO{
	int CPU;
	int MEM;
	int NET;
	int Link;
}MYLOADINFO;
void Update_Load_Info();

#endif
