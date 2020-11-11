#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <ws2tcpip.h> 
#include <time.h>
#include <tchar.h>

#define SERVERPORT 9000
#define BUFSIZE    4096
#define GAME_OVER_NUMBER 31

#define WAITING_MSG "다른 플레이어를 기다리는 중입니다. (%d/3)\n"
#define INTRO_MSG "31숫자게임입니다.1부터 시작합니다.\n"
#define CLIENT_TURN_MSG "당신차례입니다.한번에 3개까지 선택할 수 있습니다 선택하세요:"
#define OTHER_TURN_MSG "Player %d 차례입니다.\nPlayer %d가 선택한 숫자입니다.\n"
#define COUNT_ERROR_MSG "잘못 선택했습니다. 1~3개까지 선택가능합니다.\n"
#define WIN_MSG "당신이 이겼습니다.짝!짝!짝\n"
#define LOSE_MSG "당신이 졌습니다.ㅠ.ㅠ\n"

enum STATE
{
	WAIT_STATE = 1,
	INIT_STATE,
	CLIENT_TURN_STATE,
	OTHER_TURN_STATE,
	GAME_OVER_STATE,
	DISCONNECTED_STATE
};

enum PROTOCOL
{
	WAIT = 1,
	INTRO,
	SELECT_COUNT,	
	COUNT_VALUE,
	OTHER_TURN,
	CLIENT_TURN,
	GAME_ERROR,
	GAME_RESULT
};

enum RESULT
{
	NODATA = -1,
	WIN = 1,
	LOSE
};


enum ERROR_CODE
{
	SELECT_COUNT_ERROR = 1
};


struct _ClientInfo
{
	SOCKET sock;
	SOCKADDR_IN addr;
	STATE state;
	
	RESULT game_result;
	int group_number;
	int player_number;

	char send_buf[BUFSIZE];
	char recv_buf[BUFSIZE];
};

_ClientInfo* ClientInfo[100];
int Count = 0;

struct _GroupInfo
{
	_ClientInfo* players[3];
	HANDLE totalEvent;
	HANDLE mThread[3];
	HANDLE mEvent[3];

	int game_value;
};

_GroupInfo* GroupInfo[30];
int G_Count = 0;



// 소켓 함수 오류 출력 후 종료
void err_quit(char* msg)
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
void err_display(char* msg)
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

_ClientInfo* AddClientInfo(SOCKET sock, SOCKADDR_IN addr)
{
	_ClientInfo* ptr = new _ClientInfo;
	ZeroMemory(ptr, sizeof(_ClientInfo));
	ptr->sock = sock;
	memcpy(&ptr->addr, &addr, sizeof(addr));
	ptr->state = WAIT_STATE;
	ptr->game_result = NODATA;
	ptr->group_number = G_Count;
	ClientInfo[Count++] = ptr;

	printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));

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

int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}
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

int PackPacket(char* _buf, PROTOCOL  _protocol, const char* _str)
{
	int size = 0;
	char* ptr = _buf;
	int strsize = strlen(_str);


	ptr = ptr + sizeof(size);

	memcpy(ptr, &_protocol, sizeof(_protocol));
	ptr = ptr + sizeof(_protocol);
	size = size + sizeof(_protocol);


	memcpy(ptr, &strsize, sizeof(strsize));
	ptr = ptr + sizeof(strsize);
	size = size + sizeof(strsize);

	memcpy(ptr, _str, strsize);
	ptr = ptr + strsize;
	size = size + strsize;

	ptr = _buf;
	memcpy(ptr, &size, sizeof(size));

	size = size + sizeof(size);

	return size;
}
int PackPacket(char* _buf, PROTOCOL  _protocol, int _result, const char* _str)
{
	int size = 0;
	char* ptr = _buf;
	int strsize = strlen(_str);


	ptr = ptr + sizeof(size);

	memcpy(ptr, &_protocol, sizeof(_protocol));
	ptr = ptr + sizeof(_protocol);
	size = size + sizeof(_protocol);

	memcpy(ptr, &_result, sizeof(_result));
	ptr = ptr + sizeof(_result);
	size = size + sizeof(_result);


	memcpy(ptr, &strsize, sizeof(strsize));
	ptr = ptr + sizeof(strsize);
	size = size + sizeof(strsize);

	memcpy(ptr, _str, strsize);
	ptr = ptr + strsize;
	size = size + strsize;

	ptr = _buf;
	memcpy(ptr, &size, sizeof(size));

	size = size + sizeof(size);

	return size;
}
int PackPacket(char* _buf, PROTOCOL _protocol, int _count, int* _data)
{
	char* ptr = _buf;
	int size = 0;

	ptr = ptr + sizeof(size);

	memcpy(ptr, &_protocol, sizeof(_protocol));
	ptr = ptr + sizeof(_protocol);
	size = size + sizeof(_protocol);

	memcpy(ptr, &_count, sizeof(_count));
	ptr = ptr + sizeof(_count);
	size = size + sizeof(_count);

	for (int i = 0; i < _count; i++)
	{
		memcpy(ptr, &_data[i], sizeof(_data[i]));
		ptr = ptr + sizeof(_data[i]);
		size = size + sizeof(_data[i]);
	}

	ptr = _buf;
	memcpy(ptr, &size, sizeof(size));

	size = size + sizeof(size);

	return size;
}
void UnPackPacket(char* _buf, int& _data)
{
	char* ptr = _buf + sizeof(PROTOCOL);

	memcpy(&_data, ptr, sizeof(_data));
	ptr = ptr + sizeof(_data);
}
void UnPackPacket(char* _buf, int* _data, int& _count)
{
	char* ptr = _buf + sizeof(PROTOCOL) + sizeof(int);

	memcpy(&_count, ptr, sizeof(_count));
	ptr = ptr + sizeof(_count);

	for (int i = 0; i < _count; i++)
	{
		memcpy(&_data[i], ptr, sizeof(_data[i]));
		ptr = ptr + sizeof(_data[i]);
	}
}

