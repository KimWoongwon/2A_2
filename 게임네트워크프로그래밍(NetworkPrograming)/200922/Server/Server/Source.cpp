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

#define INTRO "------업다운 게임------\n"
#define MSG_PLAYER "1~100 숫자중 하나를 입력하세요. : "
#define	MSG_CORRECT "정답입니다.\n"
#define	MSG_UP "제시한 숫자가 높습니다.\n\n"
#define	MSG_DOWN "제시한 숫자가 낮습니다.\n\n"
#define MSG_INPUTERROR "잘못된 입력 값입니다.\n\n"

#define NODATA -1
#define PLAYER_COUNT 1

enum
{
	INPUTERROR = -1,
	UP = 1,
	DOWN, 
	CORRECT
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
int isCorrectAnswer(int, int);

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

	int Answer = 0;
	
	srand(time(NULL));

	while (1)
	{
		int InputCount = 0;
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
		if (ptr->g_value == NODATA)
		{
			retval = send(ptr->sock, INTRO, strlen(INTRO), 0);
			if (retval == SOCKET_ERROR)
			{
				Err_Display("send()");
				break;
			}
		}

		Answer = rand() % 99 + 1;
		printf("------------ [ Answer : %d ] ------------\n", Answer);

		while (1)
		{
			if (ptr->g_value == NODATA)
			{
				retval = send(ptr->sock, MSG_PLAYER, strlen(MSG_PLAYER), 0);
				if (retval == SOCKET_ERROR)
				{
					Err_Display("send()");
					break;
				}
			}

			if (ptr->g_value == NODATA)
			{
				retval = recvn(ptr->sock, (char*)&ptr->g_value, sizeof(int), 0);
				if (retval == SOCKET_ERROR)
				{
					if(ptr->result.result != CORRECT)
						Err_Display("recvn()");
					break;
				}
				else if (retval == 0)
				{
					break;
				}
				++InputCount;
				printf("[ host (IP : %s) ] Player : %d\n",
					inet_ntoa(ptr->addr.sin_addr), ptr->g_value);
			}


			int ResultValue = isCorrectAnswer(Answer, ptr->g_value);
			if (ptr->g_value > 100 || ptr->g_value < 1)
				ResultValue = INPUTERROR;

			const char* msgptr;

			switch (ResultValue)
			{
			case INPUTERROR:
				ptr->result.result = INPUTERROR;
				strcpy(ptr->result.msg, MSG_INPUTERROR);
				msgptr = MSG_INPUTERROR;
				break;
			case UP:
				ptr->result.result = UP;
				strcpy(ptr->result.msg, MSG_UP);
				msgptr = MSG_UP;
				break;
			case DOWN:
				ptr->result.result = DOWN;
				strcpy(ptr->result.msg, MSG_DOWN);
				msgptr = MSG_DOWN;
				break;
			case CORRECT:
				ptr->result.result = CORRECT;
				wsprintf(ptr->result.msg, "%s입력한 횟수 : %d \n", MSG_CORRECT, InputCount);
				msgptr = MSG_CORRECT;
				break;
			}
			printf("[ Player01(%s) 의 업다운게임 결과 ] : %s\n",
				inet_ntoa(ptr->addr.sin_addr), msgptr);


			retval = send(ptr->sock, (char*)&(ptr->result), sizeof(_Result), 0);
			if (retval == SOCKET_ERROR)
			{
				Err_Display("send()");
				break;
			}
			ptr->g_value = NODATA;
		}

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

int isCorrectAnswer(int Answer, int val)
{
	if (Answer == val)
		return CORRECT;
	else if (Answer > val)
		return DOWN;
	else
		return UP;
}

