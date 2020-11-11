#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

enum PROTOCOL { INTRO, GAME_RESULT };
enum RES_GAME { WIN, LOSE, CONTINUE, INPUT_ERROR };

int Packing(char* _buf, PROTOCOL _protocol, int _input)	// player Input
{
	char* ptr = _buf + sizeof(int);
	int size = 0;

	memcpy(ptr, &_protocol, sizeof(PROTOCOL));
	size += sizeof(PROTOCOL);
	ptr += sizeof(PROTOCOL);

	memcpy(ptr, &_input, sizeof(int));
	size += sizeof(int);
	ptr += sizeof(int);

	ptr = _buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}
void UnPacking(const char* _buf, int& _result, char* _player_msg, char* _server_msg)	// Game Continue
{
	const char* ptr = _buf + sizeof(PROTOCOL);
	int msgsize = 0;

	memcpy(&_result, ptr, sizeof(int));
	ptr += sizeof(int);

	// copy player msg
	memcpy(&msgsize, ptr, sizeof(int));
	ptr += sizeof(int);

	memcpy(_player_msg, ptr, msgsize);
	ptr += msgsize;
	
	// copy server msg
	memcpy(&msgsize, ptr, sizeof(int));
	ptr += sizeof(int);

	memcpy(_server_msg, ptr, msgsize);
	ptr += msgsize;
}
void UnPacking(const char* _buf, char* _msg)	// Intro Msg
{
	const char* ptr = _buf + sizeof(PROTOCOL);
	int msgsize = 0;

	memcpy(&msgsize, ptr, sizeof(int));
	ptr += sizeof(int);

	memcpy(_msg, ptr, msgsize);
	ptr += msgsize;
}

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

	char recv_buf[BUFSIZE];
	char send_buf[BUFSIZE];
	bool endflag = false;
	while (1)
	{
		char player_msg[BUFSIZE];
		char server_msg[BUFSIZE];
		int result;

		if (!PacketRecv(sock, recv_buf))
			return 1;

		PROTOCOL protocol = GetProtocol(recv_buf);

		switch (protocol)
		{
		case INTRO:
			ZeroMemory(player_msg, sizeof(player_msg));
			UnPacking(recv_buf, player_msg);
			printf("%s", player_msg);
			break;
		case GAME_RESULT:
			ZeroMemory(player_msg, sizeof(player_msg));
			ZeroMemory(server_msg, sizeof(server_msg));
			UnPacking(recv_buf, result, player_msg, server_msg);
			switch (result)
			{
			case WIN:
			case LOSE:
				endflag = true;
			case INPUT_ERROR:
				printf("%s", player_msg);
				break;
			case CONTINUE:
				printf("%s", player_msg);
				printf("상대방 차례입니다.\n");
				printf("상대방의 숫자 선택입니다.\n");
				printf("%s", server_msg);
				break;
			}
			break;
		}

		if (endflag)
			break;

		int input;
		printf("당신의 차례입니다. 1~3사이의 숫자를 입력해주세요. : ");
		scanf("%d", &input);
		int size = Packing(send_buf, GAME_RESULT, input);

		retval = send(sock, send_buf, size, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}

	}

	closesocket(sock);

	WSACleanup();
	return 0;
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