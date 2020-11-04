#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

enum PROTOCOL
{
	JOIN = 1,
	LOGIN,
	EXIT,
	LOGOUT,
	DROP,

	LOGIN_RESULT,
	JOIN_RESULT
};

enum RES_LOGIN
{
	ID_ERROR = 1,
	PW_ERROR,
	LOGIN_SUCCESS
};

enum RES_JOIN
{
	JOIN_SUCCESS,
	ID_EXIST
};

struct _UserInfo
{
	char id[255];
	char pw[255];

	_UserInfo(const _UserInfo& info)
	{
		strcpy(id, info.id);
		strcpy(pw, info.pw);
	}
	_UserInfo()
	{

	}
};

int Packing(char* _buf, PROTOCOL _protocol, const char* _str1, const char* _str2)
{
	char* ptr = _buf + sizeof(int);
	int size = 0;

	memcpy(ptr, &_protocol, sizeof(PROTOCOL));
	size += sizeof(PROTOCOL);
	ptr += sizeof(PROTOCOL);

	// first string copy
	int str1_size = strlen(_str1);

	memcpy(ptr, &str1_size, sizeof(int));
	size += sizeof(int);
	ptr += sizeof(int);

	memcpy(ptr, _str1, str1_size);
	size += str1_size;
	ptr += str1_size;

	// second string copy
	int str2_size = strlen(_str2);

	memcpy(ptr, &str2_size, sizeof(int));
	size += sizeof(int);
	ptr += sizeof(int);

	memcpy(ptr, _str2, str2_size);
	size += str2_size;
	ptr += str2_size;

	ptr = _buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}
/*Drop, Logout*/int Packing(char* _buf, PROTOCOL _protocol)
{
	char* ptr = _buf + sizeof(int);
	int size = 0;

	memcpy(ptr, &_protocol, sizeof(PROTOCOL));
	size += sizeof(PROTOCOL);
	ptr += sizeof(PROTOCOL);

	ptr = _buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}
void UnPacking(const char* _buf, RES_LOGIN& _result, char* _msg)
{
	const char* ptr = _buf + sizeof(PROTOCOL);
	int msgsize = 0;

	memcpy(&_result, ptr, sizeof(RES_LOGIN));
	ptr += sizeof(RES_LOGIN);

	memcpy(&msgsize, ptr, sizeof(int));
	ptr += sizeof(int);

	memcpy(_msg, ptr, msgsize);
	ptr += msgsize;
}
void UnPacking(const char* _buf, RES_JOIN& _result, char* _msg)
{
	const char* ptr = _buf + sizeof(PROTOCOL);
	int msgsize = 0;

	memcpy(&_result, ptr, sizeof(RES_JOIN));
	ptr += sizeof(RES_JOIN);

	memcpy(&msgsize, ptr, sizeof(int));
	ptr += sizeof(int);

	memcpy(_msg, ptr, msgsize);
	ptr += msgsize;
}
/*Drop, Logout*/void UnPacking(const char* _buf, char* _msg)
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

	while (left>0)
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

	while (1)
	{

		printf("<<메뉴>>\n");
		printf("1.가입\n");
		printf("2.로그인\n");
		printf("3.종료\n");
		printf("선택:");

		int select;
		scanf("%d", &select);
		if (select == EXIT)
		{
			break;
		}

		_UserInfo info;
		ZeroMemory(&info, sizeof(_UserInfo));

		printf("ID:");
		scanf("%s", info.id);
		printf("PW:");
		scanf("%s", info.pw);

		int size = Packing(buf, (PROTOCOL)select, info.id, info.pw);
		
		retval = send(sock, buf, size, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}
		
		retval = recvn(sock, (char*)&size, sizeof(int), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			break;
		}
		else if (retval == 0)
		{
			break;
		}

		retval = recvn(sock, buf, size, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			break;
		}
		else if (retval == 0)
		{
			break;
		}

		PROTOCOL protocol;
		memcpy(&protocol, buf, sizeof(PROTOCOL));
		RES_LOGIN login_result;
		RES_JOIN join_result;

		char msg[BUFSIZE];
		
		switch (protocol)
		{
		case LOGIN_RESULT:
			ZeroMemory(msg, sizeof(msg));
			UnPacking(buf, login_result, msg);
			switch (login_result)
			{
				case PW_ERROR:
				case ID_ERROR:
					printf("%s\n", msg);
					break;
				case LOGIN_SUCCESS:
					printf("%s\n", msg);

					printf("<<메뉴>>\n");
					printf("4.로그아웃\n");
					printf("5.탈퇴\n");
					printf("선택:");
					int select;
					scanf("%d", &select);

					size = Packing(buf, (PROTOCOL)select);

					retval = send(sock, buf, size, 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}

					retval = recvn(sock, (char*)&size, sizeof(int), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					else if (retval == 0)
					{
						break;
					}

					retval = recvn(sock, buf, size, 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					else if (retval == 0)
					{
						break;
					}

					PROTOCOL protocol;
					memcpy(&protocol, buf, sizeof(PROTOCOL));

					switch (protocol)
					{
					case LOGOUT:
					case DROP:
						ZeroMemory(msg, sizeof(msg));
						UnPacking(buf, msg);
						printf("%s\n", msg);
						break;
					}
					
					break;
			}
			break;
		case JOIN_RESULT:
			ZeroMemory(msg, sizeof(msg));
			UnPacking(buf, join_result, msg);
			switch (join_result)
			{
			case JOIN_SUCCESS:
			case ID_EXIST:
				printf("%s\n", msg);
				break;
			}
			break;			
		}
			
	}

	closesocket(sock);

	WSACleanup();
	return 0;
}