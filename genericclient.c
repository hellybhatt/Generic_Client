
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<netdb.h>


void exitSysWithError(const char *call){
	fprintf(stderr, "Syscall %s failed with errno= %d: %s\n", call, errno,
	 strerror(errno));
	exit(-1);
}
void tcpClient(struct sockaddr_in ina,char buff[]){
	//printf("My TCP Client\n");
	char dtbuf[100];
	int s = socket(AF_INET, SOCK_STREAM, 0);
	if(s ==-1){
		exitSysWithError("socket()");
	}
	int c = connect (s, (struct sockaddr*)&ina, sizeof(ina));
	if (c==-1){
		exitSysWithError("connect()");
	}
	struct timeval tv;
	tv.tv_sec = 8;
	tv. tv_usec = 0;
	if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0 )
		printf("Error in Timeout");
	int len = strlen(buff);
		write(s, buff, len);
		bzero(buff , len);
		int r = read(s, buff, 100);
		if(r==-1){
			exitSysWithError("read()");
		}
		buff[r] = 0;
		printf("From Server: %s\n",buff);
	int cls = close(s);
	if (cls!=0){
		exitSysWithError("close()");
	}

}
void udpClient(struct sockaddr_in ina, char buff[]){
	//printf("My UDP Client\n");
	char wrt[100];
	int n,r;
	int serverlen;
	struct sockaddr_in addr;
	int len = sizeof(addr);
	serverlen = sizeof(ina);
	int s = socket(AF_INET, SOCK_DGRAM,0);
	if(s==-1){
		exitSysWithError("socket()");
	}
	n = sendto(s, buff, strlen(buff),0, (const struct sockaddr *)&ina , serverlen);
	if(n==-1){
		exitSysWithError("sendto()");
	}
	struct timeval tv;
	tv.tv_sec= 8;
	tv.tv_usec = 0;
	if(setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &tv,sizeof(tv)) < 0)
	{ printf("Timeout Error\n");
	}
	r = recvfrom(s,(char *)wrt, 100, 0, (struct sockaddr *)&addr , &len);
	if (r==-1){
		printf("Timeout Invoked\n");
		exitSysWithError("recvfrom()");
	}
	else {
		wrt[r] = '\0';
		printf("Response:");puts(wrt);
	}
	int c = close(s);
	if (c != 0)
		exitSysWithError("close()");
}
void exitWithError(){
	printf("Please provide valid arguments\n");
	exit(-1);
}


int main(int argc, char** argv){

	//Generic Client Code
	struct servent *sp;
	int opt;
	char *passedstring, *host, *port, *servicename; 
	if (argc != 9){
		exitWithError();
	}
	while((opt = getopt(argc, argv, "m:h:s:p:")) != -1){
		switch(opt){
		case 'm':
			passedstring = optarg;
			break;
		case 'h':
			host = optarg;
			break;
		case 's':
			servicename = optarg;
			break;
		case 'p':
			port = optarg;
			break;
		}
	}
	struct sockaddr_in ina;
	int slen = strlen(passedstring);
	char buff[slen];
	char *IPbuffer;
	char *tcpport = "tcp";
	char *udpport = "udp";
	strcpy(buff, passedstring);
	char *pch = strstr(port, udpport);
	char *tch = strstr(port, tcpport);
	if (pch != NULL)
		sp = getservbyname(servicename,udpport);
	else
		sp = getservbyname(servicename,tcpport);
	if (sp == NULL)
		printf("Port# can not be fetched due to invalid parameters of service");
	ina.sin_family = AF_INET;
	struct hostent *hp;
	hp = gethostbyname(host);
	int r = inet_pton(AF_INET,host, &(ina.sin_addr));
	if (r == 0){
		IPbuffer = inet_ntoa(*((struct in_addr*)hp->h_addr_list[0]));
		ina.sin_addr.s_addr = inet_addr(IPbuffer);
	}
	else
		ina.sin_addr.s_addr=inet_addr(host);
	ina.sin_port = sp->s_port;
	if (tch != NULL){
		tcpClient(ina,buff);
	} else if (pch != NULL){
		udpClient(ina,buff);
	}else{
		exitWithError();
	}
	exit(0);
}
