
//////////////////////////////////////////////////////////////////////////////

// Description : �ͻ�����

//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include <iostream>

#include <winsock2.h>

#include <thread>

#include "test11.22-1.pb.h"

#pragma comment(lib,"ws2_32.lib")
#include<string>
using namespace test11;
using namespace std;
static int number;
static int number1 = 0;

//#pragma warning(suppress : 4996)
//#pragma warning(disable : 4996)

//�������Է���������Ϣ��Ҳ����ʱ������ת���Ŀͻ��˵���Ϣ��clientid = 0����������clientid = number������,clientid = ��������Ŀͻ���

void recvFromServer(SOCKET clientsocket, char *buf)

{

	sendmsg smsg;

	while (1) {

		memset(buf, '\0', sizeof(buf));//�ڴ�ռ��ʼ����������bufÿһ��Ԫ�ض���Ϊ'\0'

		if (recv(clientsocket, buf, 1024, 0) <= 0)//������Ϣ

		{

			printf("�ر�����!\n");

			break;

			closesocket(clientsocket);

		}
		
		smsg.ParseFromArray(buf, 1024);//�����л�

		if (smsg.clientid() == 0) std::cout << "������" << ":";

		else if (smsg.clientid() == number) std::cout << "�ҷ��������������������ظ��ҵ���Ϣ " << ":";

		else std::cout << "�ͻ���" << smsg.clientid() << ":";

		std::cout<< smsg.msg() << "\n";
		//std::cout<< "���Է���������Ϣ"<<buf << "\n";
	}

}



int main()

{
	printf("*****Run client.cpp*****   ");
	SOCKET clientsocket;

	SOCKADDR_IN serveraddr;

	SOCKADDR_IN clientaddr;

	char  buf[1024];

	WSADATA wsa;//WSADTA:������ṹ�������洢 ��WSAStartup�������ú󷵻ص� Windows Sockets���ݡ�������Winsock.dllִ�е����ݡ�

	WSAStartup(MAKEWORD(2, 2), &wsa);	//��ʼ��WS2_32.DLL



										//�����׽���

	if ((clientsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) <= 0)

	{

		printf("�׽���socket����ʧ��!\n");

		system("pause");

		return -1;

	}



	serveraddr.sin_family = AF_INET;

	serveraddr.sin_port = htons(9102);//htons()������һ��16λ���޷��Ŷ���������
	                                  //������������ʽ��ʽת�����ռ�������ʽ��ʽ

	serveraddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//inet_addr()�����ǽ�һ������Ƶ�IP��ַ(��192.168.0.1)ת��Ϊ�����ṹ��
	                                                           //��Ҫ��32λIP��ַ(0xC0A80001)��



	//��������

	printf("����������...\n");

	if (connect(clientsocket, (SOCKADDR *)&serveraddr, sizeof(serveraddr)) != 0)

	{

		printf("����ʧ��!\n");

		system("pause");

		return -1;

	}



	printf("���ӳɹ�!\n");



	//��������ʱ�����ܵ���������һ����Ϣ������Ϣ���б��ͻ��˵�id����ȡ����ֵ��number

	if (recv(clientsocket, buf, 1024, 0) <= 0)

	{

		printf("�ر�����!\n");

		return -1;

		closesocket(clientsocket);

	}

	sendmsg smsg;

	smsg.ParseFromArray(buf, 1024);

	std::cout << "������" << ":" << smsg.msg() << "\n";

	number = smsg.clientid();//id��ֵ��number

	smsg.Clear();



	std::thread task03(recvFromServer, clientsocket, buf);//����������Ϣ�߳�

	task03.detach();

	//std::thread task04(recvFromServer, clientsocket, buf);//����������Ϣ�߳�

	//task04.detach();

	//�������ݣ���Ϊֻ��Ҫ�������ݸ������������Է�����Ϣ����ֱ�ӷ������߳�

	sendmsg smsgs;

	char ss[1024];

	memset(ss, '\0', sizeof(ss));

	printf("����������:\n");

	while (1) {

		scanf_s("%s", buf,1024);
		smsgs.set_clientid(number);

		smsgs.set_msg(buf);
	
		smsgs.SerializeToArray(ss, 1024);//���л�

		if (send(clientsocket, ss, strlen(ss) + 1, 0) <= 0)

		{

			printf("���ʹ���!\n");

			break;

		}

	}



	closesocket(clientsocket);

	WSACleanup();    //�ͷ�WS2_32.DLL

	system("pause");

	return 0;

}