void ClientTurnProcess(_ClientInfo* _ptr)
{
	int retval = WaitForSingleObject(GroupInfo[_ptr->group_number]->mEvent[_ptr->player_number], INFINITE);
	if (retval != WAIT_OBJECT_0)
		return;

	int size = PackPacket(_ptr->send_buf, CLIENT_TURN, CLIENT_TURN_MSG);
	retval = send(_ptr->sock, _ptr->send_buf, size, 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		_ptr->state = DISCONNECTED_STATE;
		return;
	}

	if (!PacketRecv(_ptr->sock, _ptr->recv_buf))
	{
		_ptr->state = DISCONNECTED_STATE;
		return;
	}

	PROTOCOL protocol = GetProtocol(_ptr->recv_buf);
	switch (protocol)
	{
	case SELECT_COUNT:
		int count;
		UnPackPacket(_ptr->recv_buf, count);
		if (count < 1 || count >3)
		{
			int size = PackPacket(_ptr->send_buf, GAME_ERROR, SELECT_COUNT_ERROR, COUNT_ERROR_MSG);
			retval = send(_ptr->sock, _ptr->send_buf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send()");
				_ptr->state = DISCONNECTED_STATE;
				return;
			}
			return;
		}

		int  count_value[3] = { 0 };		

		for (int i = 0; i < count; i++)
		{
			GroupInfo[_ptr->group_number]->game_value++;

			if (GroupInfo[_ptr->group_number]->game_value == GAME_OVER_NUMBER)
			{
				_ptr->game_result = LOSE;
				_ptr->state = GAME_OVER_STATE;

				SetEvent(GroupInfo[_ptr->group_number]->mEvent[_ptr->player_number]);
				return;
			}

			count_value[i] = GroupInfo[_ptr->group_number]->game_value;
		}

		int size = PackPacket(_ptr->send_buf, COUNT_VALUE, count, count_value);
		SetEvent(GroupInfo[_ptr->group_number]->mEvent[(_ptr->player_number + 1) % 3]);

		retval = send(_ptr->sock, _ptr->send_buf, size, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			_ptr->state = DISCONNECTED_STATE;
			return;
		}
	}
	_ptr->state = OTHER_TURN_STATE;
	
}

void OtherTurnProcess(_ClientInfo* _ptr)
{
	int  count_value[3] = { 0 };
	int other_count = 0;

	int tempCount = _ptr->player_number;
	char buf[BUFSIZE];
	if (--tempCount < 0)
		tempCount = 2;
		
	int retval = WaitForSingleObject(GroupInfo[_ptr->group_number]->mEvent[_ptr->player_number], INFINITE);
	if (retval != WAIT_OBJECT_0)
		return;
	if (_ptr->state == GAME_OVER_STATE)
		return;

	UnPackPacket(GroupInfo[_ptr->group_number]->players[tempCount]->send_buf, count_value, other_count);

	char msg[BUFSIZE] = { 0 };
	sprintf(msg, OTHER_TURN_MSG, tempCount + 1, tempCount + 1);

	for (int i = 0; i < other_count; i++)
	{
		char temp[50];
		sprintf(temp, "%d\t", count_value[i]);
		strcat(msg, temp);
	}

	strcat(msg, "\n");

	ZeroMemory(_ptr->send_buf, sizeof(_ptr->send_buf));
	int size = PackPacket(_ptr->send_buf, OTHER_TURN, msg);
	retval = send(_ptr->sock, _ptr->send_buf, size, 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		_ptr->state = DISCONNECTED_STATE;
		return;
	}

	if (GroupInfo[_ptr->group_number]->game_value + 1 == GAME_OVER_NUMBER)
	{
		_ptr->game_result = LOSE;
		_ptr->state = GAME_OVER_STATE;
		SetEvent(GroupInfo[_ptr->group_number]->mEvent[_ptr->player_number]);
		return;
	}

	_ptr->state = CLIENT_TURN_STATE;
	SetEvent(GroupInfo[_ptr->group_number]->mEvent[_ptr->player_number]);
}

