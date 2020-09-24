#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

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



#define INTRO "------가위바위보 게임------\n"
#define	MSG_WIN "승리하였습니다.\n"
#define	MSG_LOSE "패배하였습니다.\n"
#define	MSG_DRAW "비겼습니다.\n"
#define CONNECTION_LOST "상대방이 나갔습니다.\n"
#define NODATA -1
#define PLAYER_COUNT 2

enum
{
	WIN = 1,
	LOSE,
	DRAW
};

enum
{
	SCISSORS = 0,
	ROCK,
	PAPER
};

struct RSP
{
	int Val;
};

struct _Result
{
	int result;
	char msg[BUFSIZE];
};

struct _ClientInfo
{
	SOCKET sock;
	SOCKADDR_IN addr;
	_Result result;
	int g_value;
};

int recvn(SOCKET sock, char* buf, int len, int flags);
_ClientInfo* AddClientInfo(SOCKET sock, SOCKADDR_IN addr);
void RemoveClientInfo(_ClientInfo*);

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

	SOCKET client_sock, client02_sock;
	SOCKADDR_IN client_addr, client02_addr;
	int addrlen;
	char buf[BUFSIZE + 1];

	bool Connect_Lost = false;
	int index;

	while (1)
	{
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
		
		bool Endflag = false;

		while (1)
		{
			if (!Endflag)
			{
				for (int i = 0; i < Count; i++)
				{
					_ClientInfo* ptr = ClientInfo[i];
					if (ptr->g_value == NODATA)
					{
						retval = send(ptr->sock, INTRO, strlen(INTRO), 0);
						if (retval == SOCKET_ERROR)
						{
							Err_Display("send()");
						}
					}
				}
			}

			for (int i = 0; i < Count; i++)
			{
				_ClientInfo* ptr = ClientInfo[i];
				if (ptr->g_value == NODATA)
				{
					retval = recvn(ptr->sock, (char*)&ptr->g_value, sizeof(int), 0);
					if (retval == SOCKET_ERROR)
					{
						Err_Display("recvn()");
						RemoveClientInfo(ptr);
						i--;
					}
					else if (retval == 0)
					{
						RemoveClientInfo(ptr);
						i--;
					}
					
					printf("[ host_%02d(IP : %s) ] Player : %d\n", i + 1,
						inet_ntoa(ptr->addr.sin_addr), ptr->g_value);
				}
			}

			if (Count < PLAYER_COUNT)
			{
				break;
			}

			if (Endflag)
			{
				break;
			}
			

			int ResultValue = abs(ClientInfo[0]->g_value - (ClientInfo[1]->g_value + 3));

			if (ResultValue == 3)
			{
				ClientInfo[0]->result.result = DRAW;
				strcpy(ClientInfo[0]->result.msg, MSG_DRAW);
				ClientInfo[1]->result.result = DRAW;
				strcpy(ClientInfo[1]->result.msg, MSG_DRAW);
				printf("[ Player01(%s) vs Player02(%s) 가위바위보 결과 ] : %s\n\n",
					inet_ntoa(ClientInfo[0]->addr.sin_addr), inet_ntoa(ClientInfo[1]->addr.sin_addr), "Draw..");
			}
			else if (ResultValue == 4 || ResultValue == 1)
			{
				Endflag = true;
				ClientInfo[0]->result.result = LOSE;
				strcpy(ClientInfo[0]->result.msg, MSG_LOSE);
				ClientInfo[1]->result.result = WIN;
				strcpy(ClientInfo[1]->result.msg, MSG_WIN);
				printf("[ Player01(%s) vs Player02(%s) 가위바위보 결과 ] : %s\n\n",
					inet_ntoa(ClientInfo[0]->addr.sin_addr), inet_ntoa(ClientInfo[1]->addr.sin_addr), "Player02 Win");
			}
			else if (ResultValue == 5 || ResultValue == 2)
			{
				Endflag = true;
				ClientInfo[0]->result.result = WIN;
				strcpy(ClientInfo[0]->result.msg, MSG_WIN);
				ClientInfo[1]->result.result = LOSE;
				strcpy(ClientInfo[1]->result.msg, MSG_LOSE);
				printf("[ Player01(%s) vs Player02(%s) 가위바위보 결과 ] : %s\n\n",
					inet_ntoa(ClientInfo[0]->addr.sin_addr), inet_ntoa(ClientInfo[1]->addr.sin_addr), "Player01 Win");
			}

			for (int i = 0; i < PLAYER_COUNT; i++)
			{
				_ClientInfo* ptr = ClientInfo[i];
				retval = send(ptr->sock, (char*)&(ptr->result), sizeof(_Result), 0);
				if (retval == SOCKET_ERROR)
				{
					Err_Display("send()");
				}
				ClientInfo[i]->g_value = NODATA;
			}

			
		}

		RemoveClientInfo(ClientInfo[1]);
		RemoveClientInfo(ClientInfo[0]);

	}

	closesocket(listen_sock);

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
	ptr->g_value = NODATA;
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

