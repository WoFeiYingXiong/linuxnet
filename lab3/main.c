#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "menu.h"

#define FONTSIZE 10
int PrintMenuOS()
{
    int i, j;
    char data_M[FONTSIZE][FONTSIZE] =
    {
        "          ",
        "  *    *  ",
        " ***  *** ",
        " * *  * * ",
        " * *  * * ",
        " *  **  * ",
        " *      * ",
        " *      * ",
        " *      * ",
        "          "
    };
    char data_e[FONTSIZE][FONTSIZE] =
    {
        "          ",
        "          ",
        "    **    ",
        "   *  *   ",
        "  *    *  ",
        "  ******  ",
        "  *       ",
        "   *      ",
        "    ***   ",
        "          "
    };
    char data_n[FONTSIZE][FONTSIZE] =
    {
        "          ",
        "          ",
        "    **    ",
        "   *  *   ",
        "  *    *  ",
        "  *    *  ",
        "  *    *  ",
        "  *    *  ",
        "  *    *  ",
        "          "
    };
    char data_u[FONTSIZE][FONTSIZE] =
    {
        "          ",
        "          ",
        "  *    *  ",
        "  *    *  ",
        "  *    *  ",
        "  *    *  ",
        "  *    *  ",
        "   *  **  ",
        "    **  * ",
        "          "
    };
    char data_O[FONTSIZE][FONTSIZE] =
    {
        "          ",
        "   ****   ",
        "  *    *  ",
        " *      * ",
        " *      * ",
        " *      * ",
        " *      * ",
        "  *    *  ",
        "   ****   ",
        "          "
    };
    char data_S[FONTSIZE][FONTSIZE] =
    {
        "          ",
        "    ****  ",
        "   **     ",
        "  **      ",
        "   ***    ",
        "     **   ",
        "      **  ",
        "     **   ",
        "  ****    ",
        "          "
    };

    for(i=0; i<FONTSIZE; i++)
    {
        for(j=0; j<FONTSIZE; j++)
        {
            printf("%c", data_M[i][j]);
        }
        for(j=0; j<FONTSIZE; j++)
        {
            printf("%c", data_e[i][j]);
        }
        for(j=0; j<FONTSIZE; j++)
        {
            printf("%c", data_n[i][j]);
        }
        for(j=0; j<FONTSIZE; j++)
        {
            printf("%c", data_u[i][j]);
        }
        for(j=0; j<FONTSIZE; j++)
        {
            printf("%c", data_O[i][j]);
        }
        for(j=0; j<FONTSIZE; j++)
        {
            printf("%c", data_S[i][j]);
        }
        printf("\n");
    }
    return 0;
}

int Quit(int argc, char *argv[])
{
    /* add XXX clean ops */
}

#include"syswrapper.h"
#define MAX_CONNECT_QUEUE   1024
int Replyhi()
{
	char szBuf[MAX_BUF_LEN] = "\0";
	char szReplyMsg[MAX_BUF_LEN] = "hi\0";
	InitializeService();
	while (1)
	{
		ServiceStart();
		RecvMsg(szBuf);
		SendMsg(szReplyMsg);
		ServiceStop();
	}
	ShutdownService();
	return 0;
}

int StartReplyhi(int argc, char *argv[])
{
	int pid;
	/* fork another process */
	pid = fork();
	if (pid < 0)
	{
		/* error occurred */
		fprintf(stderr, "Fork Failed!");
		exit(-1);
	}
	else if (pid == 0)
	{
		/*	 child process 	*/
		Replyhi();
		printf("Reply hi TCP Service Started!\n");
	}
	else
	{
		/* 	parent process	 */
		printf("Please input hello...\n");
	}
}

int Hello(int argc, char *argv[])
{
	char szBuf[MAX_BUF_LEN] = "\0";
	char szMsg[MAX_BUF_LEN] = "hello\0";
	OpenRemoteService();
	SendMsg(szMsg);
	RecvMsg(szBuf);
	CloseRemoteService();
	return 0;
}

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
 
