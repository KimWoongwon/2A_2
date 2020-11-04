#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>

#define SERVERPORT 9000
#define BUFSIZE 512
#define PLAYER_COUNT 1
#define JOIN_USER_COUNT 100

#define ID_ERROR_MSG "없는 아이디입니다\n"
#define PW_ERROR_MSG "패스워드가 틀렸습니다\n"
#define LOGIN_SUCCESS_MSG "로그인에 성공했습니다.\n"
#define JOIN_SUCCESS_MSG "가입에 성공했습니다.\n"
#define ID_EXIST_MSG "이미있는 아이디입니다.\n"
#define LOGOUT_MSG "로그아웃되었습니다.\n"
#define DROP_MSG "탈퇴하였습니다.\n"

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

struct _ClientInfo
{
	SOCKET sock;
	SOCKADDR_IN addr;
	_UserInfo* info;
	bool login;

	char buf[BUFSIZE];
};

int Packing(char* _buf, PROTOCOL _protocol, int _result, const char* _msg)
{
	// login result
	char* ptr = _buf + sizeof(int);
	int size = 0;

	memcpy(ptr, &_protocol, sizeof(PROTOCOL));
	size += sizeof(PROTOCOL);
	ptr += sizeof(PROTOCOL);

	memcpy(ptr, &_result, sizeof(int));
	size += sizeof(int);
	ptr += sizeof(int);

	int msgsize = strlen(_msg);
	memcpy(ptr, &msgsize, sizeof(int));
	size += sizeof(int);
	ptr += sizeof(int);

	memcpy(ptr, _msg, msgsize);
	size += msgsize;
	ptr += msgsize;

	ptr = _buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}
int Packing(char* _buf, PROTOCOL _protocol, RES_JOIN _result, const char* _msg)
{
	// join result

	char* ptr = _buf + sizeof(int);
	int size = 0;

	memcpy(ptr, &_protocol, sizeof(PROTOCOL));
	size += sizeof(PROTOCOL);
	ptr += sizeof(PROTOCOL);

	memcpy(ptr, &_result, sizeof(RES_JOIN));
	size += sizeof(RES_JOIN);
	ptr += sizeof(RES_JOIN);

	int msgsize = strlen(_msg);
	memcpy(ptr, &msgsize, sizeof(int));
	size += sizeof(int);
	ptr += sizeof(int);

	memcpy(ptr, _msg, msgsize);
	size += msgsize;
	ptr += msgsize;

	ptr = _buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}
int Packing(char* _buf, PROTOCOL _protocol, const char* _msg)
{
	char* ptr = _buf + sizeof(int);
	int size = 0;

	memcpy(ptr, &_protocol, sizeof(PROTOCOL));
	size += sizeof(PROTOCOL);
	ptr += sizeof(PROTOCOL);

	int msgsize = strlen(_msg);
	memcpy(ptr, &msgsize, sizeof(int));
	size += sizeof(int);
	ptr += sizeof(int);

	memcpy(ptr, _msg, msgsize);
	size += msgsize;
	ptr += msgsize;

	ptr = _buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}
void UnPacking(const char* _buf, char* _str1, char* _str2)
{
	const char* ptr = _buf + sizeof(PROTOCOL);
	int str1_size = 0;
	int str2_size = 0;

	memcpy(&str1_size, ptr, sizeof(int));
	ptr += sizeof(int);

	memcpy(_str1, ptr, str1_size);
	ptr += str1_size;

	memcpy(&str2_size, ptr, sizeof(int));
	ptr += sizeof(int);

	memcpy(_str2, ptr, str2_size);
	ptr += str2_size;
}

_ClientInfo* ClientInfo[PLAYER_COUNT];
int Count = 0;

_UserInfo* UserInfo[JOIN_USER_COUNT];
int JoinCount = 0;

