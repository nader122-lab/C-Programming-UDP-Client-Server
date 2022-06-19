#include<stdio.h>
#include<string.h>/*Necessaryformemset()*/
#include<stdlib.h>/*Necessaryforexit()*/
#include<netdb.h>/*ThisisneededinUDPserverstoo*/
#include<sys/types.h>
#include <math.h>
#include<sys/socket.h>/*definitionsofstructuresneededforsockets*/
#include<netinet/in.h>/*constantsandstructuresneededforInternetdomainaddresses*/
#include<arpa/inet.h>/*Itisneededbecauseinet_ntoa()*/
#define BUFSIZE 1024

int convertBinToDec (long int n);


/*error ‐wrapperforperror */
void error(char*msg){
 perror(msg);
 exit(0);
}




int main(int argc,char*argv[]){
 int sockid;
 int portno;	
 socklen_t clientlen;
 struct sockaddr_in serveraddr;
 struct sockaddr_in clientaddr;
 struct hostent *hostp;
 char buf[BUFSIZE];
 char*hostaddrp;




 int optval;/*flagvalueforsetsockopt */
 int n;/*messagebytesize*/

 /*checkcommandlinearguments*/
 if(argc !=2){
  fprintf(stderr,"usage:%s<port>\n",argv[0]);
  exit(1);
 }



 portno = atoi(argv[1]); /*Takes argument in cmd linefortheportasinterger */

 /*socket:createtheparentsocket*/
 sockid = socket(PF_INET,SOCK_DGRAM,0);
 if(sockid < 0)
  error("ERROR opening socket");

 optval =1;
 setsockopt(sockid,SOL_SOCKET,SO_REUSEADDR,(const void*)&optval ,sizeof(int));


 
 memset(&serveraddr,0,sizeof(serveraddr));
 serveraddr.sin_family =AF_INET; 
 serveraddr.sin_addr.s_addr =htonl(INADDR_ANY);
 serveraddr.sin_port =htons(portno); 
 /*bind: associate the parent socket with a port*/
 if(bind(sockid,(struct sockaddr*) &serveraddr, sizeof(serveraddr)) == -1) 
  error("ERROR on binding");

 clientlen =sizeof(clientaddr); 
 while(1){ 
  memset(buf,0,BUFSIZE); 
  n=recvfrom(sockid,buf,BUFSIZE,0,(struct sockaddr *)&clientaddr,&clientlen);
  if(n<0) error("ERROR in recvfrom");
 

  hostp = gethostbyaddr((const char*)&clientaddr.sin_addr.s_addr,sizeof(clientaddr.sin_addr.s_addr),AF_INET); 
  if(hostp ==NULL)error("ERROR on get host by addr");
  hostaddrp =inet_ntoa(clientaddr.sin_addr);
  if(hostaddrp ==NULL)error("ERROR on inet_ntoa\n");
  printf("server received datagram from %s(%s) \n",hostp -> h_name,hostaddrp);
  printf("server received %lu/%dbytes:%s\n",strlen(buf),n,buf);

  int dec;
  char buffer[2];
  long int n = atol(buf);
  dec = convertBinToDec(n);
  sprintf( buffer, "%d", dec);
   
  n=sendto(sockid,buffer,strlen(buffer),0,(struct sockaddr *)&clientaddr,clientlen);
  if(n<0) error("ERROR in sendto");

 }
}







int power(int i)
  {
   int j,p=1;
   for(j=1;j<=i;j++)
   p = p*2;
   return(p);
  }

  int convertBinToDec (long int n)
  {
   int x,s=0,i=0,flag=1;
   while(flag==1)
   {
    x=n%10;
    s=s+x*power(i);
    i=i+1;
    n=n/10;
    if(n==0)
    flag=0;
   }
  return s;
 }

