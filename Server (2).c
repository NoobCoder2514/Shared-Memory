                                                     /*2020A3PS0536H	ABHISHEK SHAH
                                                       2020AAPS1756H	Rahul Varshney
                                                       2020AAPS2198H	Vishnu Bharadwaz Gandlur
                                                       2020A1PS2501H 	Shaurya J Sharma
                                                       2020B3AA1170H	Soham Barui
                                                       2020AAPS0419H 	Siddhant Maharana
                                                       2020AAPS1206H	Ch.Sai Prajeet Reddy*/
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<stdbool.h>
#include<sys/shm.h>
#include<limits.h>
#include <pthread.h>
#include <string.h>
struct ConnectChannel {
char Name[20];                                    
key_t key ;
int readystatus ;
int next ;
int notpossible ;
int checking;
};
struct ClientMemory{
char name[20]  ;
key_t keey;
int request ;
int num1 ;
int num2 ;
int result;
int inputstatus ;
int anstaken ;
int ansgiven ;                                       
};
struct ConnectChannel *connectptr ;
bool checkprime(int n)
{
        for(int i=2;i<n;i++)
        {
        if((n%i)==0)
        {
                return false;
        }
}
return true;
}
int addition( int n1,int n2 )
{
        
        return(n1+n2);
        
}
int subtraction(int n1,int n2 )
{
        
        return(n1-n2);
       
}
int multiplication(int n1,int n2)
{
        
        return (n1*n2);
        
}

int division(int n1,int n2)
{
       
       return (n1/n2);
        
}

int Isprime(int n1)
{
        
        if(checkprime(n1))
        {
                return 1;
        }
        else return 0;
        
}

int isOdd(int n1)
{
       
        if(n1%2)
        {
                return 1;
        }
        else return 0;
        
}
void *worker(void *arg)
{
int totalrequests = 0;
key_t clientkey = *(key_t*)arg;
int clientmemoryID = shmget(clientkey,100,IPC_CREAT|0666);

if(clientmemoryID<0)
{
        printf("Error in Creating Shared Memory : /n");
        exit(0);
}
struct ClientMemory *clientmemoryptr;
clientmemoryptr = shmat(clientmemoryID,NULL,0);
(*connectptr).readystatus = 1;
printf("Memory Created Succesfully for Client \n");
while((*clientmemoryptr).request==INT_MIN)
{
sleep(1);
}
int flag = 1;
while(flag)
{
 totalrequests++;
(*clientmemoryptr).inputstatus = 0;
(*clientmemoryptr).anstaken = 0;
(*clientmemoryptr).ansgiven = 0;

while(!(*clientmemoryptr).inputstatus)
{
sleep(1);
}
int req = (*clientmemoryptr).request;
int ans ,n1,n2;
n1 = (*clientmemoryptr).num1;
n2 = (*clientmemoryptr).num2; 
switch(req)
{
case 1 : { ans = addition(n1,n2);
          break;
          }
case 2 : { ans = subtraction(n1,n2);
          break;
          }
case 3 : { ans = multiplication(n1,n2);
          break;
          }
case 4 : { ans = division(n1,n2);
          break;
          }
case 5 : { ans = Isprime(n1);
          break;
          }
case 6 : { ans = isOdd(n1);
          break;
          }
case 7 : {
         flag = 0;
         break;
}
 default : {
            printf("\n Invalid Request \n ");
            break;
          }
}
if(flag)
{
(*clientmemoryptr).result = ans ;
(*clientmemoryptr).ansgiven = 1;
while(!(*clientmemoryptr).anstaken)
{
sleep(1);
}
}
else {
shmctl(clientmemoryID,IPC_RMID,NULL);
}
}
printf("\nTotal Requests Made by Client is : %d \n", totalrequests);
}
int main()
{
int numofClients = 0;
char namesofClients[100][20];
int ConnectChannelID ;
key_t K = 1304;
ConnectChannelID = shmget(K,100,IPC_CREAT|0666);
if(ConnectChannelID<0)
{
        printf("Error in Creating Shared Memory : /n");
        exit(0);
}
printf("Creating Comm Channel : \n");
connectptr = shmat(ConnectChannelID,NULL,0);
printf("Comm Channel Created Succesfully : \n");
while(1)
{
(*connectptr).Name[0] = '#';
(*connectptr).key = INT_MIN;
(*connectptr).readystatus = -1;
(*connectptr).next = -1;
(*connectptr).notpossible = 0;
(*connectptr).checking = 1;

while((*connectptr).Name[0]=='#')
{
sleep(1);
}
printf("%s\n",(*connectptr).Name);
for(int i=0;i<numofClients;i++)
{
if(!strcmp((*connectptr).Name,namesofClients[i]))
{
(*connectptr).notpossible = 1;
break;
}
}

if((*connectptr).notpossible)
{
continue;
}
(*connectptr).checking = 0;
strcpy(namesofClients[numofClients],(*connectptr).Name);
numofClients++;

(*connectptr).key = ftok(".",numofClients);
key_t clientkey = (*connectptr).key;

printf("Created Key by Server \n");
printf("Creating Thread to Allocate Memory to Client \n");
pthread_t id;
pthread_create(&id, NULL,&worker,&clientkey);
pthread_detach(id);
while((*connectptr).next == -1)
{
sleep(1);
}
printf("\n");
printf("\nList of All Registered CLients : \n");
for(int i=0;i<numofClients;i++)
{
printf("%s\n",namesofClients[i]);
}
printf("\n");

}
shmctl(ConnectChannelID,IPC_RMID,NULL);
}