#include <unistd.h>
#include <stdio.h>
#define MAX_IFS 64
 
int BringUpNetInterface()
{
    printf("Bring up interface:lo\n");
    struct sockaddr_in sa;
    struct ifreq ifreqlo;
    int fd;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr("127.0.0.1");
    fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    strncpy(ifreqlo.ifr_name, "lo",sizeof("lo"));
    memcpy((char *) &ifreqlo.ifr_addr, (char *) &sa, sizeof(struct sockaddr));
    ioctl(fd, SIOCSIFADDR, &ifreqlo);
    ioctl(fd, SIOCGIFFLAGS, &ifreqlo);
    ifreqlo.ifr_flags |= IFF_UP|IFF_LOOPBACK|IFF_RUNNING;
    ioctl(fd, SIOCSIFFLAGS, &ifreqlo);
    close(fd);
    
    printf("Bring up interface:eth0\n");
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr("192.168.40.254");
    fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
    strncpy(ifreqlo.ifr_name, "eth0",sizeof("eth0"));
    memcpy((char *) &ifreqlo.ifr_addr, (char *) &sa, sizeof(struct sockaddr));
    ioctl(fd, SIOCSIFADDR, &ifreqlo);
    ioctl(fd, SIOCGIFFLAGS, &ifreqlo);
    ifreqlo.ifr_flags |= IFF_UP|IFF_RUNNING;
    ioctl(fd, SIOCSIFFLAGS, &ifreqlo);
    close(fd);

    printf("List all interfaces:\n");
    struct ifreq *ifr, *ifend;
    struct ifreq ifreq;
    struct ifconf ifc;
    struct ifreq ifs[MAX_IFS];
    int SockFD;
 
 
    SockFD = socket(PF_INET, SOCK_DGRAM, 0);
 
 
    ifc.ifc_len = sizeof(ifs);
    ifc.ifc_req = ifs;
    if (ioctl(SockFD, SIOCGIFCONF, &ifc) < 0)
    {
        printf("ioctl(SIOCGIFCONF): %m\n");
        return 0;
    }
 
    ifend = ifs + (ifc.ifc_len / sizeof(struct ifreq));
    for (ifr = ifc.ifc_req; ifr < ifend; ifr++)
    {
        printf("interface:%s\n", ifr->ifr_name);
#if 0
        if (strcmp(ifr->ifr_name, "lo") == 0)
        {
            strncpy(ifreq.ifr_name, ifr->ifr_name,sizeof(ifreq.ifr_name));
            ifreq.ifr_flags == IFF_UP;
            if (ioctl (SockFD, SIOCSIFFLAGS, &ifreq) < 0)
            {
              printf("SIOCSIFFLAGS(%s): IFF_UP %m\n", ifreq.ifr_name);
              return 0;
            }			
	    }
#endif
	    if (ifr->ifr_addr.sa_family == AF_INET)
        {
            strncpy(ifreq.ifr_name, ifr->ifr_name,sizeof(ifreq.ifr_name));
            if (ioctl (SockFD, SIOCGIFHWADDR, &ifreq) < 0)
            {
              printf("SIOCGIFHWADDR(%s): %m\n", ifreq.ifr_name);
              return 0;
            }
 
            printf("Ip Address %s\n", inet_ntoa( ( (struct sockaddr_in *)  &ifr->ifr_addr)->sin_addr)); 
            printf("Device %s -> Ethernet %02x:%02x:%02x:%02x:%02x:%02x\n", ifreq.ifr_name,
                (int) ((unsigned char *) &ifreq.ifr_hwaddr.sa_data)[0],
                (int) ((unsigned char *) &ifreq.ifr_hwaddr.sa_data)[1],
                (int) ((unsigned char *) &ifreq.ifr_hwaddr.sa_data)[2],
                (int) ((unsigned char *) &ifreq.ifr_hwaddr.sa_data)[3],
                (int) ((unsigned char *) &ifreq.ifr_hwaddr.sa_data)[4],
                (int) ((unsigned char *) &ifreq.ifr_hwaddr.sa_data)[5]);
        }
    }
 
    return 0;
}