void GameOverProcess(_ClientInfo* _ptr)
{
	int size;
	int retval;

	switch (_ptr->game_result)
	{
	case WIN:

		size = PackPacket(_ptr->send_buf, GAME_RESULT, WIN, WIN_MSG);
		retval = send(_ptr->sock, _ptr->send_buf, size, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			_ptr->state = DISCONNECTED_STATE;
			return;
		}

		break;
	case LOSE:
		size = PackPacket(_ptr->send_buf, GAME_RESULT, LOSE, LOSE_MSG);
		retval = send(_ptr->sock, _ptr->send_buf, size, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			_ptr->state = DISCONNECTED_STATE;
			return;
		}
		break;
	}

	if (!PacketRecv(_ptr->sock, _ptr->recv_buf))
	{
		_ptr->state = DISCONNECTED_STATE;
		return;
	}

}
void InitProcess(_ClientInfo* _ptr)
{
	int retval = WaitForSingleObject(GroupInfo[_ptr->group_number]->totalEvent, INFINITE);
	if (retval != WAIT_OBJECT_0)
		return;

	int size = PackPacket(_ptr->send_buf, INTRO, INTRO_MSG);
	retval = send(_ptr->sock, _ptr->send_buf, size, 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		return;
	}

	if (_ptr->player_number == 0)
		_ptr->state = CLIENT_TURN_STATE;
	else
		_ptr->state = OTHER_TURN_STATE;
}

DWORD WINAPI Client_Thread(LPVOID arg)
{
	_ClientInfo* ptr = (_ClientInfo*)arg;
	bool endflag = false;
	while (1)
	{
		/*int retval = WaitForSingleObject(GroupInfo[ptr->group_number]->mEvent[ptr->player_number], INFINITE);
		if (retval != WAIT_OBJECT_0)
			return 1;*/

		switch (ptr->state)
		{
		case INIT_STATE:
			InitProcess(ptr);
			break;
		case CLIENT_TURN_STATE:
			ClientTurnProcess(ptr);
			break;
		case OTHER_TURN_STATE:
			OtherTurnProcess(ptr);
			break;
		case GAME_OVER_STATE:
			GameOverProcess(ptr);
			break;
		case DISCONNECTED_STATE:
			ReMoveClientInfo(ptr);
			endflag = true;
			break;
		}

		//SetEvent(GroupInfo[ptr->group_number]->mEvent[(ptr->player_number + 1) % 3]);

		if (endflag)
		{
			break;
		}
	}

	return 0;
}

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

	srand(time(0));

	//HANDLE mThread[3];
	
	while (1)
	{
		GroupInfo[G_Count] = new _GroupInfo;
		_GroupInfo* _gptr = GroupInfo[G_Count];
		_gptr->game_value = 0;
		_gptr->totalEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (_gptr->totalEvent == NULL)
			break;
		for (int i = 0; i < 3; i++)
		{
			addrlen = sizeof(clientaddr);
			client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
			if (client_sock == INVALID_SOCKET)
			{
				err_display("accept()");
				break;
			}

			_ClientInfo* ClientPtr = AddClientInfo(client_sock, clientaddr);
			_gptr->players[i] = ClientPtr;
			if(i == 0)
				_gptr->mEvent[i] = CreateEvent(NULL, FALSE, TRUE, NULL);
			else
				_gptr->mEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);

			char msg[BUFSIZE];
			sprintf(msg, WAITING_MSG, i + 1);
			int size = PackPacket(ClientPtr->send_buf, WAIT, msg);
			retval = send(ClientPtr->sock, ClientPtr->send_buf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send()");
				break;
			}
			ClientPtr->player_number = i;
			ClientPtr->state = INIT_STATE;
			_gptr->mThread[i] = CreateThread(NULL, 0, Client_Thread, (LPVOID)ClientPtr, 0, NULL);
		}
		SetEvent(_gptr->totalEvent);
		++G_Count;
	}

	for (int i = G_Count - 1; i >= 0; i--)
	{
		delete GroupInfo[i];
	}

	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}