_ClientInfo* AddClientInfo(SOCKET sock, SOCKADDR_IN addr)
{
	printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

	_ClientInfo* ptr = new _ClientInfo();
	ZeroMemory(ptr, sizeof(_ClientInfo));
	ptr->sock = sock;
	memcpy(&ptr->addr, &addr, sizeof(addr));
	ptr->login = false;

	ClientInfo[Count++] = ptr;

	return ptr;
}
void ReMoveClientInfo(_ClientInfo* ptr)
{
	printf("\n[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));

	for (int i = 0; i < Count; i++)
	{
		if (ClientInfo[i] == ptr)
		{
			closesocket(ptr->sock);
			delete ptr;
			for (int j = i; j < Count - 1; j++)
			{
				ClientInfo[j] = ClientInfo[j + 1];
			}
			break;
		}
	}
	Count--;
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

int JoinProcess(_UserInfo _info)
{
	for (int i = 0; i < JoinCount; i++)
	{
		if (!strcmp(UserInfo[i]->id, _info.id))
		{
			return ID_EXIST;
		}
	}

	UserInfo[JoinCount] = new _UserInfo(_info);
	++JoinCount;
	return JOIN_SUCCESS;
}
int LoginProcess(_UserInfo _info)
{	
	for (int i = 0; i < JoinCount; i++)
	{
		if (!strcmp(UserInfo[i]->id, _info.id))
		{			
			if (!strcmp(UserInfo[i]->pw, _info.pw))
			{
				return LOGIN_SUCCESS;			
			}
			else
			{
				return PW_ERROR;				
			}			
		}
	}

	return ID_ERROR;
}
void DropProcess(_ClientInfo* _ptr)
{
	if (_ptr->login)
	{
		for (int i = 0; i < JoinCount; i++)
		{
			if (!strcmp(UserInfo[i]->id, _ptr->info->id))
			{
				delete UserInfo[i];
				for (int j = i; j < JoinCount - 1; j++)
				{
					UserInfo[j] = UserInfo[j + 1];
				}
				break;
			}
		}

		JoinCount--;
	}	
}
_UserInfo* SearchUserInfo(const char* _Id)
{
	for (int i = 0; i < JoinCount; i++)
	{
		if (!strcmp(UserInfo[i]->id, _Id))
		{
			return UserInfo[i];
		}
	}

	return nullptr;
}

int main()
{
	WSADATA  wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	int retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_quit("bind()");
	}

	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		err_quit("bind()");
	}

	SOCKADDR_IN clientaddr;
	SOCKET client_sock;
	char buf[BUFSIZE + 1];

	while (1)
	{
		int addrlen = sizeof(clientaddr);
		client_sock =accept(listen_sock, (SOCKADDR*)&clientaddr,&addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}	

		_ClientInfo* ptr = AddClientInfo(client_sock, clientaddr);

		while (1)
		{
			int size;
			retval=recvn(ptr->sock, (char*)&size, sizeof(int), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("recv()");
				break;
			}
			else if (retval == 0)
			{
				break;
			}

			retval = recvn(ptr->sock, ptr->buf, size, 0);
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
			memcpy(&protocol, ptr->buf, sizeof(PROTOCOL));

			_UserInfo info;
			int result;

			switch (protocol)
			{
			case JOIN:
				ZeroMemory(&info, sizeof(_UserInfo));
				UnPacking(ptr->buf, info.id, info.pw);
				result=JoinProcess(info);				
				switch (result)
				{
				case ID_EXIST:
					size = Packing(ptr->buf, JOIN_RESULT, ID_EXIST, ID_EXIST_MSG);
					break;
				case JOIN_SUCCESS:
					size = Packing(ptr->buf, JOIN_RESULT, JOIN_SUCCESS, JOIN_SUCCESS_MSG);
					break;
				}
				break;
			case LOGIN:
				ZeroMemory(&info, sizeof(_UserInfo));
				UnPacking(ptr->buf, info.id, info.pw);
				result = LoginProcess(info);
				switch (result)
				{
				case ID_ERROR:
					size = Packing(ptr->buf, LOGIN_RESULT, ID_ERROR, ID_ERROR_MSG);
					break;
				case PW_ERROR:
					size = Packing(ptr->buf, LOGIN_RESULT, PW_ERROR, PW_ERROR_MSG);
					break;
				case LOGIN_SUCCESS:
					size = Packing(ptr->buf, LOGIN_RESULT, LOGIN_SUCCESS, LOGIN_SUCCESS_MSG);
					ptr->info = SearchUserInfo(info.id);
					ptr->login = true;
					break;				
				}
				break;
			case LOGOUT:
				size = Packing(ptr->buf, LOGOUT, LOGOUT_MSG);
				ptr->info = nullptr;
				ptr->login = false;
				break;
			case DROP:
				DropProcess(ptr);
				size = Packing(ptr->buf, DROP, DROP_MSG);
				ptr->info = nullptr;
				ptr->login = false;
				break;
			}			

			retval = send(client_sock, ptr->buf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send()");
				break;
			}
		}

		ReMoveClientInfo(ptr);
	}

	closesocket(listen_sock);

	WSACleanup();

	return 0;
}