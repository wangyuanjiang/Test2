//////////////////////////////////////////////////////////////////////////////
// Description : ��������
//////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <iostream>
#include <winsock2.h>

#include <thread>

#include <iostream>

#include <vector>

#include <cstring>

#include <string>

#include "test11.22-1.pb.h"
//#include "client.cpp"

#pragma comment(lib, "ws2_32.lib")

using namespace test11;
static int connectCount = 0;//��ǰ���ӿͻ��˵�����
static int server = 0;//me

std::vector <SOCKET> clients;//�������������ӵĿͻ��˷���������

void delClient(SOCKET clientsoc)//���ͻ��˶Ͽ�����ʱ����������ɾ���ͻ���

{

	for (int i = 0; i < clients.size(); i++)

	{

		if (clients[i] == clientsoc)

		{

			clients.erase(clients.begin() + i);

			break;

		}

	}

}

void sendMsg(char *buf)//�����пͻ��˷�����Ϣ

{

	std::vector<SOCKET>::iterator it;

	for (it = clients.begin(); it != clients.end(); it++) {


		if (send(*it, buf, strlen(buf) + 1, 0) <= 0)//error?

		{
			printf("���ʹ���!\n");
			break;
		}


	}
	
}

void listenThread(char *buf, SOCKET clientsoc, SOCKADDR_IN clientaddr, int number)//�����̣߳������ͻ��˵���������

{

	printf("----------------------------------------------------------\n");

    printf("���ӳɹ����ͻ���ip:%s\n�ͻ��˱��%d,��ǰ��������%d\n", inet_ntoa(clientaddr.sin_addr), number, connectCount);//inet_ntoa:����ip���ַ�����ʽ���ͻ��˵ı�Ű������Ⱥ�˳��ֵ

	printf("----------------------------------------------------------\n");

	memset(buf, '\0', sizeof(buf));



	//�ͻ��˸�������ʱ������һ�����ݣ����ͻ��˵ı�Ÿ��߿ͻ���

	sendmsg smsg;

	char ss[1024];

	memset(ss, '\0', sizeof(ss));

	smsg.set_clientid(number);

	char tt[16] = "���ѽ���ͻ���";//�̶���ʽ��һ�������ַ�ռ2��char�����Ҫ��һ��charΪ������'\0'
	tt[14] = number + '0';
	//char tt[14] = "hello client";
	//tt[12] = number + '0';
	smsg.set_msg(tt);

	smsg.SerializeToArray(ss, 1024);//���л�

	if (send(clientsoc, ss, strlen(ss) + 1, 0) <= 0)

	{

		printf("���ʹ���!\n");

		return;

	}

	//�����пͻ��˷��ͷ���������Ϣ

	memset(ss, '\0', sizeof(ss));
	
	while (0) {
		printf("�����뷢���ͻ��˵���Ϣ\n");
		/*scanf_s("%s", buf,1024);
		int x = 0;
		smsg.set_clientid(x);
		smsg.set_msg(buf);
		int  len = smsg.ByteSize()+1;
		smsg.SerializeToArray(buf, len);
		smsg.SerializeToArray(ss, 1024);
		sendMsg(buf+4);
		//sendMsg(ss);//δ����sedMsgʱ��ss��ֵ����ȷ��*/

		scanf_s("%s", buf, 1024);
		smsg.set_clientid(number);
		smsg.set_msg(buf);
		smsg.SerializeToArray(ss, 1024);//���л�
		sendMsg(ss);
		if (send(clientsoc, ss, strlen(ss) + 1, 0) <= 0)

		{

			printf("���ʹ���!\n");

			return;

		}
	}

}


void recvFromClient(SOCKET clientsoc, char *buf, int number)//����һ���ͻ��˵���Ϣ��ת�������пͻ���

{

	while (1)

	{

		if (recv(clientsoc, buf, 1024, 0) <= 0)

		{

			printf("�ͻ���%d�ر�����!\n", number);

			delClient(clientsoc);

			connectCount--;

			closesocket(clientsoc);

			return;

		}

		sendMsg(buf);

	}

}
void listenThread1(char *buf, SOCKET clientsoc, SOCKADDR_IN clientaddr, int number)//�����̣߳������ͻ��˵���������

