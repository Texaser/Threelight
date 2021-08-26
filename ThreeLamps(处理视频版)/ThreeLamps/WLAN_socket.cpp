#include <winsock2.h>                
#include <stdio.h>  
#include "WLAN_socket.h"
#pragma comment(lib,"ws2_32.lib")  

int WLAN_SEND(int buf[3],unsigned char cmd)
{
	//SOCKET前的一些检查，检查协议库的版本，为了避免别的版本的socket，并且通过  
	//WSAStartup启动对应的版本，WSAStartup的参数一个是版本信息，一个是一些详细的细节，注意高低位  
	//WSAStartup与WSACleanup对应  
	int err;
	WORD versionRequired;
	WSADATA wsaData;
	versionRequired = MAKEWORD(1, 1);
	err = WSAStartup(versionRequired, &wsaData);//协议库的版本信息  

												//通过WSACleanup的返回值来确定socket协议是否启动  
	//if (!err)
	//{
	//	printf("客户端嵌套字已经打开!\n");
	//}
	//else
	//{
	//	printf("客户端的嵌套字打开失败!\n");
	//
	//	return 0;//结束  
	//}

	//创建socket这个关键词，这里想一下那个图形中的socket抽象层  
	//注意socket这个函数，他三个参数定义了socket的所处的系统，socket的类型，以及一些其他信息  
	/*SOCK_DGRAM:无连接即udp  SOCK_STREAM:有连接即udp*/
	/*AF_INET即ipv4族*/
	//SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	/*socket就是int，这里只是声明一个关键字，申请套接字是sockaddr_in ， 后面需要将两者bind*/
	SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if(clientSocket==-1)
	printf("error");
	//socket编程中，它定义了一个结构体SOCKADDR_IN来存计算机的一些信息，像socket的系统，  
	//端口号，ip地址等信息，这里存储的是服务器端的计算机的信息  
	/*
	SOCKADDR_IN clientsock_in;
	memset(&clientsock_in, 0, sizeof(clientsock_in));
	clientsock_in.sin_addr.S_un.S_addr = inet_addr("192.168.0.101");
	clientsock_in.sin_family = AF_INET;
	clientsock_in.sin_port = htons(1201);
	 */
	SOCKADDR_IN client_addr;
	memset(&client_addr, 0, sizeof(client_addr));
	client_addr.sin_addr.S_un.S_addr = inet_addr("192.168.31.255");
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(1201);
/*
	bind(clientSocket, (struct sockaddr*)&client_addr, sizeof(client_addr));

	if(bind(clientSocket, (struct sockaddr*)&client_addr, sizeof(client_addr))==-1);//绑定完成  
	{
		;
    }
	*/
	char receiveBuf[100];
	int buff[3] = { 7,200,300 };

	char buff1[6] = {0}, buff2[2], buff3[2], buff4[2];


	//buff1[0] = buff[0] >> 8 & 0x000000ff;
	//buff1[1] = buff[0] & 0x000000ff;
	//buff1[2] = buff[1] >> 8 & 0x000000ff;
	//buff1[3] = buff[1] & 0x000000ff;
	//buff1[4] = buff[2] >> 8 & 0x000000ff;
	//buff1[5] = buff[2] & 0x000000ff;

	buff1[0] = buf[0] >> 8 & 0x000000ff;
	buff1[1] = buf[0] & 0x000000ff;
	buff1[2] = buf[1] >> 8 & 0x000000ff;
	buff1[3] = buf[1] & 0x000000ff;
	buff1[4] = buf[2] >> 8 & 0x000000ff;
	buff1[5] = buf[2] & 0x000000ff;

	int t = strlen(buff1);
	/*以clientSocket的名义，发送buff里的数据到之前connect的地址，这里没有connect,就sendto了*/
	//send(clientSocket, "hello,this is client", strlen("hello,this is client") + 1, 0);

	int i; int j;
	//sendto(clientSocket, "hello,this is client", strlen("hello,this is client"),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
	//while (1)
	//{
		for (i = 0; i < 1000; i++) { for (j = 0; j < 10000; j++) {} }
		//sendto(clientSocket, "hello,this is client", strlen("hello,this is client"), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
		sendto(clientSocket, buff1, 6, 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
	//}
	


	//解释socket里面的内容  
	//recv(clientSocket, receiveBuf, 101, 0);
	//printf("%s\n", receiveBuf);

	//发送socket数据  
	//send(clientSocket, "hello,this is client", strlen("hello,this is client") + 1, 0);

	//关闭套接字  
	closesocket(clientSocket);
	//关闭服务  
	WSACleanup();
	return 0;
}