#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <ws2tcpip.h> 
#include <time.h>

#define SERVERPORT 9000
#define BUFSIZE    4096
#define NUMBER_COUNT 3

#define MAX_COUNT 99
#define MIN_COUNT 0

#define INTRO_MSG "숫자야구게임을 시작합니다~ 1~9까지의 수중 중복되지 않는 3개의 숫자를 입력하세요.\n"
#define DATA_DUPLICATE_MSG "입력된 숫자에 중복이 있습니다.다시 입력하세요.\n"
#define DATA_RANGE_ERROR_MSG "입력된 숫자는 숫자야구 게임의 숫자 범위가 아닙니다. 다시 입력하세요.\n"
#define GAME_CONTINUE_MSG "게임을 계속하시겠습니까? "

enum CLIENT_STATE
{
	INIT_STATE = 1,
	INTRO_STATE,
	GAME_PLAY_STATE,
	GAME_OVER_STATE,
	DISCONNECTED_STATE
};

enum PROTOCOL
{
	INTRO=1, 
	ANSWER_NUM, 
	ANSWER_RESULT,
	ANSWER_ERROR,
	CORRECT_ANSWER_RESULT,	
	GAME_PROCESS,
	GAME_RESULT
};

enum ERROR_CODE
{
	DATA_DUPLICATE=1,
	DATA_RANGE_ERROR	
};

enum GAME_PROCESS
{
	GAME_CONTINUE=1,
	GAME_OVER
};

enum 
{
		STRIKE,
		BALL, 
		OUTC
};

struct _ClientInfo
{
	SOCKET sock;
	SOCKADDR_IN addr;
	CLIENT_STATE state;
	int corr_num[NUMBER_COUNT];
	int game_count;
	int answer_count;
	int min_answer_count;
	int max_answer_count;
	char packetbuf[BUFSIZE];
};

_ClientInfo* ClientInfo[100];
int Count = 0;
// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

_ClientInfo* AddClientInfo(SOCKET sock, SOCKADDR_IN addr)
{
	_ClientInfo* ptr = new _ClientInfo;
	ZeroMemory(ptr, sizeof(_ClientInfo));
	ptr->sock = sock;
	memcpy(&ptr->addr, &addr, sizeof(addr));
	ptr->state = INIT_STATE;
	ptr->max_answer_count = MIN_COUNT;
	ptr->min_answer_count = MAX_COUNT;
	
	ClientInfo[Count++] = ptr;

	printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));

	return ptr;
}

