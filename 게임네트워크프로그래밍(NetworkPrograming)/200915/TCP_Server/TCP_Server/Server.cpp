#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

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

#define INTRO "ID, PW를 입력하세요.\n"
#define	ID_ERROR "없는 ID입니다.\n"
#define	PW_ERROR "잘못된 PW입니다.\n"
#define	LOGIN_SUCCESS "로그인에 성공했습니다.\n"

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
}UserInfo[3] = { {"woong9711", "a7612590"}, {"aaa", "111"}, {"bbb", "222"} };

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
	if(listen_sock == SOCKET_ERROR)
		Err_Quit("bind()");

	retval = listen(listen_sock, SOMAXCONN);
	if (listen_sock == SOCKET_ERROR)
		Err_Quit("listen()");

	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	while (1)
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			Err_Display("accept()");
			break;
		}

		printf("클라이언트 접속: IP = %s port = %d \n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		retval = send(client_sock, INTRO, strlen(INTRO), 0);
		if (retval == SOCKET_ERROR)
		{
			Err_Display("send()");
			continue;
		}

		_UserInfo info;
		ZeroMemory(&info, sizeof(info));

		while (1)
		{
			retval = recvn(client_sock, (char*)&info, sizeof(info), 0);
			if (retval == SOCKET_ERROR)
			{
				Err_Display("recvn()");
				break;
			}
			else if (retval == 0)
			{
				break;
			}

			printf("[host(%s) 에서 로그인 시도] ID : %s	PW: %s\n", 
				inet_ntoa(clientaddr.sin_addr), info.id, info.pw);

			bool idflag = false, pwflag = false;
			_Result login_result;
			
			for (int i = 0; i < 3; i++)
			{
				if (!strcmp(UserInfo[i].id, info.id))
				{
					idflag = true;
					if (!strcmp(UserInfo[i].pw, info.pw))
						pwflag = true;
				}
			}

			if (idflag == false)
			{
				login_result.result = ERROR_ID;
				strcpy(login_result.msg, ID_ERROR);
			}
			else
			{
				if (pwflag == false)
				{
					login_result.result = ERROR_PW;
					strcpy(login_result.msg, PW_ERROR);
				}
				else
				{
					login_result.result = SUCCESS;
					strcpy(login_result.msg, LOGIN_SUCCESS);
				}
			}
			
			printf("[host(%s) 에서 로그인 결과] : %s\n",
				inet_ntoa(clientaddr.sin_addr), login_result.msg);

			retval = send(client_sock, (char*)&login_result, sizeof(login_result), 0);
			if (retval == SOCKET_ERROR)
			{
				Err_Display("send()");
				break;
			}

		}

		closesocket(client_sock);
		printf("클라이언트 종료: IP = %s port = %d \n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		
	}

	closesocket(listen_sock);

	WSACleanup();
	return 0;
}