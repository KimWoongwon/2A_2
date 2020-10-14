#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

#pragma region print_error
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

enum
{
	JOIN = 1,
	LOGIN,
	EXIT,
	LOGOUT,
	DROP,

	ID_ERROR = 1,
	PW_ERROR,
	JOIN_ERROR,
	LOGIN_SUCCESS,
	JOIN_SUCCESS,
	ID_EXIST
};
struct _UserInfo
{
	char id[255];
	char pw[255];

	_UserInfo() { ZeroMemory(this, sizeof(_UserInfo)); }
	_UserInfo(const _UserInfo& refinfo)
	{
		ZeroMemory(this, sizeof(_UserInfo));
		strcpy(id, refinfo.id);
		strcpy(pw, refinfo.pw);
	}
};

struct _ServerPacket
{
	int protocol;
	char msg[BUFSIZE];
};

struct _ClientPacket
{
	int protocol;
	_UserInfo info;
};

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
	_ServerPacket s_packet;
	_ClientPacket c_packet;

	while (1)
	{
		printf("<< 메뉴 >>\n");
		printf("1. 가입\n");
		printf("2. 로그인\n");
		printf("3. 종료\n");
		printf("선택 : ");

		int select = 0;
		scanf("%d", &select);

		if (select == EXIT)
			break;

		ZeroMemory(&c_packet, sizeof(_ClientPacket));
		ZeroMemory(&s_packet, sizeof(_ServerPacket));
		
		printf("ID : ");
		scanf("%s", c_packet.info.id);
		printf("PW : ");
		scanf("%s", c_packet.info.pw);

		c_packet.protocol = select;

		retval = send(sock, (char*)&c_packet, sizeof(_ClientPacket), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}

		retval = recvn(sock, (char*)&s_packet, sizeof(_ServerPacket), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("recvn()");
			break;
		}
		else if (retval == 0)
		{
			break;
		}

		switch (s_packet.protocol)
		{
		case JOIN_SUCCESS:
		case ID_EXIST:
		case PW_ERROR:
		case ID_ERROR:
		case JOIN_ERROR:
			printf("[From Server] : %s\n", s_packet.msg);
			break;
		case LOGIN_SUCCESS:
			{
				printf("[From Server] : %s\n", s_packet.msg);

				printf("<< 메뉴 >>\n");
				printf("4. 로그아웃\n");
				printf("5. 탈퇴\n");

				printf("선택 : ");
				scanf("%d", &select);
				ZeroMemory(&c_packet, sizeof(_ClientPacket));
				c_packet.protocol = select;
				retval = send(sock, (char*)&c_packet, sizeof(_ClientPacket), 0);
				if (retval == SOCKET_ERROR)
				{
					err_display("send()");
					break;
				}

				retval = recvn(sock, (char*)&s_packet, sizeof(_ServerPacket), 0);
				if (retval == SOCKET_ERROR)
				{
					err_display("recvn()");
					break;
				}
				else if (retval == 0)
				{
					break;
				}

				printf("[From Server] : %s\n", s_packet.msg);
			}
			break;
		}
	}

	closesocket(sock);
	WSACleanup();
	return 0;
}