//ser.c
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
 
#include<sys/socket.h>
#include<netinet/in.h>
 
int UDP_Server()
{
    int sockSer = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockSer == -1)
    {
        perror("socket");
        exit(1);
    }
 
    struct sockaddr_in addrSer;
    addrSer.sin_family = AF_INET;
    addrSer.sin_port = htons(1234);//???
    addrSer.sin_addr.s_addr = inet_addr("127.0.0.1");//?????
 
    socklen_t addrlen = sizeof(struct sockaddr);
    int ret = bind(sockSer, (struct sockaddr*)&addrSer, addrlen);
    if(ret == -1)
    {
        perror("bind.");
        exit(1);
    }
 
    struct sockaddr_in addrCli;
 
    char sendbuf[256];
    char recvbuf[256];
    while(1)
    {
        recvfrom(sockSer, recvbuf, 256, 0, (struct sockaddr*)&addrCli, &addrlen);
        printf("Server output :>%s\n", recvbuf);
        printf("Server input:>");
        scanf("%s",sendbuf);
        sendto(sockSer, sendbuf, strlen(sendbuf)+1, 0, (struct sockaddr*)&addrCli, addrlen);
    }
 
    close(sockSer);
    return 0;
}
 
 
//cli.c
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
 
#include<sys/socket.h>
#include<netinet/in.h>
 
