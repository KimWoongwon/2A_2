#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

#pragma region Error_Message_DEFINE
void Err_Quit(const char* msg)
{
	LPVOID lpmsgbuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpmsgbuf,
		0,
		NULL);
	MessageBox(NULL, (LPCSTR)lpmsgbuf, msg, MB_OK);
	LocalFree(lpmsgbuf);
	exit(-1);
}

void Err_Display(const char* msg)
{
	LPVOID lpmsgbuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpmsgbuf,
		0,
		NULL);
	printf("[%s] %s\n", msg, (LPTSTR)lpmsgbuf);
	LocalFree(lpmsgbuf);
}
#pragma endregion



enum
{
	INTRO = 1,
	MAIN,
	LOGIN,
};

struct _UserInfo
{
	char ID[255];
	char PW[255];
	int select;
};

struct _Packet
{
	int result;
	char msg[BUFSIZE];
};

int recvn(SOCKET sock, char* buf, int len, int flags);
void Print_MainMenu();
void Print_LoginMenu();

int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		Err_Quit("socket()");

	// binding..
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);

	int retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		Err_Quit("connect()");

	char buf[BUFSIZE];
	ZeroMemory(buf, sizeof(buf));

	_Packet packet;
	ZeroMemory(&packet, sizeof(_Packet));
	_UserInfo userinput;
	ZeroMemory(&userinput, sizeof(_UserInfo));

	bool Endflag = false;

	retval = recv(sock, (char*)&packet, sizeof(_Packet), 0);
	if (retval == SOCKET_ERROR)
		Err_Quit("recv()");
	else if (retval == 0)
		Err_Quit("recv()");

	printf("[From Server] : %s", packet.msg);

	while (1)
	{
		switch (packet.result)
		{
		case INTRO:
		case MAIN:
			Print_MainMenu();
			printf("선택 : ");
			scanf("%d", &userinput.select);
			switch (userinput.select)
			{
			case 1:
			case 2:
				printf("ID : ");
				scanf("%s", userinput.ID);
				printf("PW : ");
				scanf("%s", userinput.PW);
				break;
			case 3:
				Endflag = true;
				break;
			}
			break;
		case LOGIN:
			Print_LoginMenu();
			printf("선택 : ");
			scanf("%d", &userinput.select);
			break;
		}

		if (Endflag)
			break;

		retval = send(sock, (char*)&userinput, sizeof(_UserInfo), 0);
		if (retval == SOCKET_ERROR)
		{
			Err_Display("send()");
			break;
		}


		retval = recvn(sock, (char*)&packet, sizeof(_Packet), 0);
		if (retval == SOCKET_ERROR)
		{
			Err_Display("recv()");
			break;
		}
		else if (retval == 0)
		{
			break;
		}

		printf("[From Server] : %s", packet.msg);

	}

	closesocket(sock);
	WSACleanup();

	return 0;
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
			return SOCKET_ERROR;
		else if (recived == 0)
			break;

		left -= recived;
		ptr += recived;
	}
	return (len - left);
}

void Print_MainMenu()
{
	printf("---<메뉴>---\n");
	printf("1.로그인\n");
	printf("2.가입\n");
	printf("3.종료\n");
}
void Print_LoginMenu()
{
	printf("---<메뉴>---\n");
	printf("1.로그아웃\n");
	printf("2.탈퇴\n");
}