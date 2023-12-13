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

int main()
{  struct ConnectChannel *connectptr ;
  int connectID;
  key_t K = 1304;
connectID = shmget(K,100,0666);
if(connectID<0)
{
        printf("Error in Creating Shared Memory : /n");
        exit(0);
}
connectptr = shmat(connectID,NULL,0);  
printf("Enter your Name : ");
char str[20] ;
scanf("%s",str);
printf("\n");

strcpy((*connectptr).Name, str);
printf("Checking if User Registration is Unique \n");
while((*connectptr).checking)
{
sleep(1);
}
if((*connectptr).notpossible)
{
printf("Server has found Registration with Same Name \n");
exit(0);
}
printf("Creating Key :\n");
while((*connectptr).key==INT_MIN)
{
sleep(1);
}
(*connectptr).next = 1;
printf("Key Created Succesfully \n");
key_t k = (*connectptr).key;
printf("Created Key \n");
int clientID = 0;
struct ClientMemory *clientptr ;
clientID = shmget(k,100,0666);
printf("%d\n",clientID);
if(clientID<0)
{
        printf("Error in Creating Shared Memory : /n");
        exit(0);
}
printf("Memory Created Successfully \n");
clientptr = shmat(clientID,NULL,0);
int req,n1,n2,res;
while(1)
{
printf("The Following Operations are : \n");
printf("Press 1 for Addition of 2 numbers \n");
printf("Press 2 for Subtraction of 2 numbers : \n");
printf("Press 3 for Multiplication of 2 numbers : \n");
printf("Press 4 for Division of 2 numbers : \n");
printf("Press 5 for checking if the number is Prime or not : \n");
printf("Press 6 for checking if number is even or odd: \n");
printf("Press 7 Unregistering from Server : \n");

printf("Enter your Request : ");
scanf("%d",&req);
printf("\n");
if((req>=1)&&(req<=4))
{
printf("Enter num1 : ");
scanf("%d",&n1);
printf("\n");
printf("Enter num2 : ");
scanf("%d",&n2);
printf("\n");
}
else if((req==5)||(req==6)){
printf("Enter number : ");
scanf("%d",&n1);
printf("\n");
}
else if(req==7)
{
 (*clientptr).request = req;
 (*clientptr).inputstatus = 1;
 exit(0);
}
else {
printf("Invalid Request : \n");
continue;
}
(*clientptr).num1 = n1;
(*clientptr).num2 = n2;
(*clientptr).request = req;
(*clientptr).inputstatus = 1;
while(!(*clientptr).ansgiven)
{ 
sleep(1);
}
int ans = (*clientptr).result ;
(*clientptr).anstaken = 1;

if((req>=1)&&(req<=4))
{
printf("Ans is %d \n " , ans );
}
else if(req==5)
{
if(ans)
{
printf("%d is Prime \n" , n1);
}
else printf("%d is not Prime \n" , n1);
}
else if(req==6)
{
if(ans)
{
printf("%d is Odd \n" , n1);
}
else printf("%d is Even \n" , n1);
}
(*connectptr).Name[0] = '\0';
printf("\n");
}

}