void ReMoveClientInfo(_ClientInfo* ptr)
{	
	printf("\n[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));

	for (int i = 0; i<Count; i++)
	{
		if (ClientInfo[i] == ptr)
		{
			closesocket(ptr->sock);
			delete ptr;
			for (int j = i; j<Count - 1; j++)
			{
				ClientInfo[j] = ClientInfo[j + 1];
			}
			break;
		}
	}

	Count--;
}

int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
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

//INTRO,GAME_RESULT
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

//CORRECT_ANSWER_RESULT, ANSWER_ERROR,ANSWER_RESULT
int PackPacket(char* _buf, PROTOCOL  _protocol, int _data, const char* _str) 
{
	char* ptr = _buf;
	int strsize = strlen(_str);
	int size = 0;

	ptr = ptr + sizeof(size);

	memcpy(ptr, &_protocol, sizeof(_protocol));
	ptr = ptr + sizeof(_protocol);
	size = size + sizeof(_protocol);

	memcpy(ptr, &_data, sizeof(_data));
	ptr = ptr + sizeof(_data);
	size = size + sizeof(_data);

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

//ANSWER_NUM
void UnPackPacket(char* _buf, int* _data, int& _count) 
{
	char* ptr = _buf + sizeof(PROTOCOL);

	memcpy(&_count, ptr, sizeof(_count));
	ptr = ptr + sizeof(_count);

	for (int i = 0; i < _count; i++)
	{
		memcpy(&_data[i], ptr, sizeof(_data[i]));
		ptr = ptr + sizeof(_data[i]);
	}	
}

//GAME_PROCESS
void UnPackPacket(char* _buf, int& _data) 
{
	char* ptr = _buf + sizeof(PROTOCOL);

	memcpy(&_data, ptr, sizeof(_data));
	ptr = ptr + sizeof(_data);
}

void RandCorrectNumber(int* _corr_num, int _count)
{
	int count = 0; 

	while (1)
	{
		int num = rand() % 9 + 1;
		bool flag = false;

		for (int i = 0; i < count; i++)
		{
			if (_corr_num[i] == num)
			{
				flag = true;
				break;
			}
		}
		if (flag)
		{
			continue;
		}
		_corr_num[count++] = num;
		if (count == _count)
		{
			break;
		}
	}

	printf("%d  %d  %d\n", _corr_num[0], _corr_num[1], _corr_num[2]);
}

bool ChecKDuplicate(int* _data, int _count)
{	
	for (int i = 0; i < _count - 1; i++)
	{
		for (int j = i + 1; j < _count; j++)
		{
			if (_data[i] == _data[j])
			{
				return false;
			}
		}		
	}
	return true;
}

bool ChecKDataRange(int* _data, int _count)
{

	for (int i = 0; i < _count; i++)
	{
		if (_data[i] < 1 || _data[i]>9)
		{
			return false;
		}
	}
	
	return true;
}

void ProcessBaseBallGame(int* _corr_num, int* _answer_num, int* _result, int _count)
{
	memset(_result, 0, sizeof(_result)*_count);

	for (int i = 0; i < _count; i++)
	{
		bool flag = false;
		for (int j = 0; j < _count; j++)
		{
			if (_answer_num[i] == _corr_num[j])
			{
				flag = true;
				if (i == j)
				{
					_result[STRIKE]++;
				}
				else
				{
					_result[BALL]++;
				}
			}
		}
		if (!flag)
		{
			_result[OUTC]++;
		}
	}
}

void IntroProcess(_ClientInfo* _ptr)
{
	RandCorrectNumber(_ptr->corr_num, NUMBER_COUNT);

	int size = PackPacket(_ptr->packetbuf, INTRO, INTRO_MSG);

	int retval = send(_ptr->sock, _ptr->packetbuf, size, 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		_ptr->state = DISCONNECTED_STATE;
		return;
	}
	_ptr->game_count++;
	_ptr->state = GAME_PLAY_STATE;
}

void GamePlayProcess(_ClientInfo* _ptr)
{
	if (!PacketRecv(_ptr->sock, _ptr->packetbuf))
	{
		_ptr->state = DISCONNECTED_STATE;
		return;
	}

	PROTOCOL protocol = GetProtocol(_ptr->packetbuf);

	int answer[NUMBER_COUNT];
	int result[3];
	int count;

	switch (protocol)
	{
	case ANSWER_NUM:		

		memset(answer, 0, sizeof(int)*NUMBER_COUNT);
		UnPackPacket(_ptr->packetbuf, answer, count);
		_ptr->answer_count++;

		if (!ChecKDuplicate(answer, NUMBER_COUNT))
		{
			int size = PackPacket(_ptr->packetbuf, ANSWER_ERROR, DATA_DUPLICATE, DATA_DUPLICATE_MSG);
			int retval = send(_ptr->sock, _ptr->packetbuf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				_ptr->state = DISCONNECTED_STATE;
				return;
			}
			break;
		}

		if (!ChecKDataRange(answer, NUMBER_COUNT))
		{
			int size = PackPacket(_ptr->packetbuf, ANSWER_ERROR, DATA_RANGE_ERROR, DATA_RANGE_ERROR_MSG);
			int retval = send(_ptr->sock, _ptr->packetbuf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send()");
				_ptr->state = DISCONNECTED_STATE;
				return;
			}
			break;
		}

		ProcessBaseBallGame(_ptr->corr_num, answer, result, NUMBER_COUNT);

		if (result[STRIKE] == NUMBER_COUNT)
		{
			_ptr->state = GAME_OVER_STATE;
			return;
		}

		char msg[BUFSIZE];
		sprintf(msg, "Strike:%d Ball:%d Out:%d\n", result[STRIKE], result[BALL], result[OUTC]);
		int size = PackPacket(_ptr->packetbuf, ANSWER_RESULT, msg);
		int retval = send(_ptr->sock, _ptr->packetbuf, size, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			_ptr->state = DISCONNECTED_STATE;
			return;
		}
	}		
}

void GameOverProcess(_ClientInfo* _ptr)
{
	char msg[BUFSIZE];
	sprintf(msg, "%d번 만에 정답을 맞추셨습니다.\n", _ptr->answer_count);
	int size = PackPacket(_ptr->packetbuf, CORRECT_ANSWER_RESULT, msg);
	int retval = send(_ptr->sock, _ptr->packetbuf, size, 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		_ptr->state = DISCONNECTED_STATE;
		return;
	}
	if (_ptr->max_answer_count < _ptr->answer_count)
	{
		_ptr->max_answer_count = _ptr->answer_count;
	}

	if (_ptr->min_answer_count > _ptr->answer_count)
	{
		_ptr->min_answer_count = _ptr->answer_count;
	}

	size = PackPacket(_ptr->packetbuf, GAME_PROCESS, GAME_CONTINUE_MSG);
	retval = send(_ptr->sock, _ptr->packetbuf, size, 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		_ptr->state = DISCONNECTED_STATE;
		return;
	}

	if (!PacketRecv(_ptr->sock, _ptr->packetbuf))
	{
		_ptr->state = DISCONNECTED_STATE;
		return;
	}

	PROTOCOL protocol = GetProtocol(_ptr->packetbuf);

	switch (protocol)
	{
	case GAME_PROCESS:	
		int result;
		UnPackPacket(_ptr->packetbuf, result);

		switch (result)
		{
		case GAME_CONTINUE:
			_ptr->answer_count = 0;
			_ptr->state = INTRO_STATE;
			break;
		case GAME_OVER:
			char msg[BUFSIZE];

			sprintf(msg, "전체게임횟수:%d, 최저 성적:%d  최고 성적:%d\n", _ptr->game_count, _ptr->max_answer_count, _ptr->min_answer_count);
			size = PackPacket(_ptr->packetbuf, GAME_RESULT, msg);
			retval = send(_ptr->sock, _ptr->packetbuf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send()");				
			}

			if (!PacketRecv(_ptr->sock, _ptr->packetbuf))
			{
				_ptr->state = DISCONNECTED_STATE;				
			}

			break;
		}
		break;		
	}
}

int main(int argc, char *argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if(retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	
	char buf[BUFSIZE+1];

	srand(time(0));

	while (1)
	{
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}

		_ClientInfo* ClientPtr = AddClientInfo(client_sock, clientaddr);				

		while (1)
		{
			bool endflag = false;

			switch (ClientPtr->state)
			{
			case INIT_STATE:
				ClientPtr->state = INTRO_STATE;
				break;
			case INTRO_STATE:
				IntroProcess(ClientPtr);
				break;
			case GAME_PLAY_STATE:
				GamePlayProcess(ClientPtr);
				break;
			case GAME_OVER_STATE:
				GameOverProcess(ClientPtr);
				break;
			case DISCONNECTED_STATE:
				ReMoveClientInfo(ClientPtr);
				endflag = true;
				break;
			}

			if (endflag)
			{
				break;
			}			
		}	

	}	
	// closesocket()
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}