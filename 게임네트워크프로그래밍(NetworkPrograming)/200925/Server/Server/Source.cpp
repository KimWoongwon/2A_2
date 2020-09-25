#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <list>
using namespace std;


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

#define MSG_INTRO "------Login Server & Client------\n"
#define MSG_SUCCESS "성공했습니다.\n"
#define MSG_FAIL "실패했습니다.\n"
#define MSG_IDERROR "없는 ID입니다.\n"
#define MSG_PWERROR "잘못된 PW입니다.\n"

#define NODATA -1
#define PLAYER_COUNT 1

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

struct _ClientInfo
{
	SOCKET sock;
	SOCKADDR_IN addr;
	_Packet packet;
	_UserInfo data;
	
};

int recvn(SOCKET sock, char* buf, int len, int flags);
_ClientInfo* AddClientInfo(SOCKET sock, SOCKADDR_IN addr);
void RemoveClientInfo(_ClientInfo*);
void ClientInput(_Packet*);

_ClientInfo* ClientInfo[PLAYER_COUNT];
int Count = 0;

int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
		Err_Quit("socket()");

	// binding..
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	int retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (listen_sock == SOCKET_ERROR)
		Err_Quit("bind()");

	retval = listen(listen_sock, SOMAXCONN);
	if (listen_sock == SOCKET_ERROR)
		Err_Quit("listen()");

	SOCKET client_sock;
	SOCKADDR_IN client_addr;
	int addrlen;
	char buf[BUFSIZE + 1];

	bool islogin = false;
	list<_UserInfo>* userList = new list<_UserInfo>();
	list<_UserInfo>::iterator iter;

	srand(time(NULL));

	while (1)
	{
		const char* msgptr;


		while (1)
		{
			addrlen = sizeof(client_addr);
			client_sock = accept(listen_sock, (SOCKADDR*)&client_addr, &addrlen);
			if (client_sock == INVALID_SOCKET)
			{
				Err_Display("accept()");
				break;
			}

			AddClientInfo(client_sock, client_addr);
			if (Count == PLAYER_COUNT)
				break;
		}
		putchar('\n');

		_ClientInfo* ptr = ClientInfo[0];
		ptr->packet.result = INTRO;
		strcpy(ptr->packet.msg, MSG_INTRO);
		retval = send(ptr->sock, (char*)&ptr->packet, sizeof(_Packet), 0);
		if (retval == SOCKET_ERROR)
		{
			Err_Display("send()");
			break;
		}

		while (1)
		{
			retval = recvn(ptr->sock, (char*)&ptr->data, sizeof(_UserInfo), 0);
			if (retval == SOCKET_ERROR)
			{
				Err_Display("recvn()");
				break;
			}
			else if (retval == 0)
			{
				break;
			}
			printf("[ host (IP : %s) ] Client_Select : %d\n",
				inet_ntoa(ptr->addr.sin_addr), ptr->packet.result);


			int ResultValue = ptr->packet.result;

			bool idflag = false;
			bool pwflag = false;
			bool flag = false;

			switch (ResultValue)
			{
			case INTRO:
			case MAIN:
				switch (ptr->data.select)
				{
				case 1:
					for (iter = userList->begin(); iter != userList->end(); iter++)
					{
						if (!strcmp(ptr->data.ID, iter->ID))
						{
							idflag = true;
							if (!strcmp(ptr->data.PW, iter->PW))
								pwflag = true;
						}
					}

					if (idflag)
					{
						if (pwflag)
						{
							ptr->packet.result = LOGIN;
							strcpy(ptr->packet.msg, MSG_SUCCESS);
						}
						else
						{
							ptr->packet.result = MAIN;
							strcpy(ptr->packet.msg, MSG_PWERROR);
						}
					}
					else
					{
						ptr->packet.result = MAIN;
						strcpy(ptr->packet.msg, MSG_IDERROR);
					}
					break;
				case 2:
					for (iter = userList->begin(); iter != userList->end(); iter++)
					{
						if (!strcmp(ptr->data.ID, iter->ID))
							flag = true;
					}

					if (!flag)
					{
						_UserInfo temp;
						strcpy(temp.ID, ptr->data.ID);
						strcpy(temp.PW, ptr->data.PW);
						userList->push_back(temp);

						ptr->packet.result = MAIN;
						strcpy(ptr->packet.msg, MSG_SUCCESS);
					}
					else
					{
						ptr->packet.result = MAIN;
						strcpy(ptr->packet.msg, MSG_FAIL);
					}
					break;
				case 3:
					break;
				}
				break;
			case LOGIN:
				switch (ptr->data.select)
				{
				case 1:
					ptr->packet.result = MAIN;
					break;
				case 2:
					iter = userList->begin();
					while (iter != userList->end())
					{
						if (!strcmp(ptr->data.ID, iter->ID))
						{
							if (!strcmp(ptr->data.PW, iter->PW))
							{
								iter = userList->erase(iter);
								ptr->packet.result = MAIN;
								strcpy(ptr->packet.msg, MSG_SUCCESS);
							}
							else
								++iter;
						}
						else
							++iter;
					}
					break;
				}
				break;
			}

			retval = send(ptr->sock, (char*)&ptr->packet, sizeof(_Packet), 0);
			if (retval == SOCKET_ERROR)
			{
				Err_Display("send()");
				break;
			}
		}

		RemoveClientInfo(ClientInfo[0]);

	}

	closesocket(listen_sock);
	delete userList;

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