int UDP_Client()
{
    int sockCli = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockCli == -1)
    {
        perror("socket");
        exit(1);
    }
 
    struct sockaddr_in addrSer;
    addrSer.sin_family = AF_INET;
    addrSer.sin_port = htons(1234);//???
    addrSer.sin_addr.s_addr = inet_addr("127.0.0.1");//?????
 
    socklen_t addrlen = sizeof(struct sockaddr);
 
    char sendbuf[256];
    char recvbuf[256];
    while(1)
    {
        printf("Client input:>");
        scanf("%s",sendbuf);
        sendto(sockCli, sendbuf, strlen(sendbuf)+1, 0, (struct sockaddr*)&addrSer, addrlen);
 
        recvfrom(sockCli, recvbuf, 256, 0, (struct sockaddr*)&addrSer, &addrlen);
        printf("Client output:%s\n", recvbuf);
    }
    close(sockCli);
    return 0;
}
 
 int StartUDPServer(int argc, char *argv[])
{
	int pid;
	/* fork another process */
	pid = fork();
	if (pid < 0)
	{
		/* error occurred */
		fprintf(stderr, "Fork Failed!");
		exit(-1);
	}
	else if (pid == 0)
	{
		/*	 child process 	*/
		UDP_Server();
		printf("UDP Service Started!\n");
	}
	else
	{
		/* 	parent process	 */
		printf("Please input UDP_Client...\n");
	}
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <limits.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ctype.h>

 int HttpClient()
 {
	 int sockfd;
	 char buffer[1024];
	 struct sockaddr_in server_addr;
	 struct hostent *host;
	 int portnumber, nbytes;
	 char host_addr[256];
	 char host_file[1024];
	 char local_file[256];
	 FILE * fp;
	 char request[1024];
	 int send, totalsend;
	 int i;
	 char * pt;

	 //if(argc!=2)
	 //{
	 //	fprintf(stderr,"Usage:%s web-address\a\n",argv[0]);
	 //	exit(1);
	 //}
	 //char *temp = "192.30.255.113";
	 char *temp = "www.github.com";
	 portnumber = 80;
	 //strcpy(host_addr,argv[1]);
	 strcpy(host_addr, temp);
	 //if((host=gethostbyname(argv[1]))==NULL)/*取得主机IP地址*/
	 //{
	 //	fprintf(stderr,"Gethostname error, %s\n", strerror(errno));
	 //	exit(1);
	 //}
	 if ((host = gethostbyname(temp)) == NULL)/*取得主机IP地址*/
	 {
		 fprintf(stderr, "Gethostname error, %s\n", strerror(errno));
		 exit(1);
	 }
	 if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)/*建立SOCKET连接*/
	 {
		 fprintf(stderr, "Socket Error:%s\a\n", strerror(errno));
		 exit(1);
	 }
	 /* 客户程序填充服务端的资料 */
	 bzero(&server_addr, sizeof(server_addr));
	 server_addr.sin_family = AF_INET;
	 server_addr.sin_port = htons(portnumber);
	 server_addr.sin_addr = *((struct in_addr *)host->h_addr);

	 /* 客户程序发起连接请求 */
	 if (connect(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1)/*连接网站*/
	 {
		 fprintf(stderr, "Connect Error:%s\a\n", strerror(errno));
		 exit(1);
	 }

	 sprintf(request, "GET /%s HTTP/1.1\r\nAccept: */*\r\nAccept-Language: zh-cn\r\n\
					  	User-Agent: Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)\r\n\
							Host: %s:%d\r\nConnection: Close\r\n\r\n", host_file, host_addr, portnumber);
	 printf("%s", request);/*准备request，将要发送给主机*/

	 /*取得真实的文件名*/
	 strcpy(local_file, "index.html");
	 /*发送http请求request*/
	 send = 0; totalsend = 0;
	 nbytes = strlen(request);
	 while (totalsend < nbytes) {
		 send = write(sockfd, request + totalsend, nbytes - totalsend);
		 if (send == -1) { printf("send error!%s\n", strerror(errno)); exit(0); }
		 totalsend += send;
		 printf("%d bytes send OK!\n", totalsend);
	 }

	 fp = fopen(local_file, "a");
	 if (!fp) {
		 printf("create file error! %s\n", strerror(errno));
		 return 0;
	 }
	 printf("\nThe following is the response header:\n");
	 i = 0;
	 /* 连接成功了，接收http响应，response */
	 while ((nbytes = read(sockfd, buffer, 1)) == 1)
	 {
		 if (i < 4) {
			 if (buffer[0] == '\r' || buffer[0] == '\n') i++;
			 else i = 0;
			 printf("%c", buffer[0]);/*把http头信息打印在屏幕上*/
		 }
		 else {
			 fwrite(buffer, 1, 1, fp);/*将http主体信息写入文件*/
			 i++;
			 if (i % 1024 == 0) fflush(fp);/*每1K时存盘一次*/
		 }
	 }
	 fclose(fp);
	 /* 结束通讯 */
	 close(sockfd);
	 exit(0);
 }

 int StartHttpClient(int argc, char *argv[])
 {
	 int pid;
	 /* fork another process */
	 pid = fork();
	 if (pid < 0)
	 {
		 /* error occurred */
		 fprintf(stderr, "Fork Failed!");
		 exit(-1);
	 }
	 else if (pid == 0)
	 {
		 /*	 child process 	*/
		 HttpClient();
	 }
 }


int main()
{
    BringUpNetInterface();
    PrintMenuOS();
    SetPrompt("MenuOS>>");
    MenuConfig("version","MenuOS V1.0(Based on Linux 3.18.6)",NULL);
    MenuConfig("quit","Quit from MenuOS",Quit);
    MenuConfig("replyhi", "Reply hi TCP Service", StartReplyhi);
    MenuConfig("hello", "Hello TCP Client", Hello);
    MenuConfig("UDP_Server", "start UDP Service", StartUDPServer);
    MenuConfig("UDP_Client", "UDP Client", UDP_Client);
	MenuConfig("HttpClient", "This is HttpClient", StartHttpClient);
    ExecuteMenu();
}
