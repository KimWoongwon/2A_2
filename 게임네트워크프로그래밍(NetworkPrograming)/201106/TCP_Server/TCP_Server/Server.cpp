#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define SERVERPORT 9000
#define BUFSIZE    512
#define PLAYER_COUNT 100
#define JOIN_USER_COUNT 100

#define MSG_INTRO "31게입입니다. 1부터 시작합니다.\n"
#define MSG_WIN "당신이 이겼습니다! 짝! 짝! 짝!\n"
#define MSG_LOSE "당신이 졌습니다... 다음 기회에..\n"
#define MSG_INPUT_ERROR "잘못된 입력값입니다. 1~3사이의 숫자를 입력해주세요."

#pragma region Error_Function
// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
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


enum PROTOCOL { INTRO, GAME_RESULT };
enum RES_GAME { WIN, LOSE, CONTINUE, INPUT_ERROR };

struct _ClientInfo
{
	SOCKET sock;
	SOCKADDR_IN addr;

	char recv_buf[BUFSIZE + 1];
	char send_buf[BUFSIZE + 1];

};

_ClientInfo* ClientInfo[PLAYER_COUNT];
int Count = 0;

CRITICAL_SECTION cs;

_ClientInfo* AddClientInfo(SOCKET sock, SOCKADDR_IN addr);
void ReMoveClientInfo(_ClientInfo* ptr);

bool PacketRecv(SOCKET _sock, char* _buf);
PROTOCOL GetProtocol(char* _buf);

int Packing(char* _buf, PROTOCOL _protocol, int _result, const char* _player_msg, const char* _server_msg);	// 게임중 보내기
int Packing(char* _buf, PROTOCOL _protocol, const char* _msg);		// 인트로 보내기용
void UnPacking(const char* _buf, int* _input);

int Game_Process(int& _gameCount, int player, char* _pbuf, char* _sbuf);

int main(int argc, char* argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;

	InitializeCriticalSection(&cs);

	while (1) 
	{
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		_ClientInfo* ptr = AddClientInfo(client_sock, clientaddr);

		int size = Packing(ptr->send_buf, INTRO, MSG_INTRO);
		int retval = send(ptr->sock, ptr->send_buf, size, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}

		srand(time(NULL));

		int GameCount = 0;
		char player_msg_buf[BUFSIZE];
		char server_msg_buf[BUFSIZE];
		while (1)
		{
			if (!PacketRecv(ptr->sock, ptr->recv_buf))
				break;

			PROTOCOL protocol = GetProtocol(ptr->recv_buf);

			int Player, result;
			switch (protocol)
			{
			case GAME_RESULT:
				UnPacking(ptr->recv_buf, &Player);

				result = Game_Process(GameCount, Player, player_msg_buf, server_msg_buf);

				switch (result)
				{
				case INPUT_ERROR:
					size = Packing(ptr->send_buf, GAME_RESULT, INPUT_ERROR, MSG_INPUT_ERROR, "");
					break;
				case WIN:
					size = Packing(ptr->send_buf, GAME_RESULT, WIN, MSG_WIN, "");
					break;
				case LOSE:
					size = Packing(ptr->send_buf, GAME_RESULT, LOSE, MSG_LOSE, "");
					break;
				case CONTINUE:
					size = Packing(ptr->send_buf, GAME_RESULT, CONTINUE, player_msg_buf, server_msg_buf);
					break;
				}
				break;
			}

			retval = send(ptr->sock, ptr->send_buf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send()");
				break;
			}

		}
		// 스레드 생성
		
	}

	DeleteCriticalSection(&cs);
	// closesocket()
	closesocket(listen_sock);
	// 윈속 종료
	WSACleanup();
	return 0;
}

#pragma region ClientInfo_Function
_ClientInfo* AddClientInfo(SOCKET sock, SOCKADDR_IN addr)
{
	printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

	_ClientInfo* ptr = new _ClientInfo();
	ZeroMemory(ptr, sizeof(_ClientInfo));
	ptr->sock = sock;
	memcpy(&ptr->addr, &addr, sizeof(addr));

	EnterCriticalSection(&cs);
	ClientInfo[Count++] = ptr;
	LeaveCriticalSection(&cs);
	return ptr;
}
void ReMoveClientInfo(_ClientInfo* ptr)
{
	printf("\n[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));

	EnterCriticalSection(&cs);
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
	LeaveCriticalSection(&cs);
}
#pragma endregion

#pragma region Packet_Control_Function

bool PacketRecv(SOCKET _sock, char* _buf)
{
	int size;

	int retval = recvn(_sock, (char*)&size, sizeof(size), 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("gvalue recv error()");
		return false;
	}
	else if (retval == 0)
	{
		return false;
	}

	retval = recvn(_sock, _buf, size, 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("gvalue recv error()");
		return false;

	}
	else if (retval == 0)
	{
		return false;
	}

	return true;
}

PROTOCOL GetProtocol(char* _buf)
{
	PROTOCOL protocol;
	memcpy(&protocol, _buf, sizeof(PROTOCOL));

	return protocol;
}

int Packing(char* _buf, PROTOCOL _protocol, int _result, const char* _player_msg, const char* _server_msg)
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

	int msgsize = strlen(_player_msg);
	memcpy(ptr, &msgsize, sizeof(int));
	size += sizeof(int);
	ptr += sizeof(int);

	memcpy(ptr, _player_msg, msgsize);
	size += msgsize;
	ptr += msgsize;

	msgsize = strlen(_server_msg);
	memcpy(ptr, &msgsize, sizeof(int));
	size += sizeof(int);
	ptr += sizeof(int);

	memcpy(ptr, _server_msg, msgsize);
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
void UnPacking(const char* _buf, int* _input)
{
	const char* ptr = _buf + sizeof(PROTOCOL);

	memcpy(_input, ptr, sizeof(int));
	ptr += sizeof(int);

}
#pragma endregion

int Game_Process(int& _gameCount, int player, char* _pbuf, char* _sbuf)
{
	if (player > 3 || player <= 0)
		return INPUT_ERROR;

	ZeroMemory(_pbuf, sizeof(_pbuf));
	for (int i = 0; i < player; i++)
	{
		char temp[10];
		++_gameCount;
		sprintf(temp, "%d\t", _gameCount);
		strcat(_pbuf, temp);
		if (_gameCount >= 31)
			break;
	}
	strcat(_pbuf, "\n");

	if (_gameCount >= 31)
		return LOSE;

	int server;
	server = rand() % 3 + 1;

	ZeroMemory(_sbuf, sizeof(_sbuf));
	for (int i = 0; i < server; i++)
	{
		char temp[10];
		++_gameCount;
		sprintf(temp, "%d\t", _gameCount);
		strcat(_sbuf, temp);
		if (_gameCount >= 31)
			break;
	}
	strcat(_sbuf, "\n");

	if (_gameCount >= 31)
		return WIN;
	else
		return CONTINUE;

}