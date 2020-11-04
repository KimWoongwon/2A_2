#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <ws2tcpip.h> 
#include <time.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    4096
#define NUMBER_COUNT 3

#pragma region Error_Function
void err_quit(const char* msg)
{
	LPVOID lpmsgbuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&lpmsgbuf,
		0, NULL);
	MessageBox(NULL, (LPCSTR)lpmsgbuf, msg, MB_OK);
	LocalFree(lpmsgbuf);
	exit(-1);
}

void err_display(const char* msg)
{
	LPVOID lpmsgbuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&lpmsgbuf,
		0, NULL);
	printf("[%s] %s\n", msg, (LPSTR)lpmsgbuf);
	LocalFree(lpmsgbuf);
}
#pragma endregion

enum PROTOCOL
{
	INTRO = 1,
	ANSWER_NUM,
	ANSWER_RESULT,
	ANSWER_ERROR,
	CORRECT_ANSWER_RESULT,
	GAME_PROCESS,
	GAME_RESULT
};

enum GAME_PROCESS
{
	GAME_CONTINUE = 1,
	GAME_OVER
};

int recvn(SOCKET sock, char* buf, int len, int flags);

int Packing(char* _buf, PROTOCOL _protocol, const int* inputarr, int arrsize);
int Packing(char* _buf, PROTOCOL _protocol, int select);
void UnPacking(const char* _buf, char* msg);

bool PacketRecv(SOCKET _sock, char* _buf);
PROTOCOL GetProtocol(char* _buf);

int main()
{
	WSADATA  wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	int retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_quit("connect()");
	}

	char buf[BUFSIZE];
	char msg[BUFSIZE];
	int Answer[NUMBER_COUNT];
	int size = 0;
	int select = 0;
	bool endflag = false;
	while (1)
	{
		ZeroMemory(buf, sizeof(buf));
		PacketRecv(sock, buf);
		PROTOCOL protocol = GetProtocol(buf);

		switch (protocol)
		{
		case INTRO:
		case ANSWER_ERROR:
		case ANSWER_RESULT:
			ZeroMemory(msg, sizeof(msg));
			UnPacking(buf, msg);
			printf("%s", msg);

			for (int i = 0; i < NUMBER_COUNT; i++)
			{
				printf("%d : ", i + 1);
				scanf("%d", &Answer[i]);
			}
				
			size = Packing(buf, ANSWER_NUM, Answer, NUMBER_COUNT);
			break;
		case CORRECT_ANSWER_RESULT:
			ZeroMemory(msg, sizeof(msg));
			UnPacking(buf, msg);
			printf("%s", msg);

			PacketRecv(sock, buf);

			ZeroMemory(msg, sizeof(msg));
			UnPacking(buf, msg);
			printf("%s", msg);

			printf("1. 계속하기\n");
			printf("2. 그만하기\n");
			printf("선택 : ");
			scanf("%d", &select);
			if (select == 1)
				select = (int)GAME_CONTINUE;
			else if (select == 2)
				select = (int)GAME_OVER;

			size = Packing(buf, GAME_PROCESS, select);
			break;
		case GAME_RESULT:
			endflag = true;
			break;
		}

		if (!endflag)
		{
			int retval = send(sock, buf, size, 0);
			if (retval == SOCKET_ERROR)
				err_display("send()");
		}
		else
			break;

	}

	closesocket(sock);

	WSACleanup();
	return 0;
}



#pragma region Packing_Function
int Packing(char* _buf, PROTOCOL _protocol, const int* inputarr, int arrsize)
{
	char* ptr = _buf + sizeof(int);
	int size = 0;

	memcpy(ptr, &_protocol, sizeof(PROTOCOL));
	size = size + sizeof(PROTOCOL);
	ptr = ptr + sizeof(PROTOCOL);

	memcpy(ptr, &arrsize, sizeof(int));
	size = size + sizeof(int);
	ptr = ptr + sizeof(int);

	for (int i = 0; i < arrsize; i++)
	{
		memcpy(ptr, &inputarr[i], sizeof(int));
		size = size + sizeof(int);
		ptr = ptr + sizeof(int);
	}

	ptr = _buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}

int Packing(char* _buf, PROTOCOL _protocol, int select)
{
	char* ptr = _buf + sizeof(int);
	int size = 0;

	memcpy(ptr, &_protocol, sizeof(PROTOCOL));
	size = size + sizeof(PROTOCOL);
	ptr = ptr + sizeof(PROTOCOL);

	memcpy(ptr, &select, sizeof(int));
	size = size + sizeof(int);
	ptr = ptr + sizeof(int);

	ptr = _buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}

void UnPacking(const char* _buf, char* msg)
{
	const char* ptr = _buf + sizeof(PROTOCOL);
	int strsize1;

	memcpy(&strsize1, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(msg, ptr, strsize1);
	ptr = ptr + strsize1;
}
#pragma endregion

#pragma region Packet_Function
int recvn(SOCKET sock, char* buf, int len, int flags)
{
	char* ptr = buf;
	int left = len;
	int recived;

	while (left > 0)
	{
		recived = recv(sock, ptr, left, flags);
		if (recived == SOCKET_ERROR)
		{
			return SOCKET_ERROR;
		}
		if (recived == 0)
		{
			break;
		}

		left -= recived;
		ptr += recived;
	}

	return (len - left);
}

bool PacketRecv(SOCKET _sock, char* _buf)
{
	int size;

	int retval = recvn(_sock, (char*)&size, sizeof(size), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("gvalue recv error()");
		return false;
	}
	else if (retval == 0)
	{
		return false;
	}

	retval = recvn(_sock, _buf, size, 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("gvalue recv error()");
		return false;

	}
	else if (retval == 0)
	{
		return false;
	}

	return true;
}

PROTOCOL GetProtocol(char* _buf)
{
	PROTOCOL protocol;
	memcpy(&protocol, _buf, sizeof(PROTOCOL));

	return protocol;
}
#pragma endregion

