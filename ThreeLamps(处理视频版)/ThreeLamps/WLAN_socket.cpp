#include <winsock2.h>                
#include <stdio.h>  
#include "WLAN_socket.h"
#pragma comment(lib,"ws2_32.lib")  

int WLAN_SEND(int buf[3],unsigned char cmd)
{
	//SOCKETǰ��һЩ��飬���Э���İ汾��Ϊ�˱����İ汾��socket������ͨ��  
	//WSAStartup������Ӧ�İ汾��WSAStartup�Ĳ���һ���ǰ汾��Ϣ��һ����һЩ��ϸ��ϸ�ڣ�ע��ߵ�λ  
	//WSAStartup��WSACleanup��Ӧ  
	int err;
	WORD versionRequired;
	WSADATA wsaData;
	versionRequired = MAKEWORD(1, 1);
	err = WSAStartup(versionRequired, &wsaData);//Э���İ汾��Ϣ  

												//ͨ��WSACleanup�ķ���ֵ��ȷ��socketЭ���Ƿ�����  
	//if (!err)
	//{
	//	printf("�ͻ���Ƕ�����Ѿ���!\n");
	//}
	//else
	//{
	//	printf("�ͻ��˵�Ƕ���ִ�ʧ��!\n");
	//
	//	return 0;//����  
	//}

	//����socket����ؼ��ʣ�������һ���Ǹ�ͼ���е�socket�����  
	//ע��socket�������������������������socket��������ϵͳ��socket�����ͣ��Լ�һЩ������Ϣ  
	/*SOCK_DGRAM:�����Ӽ�udp  SOCK_STREAM:�����Ӽ�udp*/
	/*AF_INET��ipv4��*/
	//SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	/*socket����int������ֻ������һ���ؼ��֣������׽�����sockaddr_in �� ������Ҫ������bind*/
	SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if(clientSocket==-1)
	printf("error");
	//socket����У���������һ���ṹ��SOCKADDR_IN����������һЩ��Ϣ����socket��ϵͳ��  
	//�˿ںţ�ip��ַ����Ϣ������洢���Ƿ������˵ļ��������Ϣ  
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

	if(bind(clientSocket, (struct sockaddr*)&client_addr, sizeof(client_addr))==-1);//�����  
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
	/*��clientSocket�����壬����buff������ݵ�֮ǰconnect�ĵ�ַ������û��connect,��sendto��*/
	//send(clientSocket, "hello,this is client", strlen("hello,this is client") + 1, 0);

	int i; int j;
	//sendto(clientSocket, "hello,this is client", strlen("hello,this is client"),0,(struct sockaddr *)&client_addr,sizeof(client_addr));
	//while (1)
	//{
		for (i = 0; i < 1000; i++) { for (j = 0; j < 10000; j++) {} }
		//sendto(clientSocket, "hello,this is client", strlen("hello,this is client"), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
		sendto(clientSocket, buff1, 6, 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
	//}
	


	//����socket���������  
	//recv(clientSocket, receiveBuf, 101, 0);
	//printf("%s\n", receiveBuf);

	//����socket����  
	//send(clientSocket, "hello,this is client", strlen("hello,this is client") + 1, 0);

	//�ر��׽���  
	closesocket(clientSocket);
	//�رշ���  
	WSACleanup();
	return 0;
}