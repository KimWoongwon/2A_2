#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

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

#define IDPW_SIZE 255

enum
{
	ERROR_ID = 1,
	ERROR_PW,
	SUCCESS
};

struct _UserInfo
{
	char id[IDPW_SIZE];
	char pw[IDPW_SIZE];
};

struct _Result
{
	int result;
	char msg[BUFSIZE];
};

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

	_UserInfo info;
	ZeroMemory(&info, sizeof(info));

	retval = recv(sock, buf, BUFSIZE, 0);
	if (retval == SOCKET_ERROR)
		Err_Quit("recv()");
	else if (retval == 0)
		Err_Quit("recv()");

	printf("[From Server] : %s", buf);

	while (1)
	{
		printf("ID: ");
		scanf("%s", info.id);
		printf("PW: ");
		scanf("%s", info.pw);
		
		retval = send(sock, (char*)&info, sizeof(info), 0);
		if (retval == SOCKET_ERROR)
		{
			Err_Display("send()");
			break;
		}
		 
		_Result login_result;

		retval = recvn(sock, (char*)&login_result, sizeof(login_result), 0);
		if (retval == SOCKET_ERROR)
		{
			Err_Display("recv()");
			break;
		}
		else if (retval == 0)
		{
			break;
		}

		printf("[From Server] : %s", login_result.msg);
		if (login_result.result == SUCCESS)
			break;

	}

	closesocket(sock);
	WSACleanup();

	return 0;
}