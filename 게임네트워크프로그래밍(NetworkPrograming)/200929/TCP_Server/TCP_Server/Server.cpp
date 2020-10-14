#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>

#define SERVERPORT 9000
#define BUFSIZE 512
#define FILENAME "UserList.dat"

#define ID_ERROR_MSG "없는 아이디입니다\n"
#define PW_ERROR_MSG "패스워드가 틀렸습니다\n"
#define JOIN_ERROR_MSG "가입 최대인원에 도달하여 가입이 불가합니다.\n"
#define LOGIN_SUCCESS_MSG "로그인에 성공했습니다.\n"
#define JOIN_SUCCESS_MSG "가입에 성공했습니다.\n"
#define ID_EXIST_MSG "이미 있는 아이디 입니다.\n"
#define LOGOUT_MSG "로그아웃되었습니다.\n"
#define DROP_MSG "탈퇴하였습니다.\n"

#define PLAYER_COUNT 1
#define JOIN_USER_COUNT 3

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

//fopen
//fclose
enum
{
	JOIN = 1, 
	LOGIN,
	EXIT,
	LOGOUT,
	DROP,

	ID_ERROR=1,
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
} ;

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

struct _ClientInfo
{
	SOCKET sock;
	SOCKADDR_IN addr;
	_UserInfo* info;		// 
	bool islogin;

	_ServerPacket s_packet;
	_ClientPacket c_packet;

};

_ClientInfo* AddClientInfo(SOCKET sock, SOCKADDR_IN addr);
void RemoveClientInfo(_ClientInfo*);
int JoinProcess(_ClientInfo*);
void DropProcess(_ClientInfo*);
int LoginProcess(_ClientInfo*);
_UserInfo* SearchUserInfo(const char*);
void FileLoad();
void FileSave();


_ClientInfo* ClientInfo[PLAYER_COUNT];
int Count = 0;

_UserInfo* UserInfo[JOIN_USER_COUNT];
int JoinCount = 0;

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

	FileLoad();

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

			retval=recvn(ptr->sock, (char*)&ptr->c_packet, sizeof(_ClientPacket), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("recv()");
				FileSave();
				break;
			}
			else if (retval == 0)
			{
				break;
			}

			int result = 0;
			switch (ptr->c_packet.protocol)
			{
			case JOIN:
				result = JoinProcess(ptr);
				ZeroMemory(&(ptr->s_packet), sizeof(_ServerPacket));

				switch (result)
				{
				case JOIN_ERROR:
					ptr->s_packet.protocol = JOIN_ERROR;
					strcpy(ptr->s_packet.msg, JOIN_ERROR_MSG);
					break;
				case ID_EXIST:
					ptr->s_packet.protocol = ID_EXIST;
					strcpy(ptr->s_packet.msg, ID_EXIST_MSG);
					break;
				case JOIN_SUCCESS:
					ptr->s_packet.protocol = JOIN_SUCCESS;
					strcpy(ptr->s_packet.msg, JOIN_SUCCESS_MSG);
					break;
				}
				break;
			case LOGIN:
				result = LoginProcess(ptr);
				ZeroMemory(&(ptr->s_packet), sizeof(_ServerPacket));

				switch (result)
				{
				case ID_ERROR:
					ptr->s_packet.protocol = ID_ERROR;
					strcpy(ptr->s_packet.msg, ID_ERROR_MSG);
					break;
				case PW_ERROR:
					ptr->s_packet.protocol = PW_ERROR;
					strcpy(ptr->s_packet.msg, PW_ERROR_MSG);
					break;
				case LOGIN_SUCCESS:
					ptr->s_packet.protocol = LOGIN_SUCCESS;
					strcpy(ptr->s_packet.msg, LOGIN_SUCCESS_MSG);
					ptr->info = SearchUserInfo(ptr->c_packet.info.id);
					ptr->islogin = true;
					break;
				}
				break;
			case LOGOUT:
				ptr->info = NULL;
				ptr->islogin = false;
				ptr->s_packet.protocol = LOGOUT;
				strcpy(ptr->s_packet.msg, LOGOUT_MSG);
				break;
			case DROP:
				DropProcess(ptr);
				ptr->info = NULL;
				ptr->islogin = false;
				ptr->s_packet.protocol = DROP;
				strcpy(ptr->s_packet.msg, DROP_MSG);
				break;
			}

			retval = send(client_sock, (char*)&ptr->s_packet, sizeof(_ServerPacket), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send()");
				break;
			}
		}

		RemoveClientInfo(ptr);
	}

	closesocket(listen_sock);
	WSACleanup();
	return 0;
}