{

	printf("----------------------------------------------------------\n");

	//printf("���ӳɹ����ͻ���ip:%s\n�ͻ��˱��%d,��ǰ��������%d\n", inet_ntoa(clientaddr.sin_addr), number, connectCount);//inet_ntoa:����ip���ַ�����ʽ���ͻ��˵ı�Ű������Ⱥ�˳��ֵ
	printf("�ٴλ�ӭ�����ͻ���ip:%s\n", inet_ntoa(clientaddr.sin_addr));
	printf("----------------------------------------------------------\n");

	//memset(buf, '\0', sizeof(buf));
	//�ͻ��˸�������ʱ������һ�����ݣ����ͻ��˵ı�Ÿ��߿ͻ���
	sendmsg smsg;
	char ss[1024];
	memset(ss, '\0', sizeof(ss));
	//smsg.set_clientid(100);
	char tt[20] = "�ٴλ�ӭ�����ͻ���";//�̶���ʽ��һ�������ַ�ռ2��char�����Ҫ��һ��charΪ������'\0'
	tt[18] = 100 + '0';
	if (send(clientsoc, ss, strlen(ss) + 1, 0) <= 0)
	{
    	printf("���ʹ���!\n");
		return;
	}
	/*//�����пͻ��˷��ͷ���������Ϣ
	memset(ss, '\0', sizeof(ss));
		int x = 0;
		smsg.set_clientid(x);
		smsg.set_msg(buf);
		int len = smsg.ByteSize();
		char *ss1 = new char[len];
		smsg.SerializeToArray(buf,len);//���л�,ss1 errro?
		sendMsg(buf);*/
	while (1) {
		printf("�����뷢���ͻ��˵���Ϣ\n");
		/*scanf_s("%s", buf,1024);
		int x = 0;
		smsg.set_clientid(x);
		smsg.set_msg(buf);
		int  len = smsg.ByteSize()+1;
		smsg.SerializeToArray(buf, len);
		smsg.SerializeToArray(ss, 1024);
		sendMsg(buf+4);
		//sendMsg(ss);//δ����sedMsgʱ��ss��ֵ����ȷ��*/

		scanf_s("%s", buf, 1024);
		smsg.set_clientid(number);
		smsg.set_msg(buf);
		smsg.SerializeToArray(ss, 1024);//���л�
		sendMsg(ss);
		if (send(clientsoc, ss, strlen(ss) + 1, 0) <= 0)

		{

			printf("���ʹ���!\n");

			return;

		}
	}


}


int main()

{
	printf("*****Run server.cpp*****   ");
	SOCKET serversoc;

	SOCKET clientsoc;

	SOCKADDR_IN serveraddr;

	SOCKADDR_IN clientaddr;

	char buf[1024];

	int len;

	static int listenCount = 0;
	
	WSADATA wsa;

	WSAStartup(MAKEWORD(2, 2), &wsa);	//��ʼ��WS2_32.DLL,  MAKEWORD(2, 2)��ʾ����Winsock2.2�汾



										//�����׽���

	if ((serversoc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) <= 0)

	{

		printf("�׽���socket����ʧ��!\n");

		return -1;

	}



	//����Э�飬IP���˿�

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9102);
	serveraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);



	//���׽���

	//bind(serversoc, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	
	auto ret = bind(serversoc, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if(ret)
	{

		printf("�׽��ְ�ʧ��!\n");

		return -1;

	}
	


	printf("��ʼ����...\n");

	//��������

	if (listen(serversoc, 1) != 0)

	{

		printf("����ʧ��!\n");

		return -1;

	}
	len = sizeof(SOCKADDR_IN);
	//��������

	while (1) {

		if ((clientsoc = accept(serversoc, (SOCKADDR *)&clientaddr, &len)) <= 0)

		{

			printf("��������ʧ��!\n");

			return -1;

		}

		else {

			listenCount++;

			connectCount++;

			clients.push_back(clientsoc);//���������ϵĿͻ��˼�������

			std::thread task(listenThread, buf, clientsoc, clientaddr, listenCount);//ÿ���пͻ�����������ʱ���Ϳ���һ���߳̽�������

			task.detach();//detach()������ɶ�����join()�����߳���ɶ���

			std::thread task02(recvFromClient, clientsoc, buf, listenCount);//����һ���̼߳����ÿͻ��˵���Ϣ

			task02.detach();
		
			std::thread task03(listenThread1, buf, clientsoc, clientaddr, listenCount);
			task03.detach();
		}

	}

	WSACleanup();     //�ͷ�WS2_32.DLL

	system("pause");

	return 0;

}