_ClientInfo* AddClientInfo(SOCKET sock, SOCKADDR_IN addr)
{
	printf("클라이언트 접속: IP = %s port = %d \n",
		inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

	_ClientInfo* ptr = new _ClientInfo();
	ZeroMemory(ptr, sizeof(_ClientInfo));
	ptr->sock = sock;
	memcpy(&(ptr->addr), &addr, sizeof(SOCKADDR_IN));

	ClientInfo[Count++] = ptr;
	return ptr;
}

void RemoveClientInfo(_ClientInfo* _ptr)
{
	printf("클라이언트 종료: IP = %s port = %d \n",
		inet_ntoa(_ptr->addr.sin_addr), ntohs(_ptr->addr.sin_port));

	closesocket(_ptr->sock);
	for (int i = 0; i < Count; i++)
	{
		if (ClientInfo[i] == _ptr)
		{
			delete ClientInfo[i];
			for (int j = i; j < Count - 1; j++)
				ClientInfo[j] = ClientInfo[j + 1];

			break;
		}
	}
	--Count;
}

int JoinProcess(_ClientInfo* clientptr)
{
	if (JoinCount >= JOIN_USER_COUNT)
		return JOIN_ERROR;

	for (int i = 0; i < JoinCount; i++)
	{
		if (!strcmp(UserInfo[i]->id, clientptr->c_packet.info.id))
			return ID_EXIST;
	}

	UserInfo[JoinCount] = new _UserInfo(clientptr->c_packet.info);
	++JoinCount;

	return JOIN_SUCCESS;
}

void DropProcess(_ClientInfo* clientptr)
{
	if (!clientptr->islogin)
		return;

	for (int i = 0; i < JoinCount; i++)
	{
		if (!strcmp(UserInfo[i]->id, clientptr->info->id))
		{
			if (i == 0)
			{
				UserInfo[0] = UserInfo[1];
				break;
			}

			for (int j = i; j < JoinCount - 1; j++)
				UserInfo[j] = UserInfo[j + 1];

			break;
		}
	}
	--JoinCount;
	return;
}

int LoginProcess(_ClientInfo* clientptr)
{
	_UserInfo tempInfo(clientptr->c_packet.info);

	for (int i = 0; i < JoinCount; i++)
	{
		if (!strcmp(tempInfo.id, UserInfo[i]->id))
		{
			if (!strcmp(tempInfo.pw, UserInfo[i]->pw))
				return LOGIN_SUCCESS;
			else
				return PW_ERROR;
		}
	}

	return ID_ERROR;
}

_UserInfo* SearchUserInfo(const char* _id)
{
	for (int i = 0; i < JoinCount; i++)
	{
		if (!strcmp(UserInfo[i]->id, _id))
			return UserInfo[i];
	}
	return NULL;
}

void FileLoad()
{
	FILE* fp = nullptr;
	fp = fopen(FILENAME, "rb");
	if (fp == NULL)
		return;

	while (1)
	{
		_UserInfo info;
		ZeroMemory(&info, sizeof(_UserInfo));
		int retval = fread(&info, 1, sizeof(_UserInfo), fp);

		if (retval != sizeof(_UserInfo))
			break;

		_UserInfo* ptr = new _UserInfo();
		memcpy(ptr, &info, sizeof(_UserInfo));
		UserInfo[JoinCount++] = ptr;
	}
	fclose(fp);
}

void FileSave()
{
	FILE* fp = nullptr;
	fp = fopen(FILENAME, "wb");
	if (fp == NULL)
		err_quit("파일 저장 실패");

	for (int i = 0; i < JoinCount; i++)
	{
		int retval = fwrite(UserInfo[i], 1, sizeof(_UserInfo), fp);
		if (retval != sizeof(_UserInfo))
		{
			fclose(fp);
			err_quit("파일 저장 실패");
		}
	}
	
	fclose(fp);
}