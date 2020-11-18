#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFSIZE 4096
#define FILENAMESIZE 256
#define MAXCONCLIENT 100

#define INTRO_MSG "전송할 파일명을 입력하세요"
#define FILE_DUPLICATE_MSG "전송하고자 하는 파일은 이미 서버에 존재하는 파일입니다."
#define FILE_WAITING_MSG "다른 클라이언트를 기다리는 중입니다..\n이미 전송중인 파일이 있습니다.\n"

enum PROTOCOL
{
	INTRO = 1,
	FILE_INFO,
	FILE_TRANS_DENY,
	FILE_TRANS_START_POINT,
	FILE_TRANS_DATA
};

enum STATE
{
	INITE_STATE = 1,
	FILE_TRANS_READY_STATE,
	FILE_TRANS_WAIT_STATE,
	FILE_TRANS_STATE,
	FILE_TRANS_COMPLETE_STATE,
	DISCONNECTED_STATE
};

enum DENY_CODE
{
	FILEEXIST = -1,
	FILEWAIT = -2,
};

struct _File_info
{
	char filename[FILENAMESIZE];
	int  filesize;
	int  nowsize;
};

struct _ClientInfo
{
	SOCKET sock;
	SOCKADDR_IN addr;
	STATE	state;
	_File_info  file_info;

	HANDLE Turn_Event;
	HANDLE Next_Event;

	char recv_buf[BUFSIZE];
	char send_buf[BUFSIZE];
};

void err_quit(const char*);
void err_display(const char*);

int recvn(SOCKET, char*, int, int);

_ClientInfo* AddClientInfo(SOCKET sock, SOCKADDR_IN addr);
void ReMoveClientInfo(_ClientInfo*);

bool SearchFile(const char*);
bool PacketRecv(SOCKET, char*);
PROTOCOL GetProtocol(const char*);
int PackPacket(char*, PROTOCOL, const char*);
int PackPacket(char*, PROTOCOL, int, const char*);
int PackPacket(char*, PROTOCOL, int);

void UnPackPacket(const char* _buf, char* _str1, int& _data1);
void UnPackPacket(const char*, int&, char*);

DWORD WINAPI Client_Thread(LPVOID);

void InitProcess(_ClientInfo*);
void WaitProcess(_ClientInfo*);
void ReadyProcess(_ClientInfo*);
void FileTransProcess(_ClientInfo*);
void FileTransCompleteProcess(_ClientInfo*);

_ClientInfo* ClientInfo[MAXCONCLIENT];
int count;

CRITICAL_SECTION cs;

int main(int argc, char* argv[])
{
	int retval;

	InitializeCriticalSection(&cs);

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수

	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;


	char buf[BUFSIZE];
	FILE* fp = nullptr;

	while (1)
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			continue;
		}

		_ClientInfo* ClientPtr = AddClientInfo(client_sock, clientaddr);

		HANDLE hThread = CreateThread(nullptr, 0, Client_Thread, ClientPtr, 0, nullptr);
		if (hThread == nullptr)
		{
			ReMoveClientInfo(ClientPtr);
			continue;
		}

		CloseHandle(hThread);

		//inner while end


	}//outer while end

	DeleteCriticalSection(&cs);

	 // closesocket()
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}

// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

// 소켓 함수 오류 출력
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}


_ClientInfo* AddClientInfo(SOCKET sock, SOCKADDR_IN addr)
{
	_ClientInfo* ptr = new _ClientInfo;
	ZeroMemory(ptr, sizeof(_ClientInfo));
	ptr->sock = sock;
	memcpy(&ptr->addr, &addr, sizeof(addr));
	ptr->state = INITE_STATE;

	if(count == 0)
		ptr->Turn_Event = CreateEvent(NULL, false, true, NULL);
	else
	{
		ptr->Turn_Event = CreateEvent(NULL, false, false, NULL);
		//ClientInfo[count - 1]->Next_Event = ptr->Turn_Event;
	}

	ptr->Next_Event = nullptr;

	ClientInfo[count++] = ptr;
	printf("\nFileSender 접속: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));

	return ptr;
}

void ReMoveClientInfo(_ClientInfo* ptr)
{

	printf("FileSender 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));

	for (int i = 0; i < count; i++)
	{
		if (ClientInfo[i] == ptr)
		{
			closesocket(ptr->sock);
			delete ptr;
			for (int j = i; j < count - 1; j++)
			{
				ClientInfo[j] = ClientInfo[j + 1];
			}
			break;
		}
	}

	count--;
}

bool SearchFile(const char* filename)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFindFile = FindFirstFile(filename, &FindFileData);
	if (hFindFile == INVALID_HANDLE_VALUE)
		return false;
	else {
		FindClose(hFindFile);
		return true;
	}
}
#pragma region 사용자 정의 데이터 수신 함수
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
PROTOCOL GetProtocol(const char* _buf)
{
	PROTOCOL protocol;
	memcpy(&protocol, _buf, sizeof(PROTOCOL));
	return protocol;
}

int PackPacket(char* _buf, PROTOCOL  _protocol, const char* _str) //인트로 
{
	char* ptr = _buf;
	int strsize = strlen(_str);
	int size = 0;

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
int PackPacket(char* _buf, PROTOCOL  _protocol, int _data, const char* _str) //파일전송 거부 정보
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
int PackPacket(char* _buf, PROTOCOL _protocol, int _data)//파일 전송 위치 정보
{
	char* ptr = _buf;
	int size = 0;

	ptr = ptr + sizeof(size);

	memcpy(ptr, &_protocol, sizeof(_protocol));
	ptr = ptr + sizeof(_protocol);
	size = size + sizeof(_protocol);

	memcpy(ptr, &_data, sizeof(_data));
	ptr = ptr + sizeof(_data);
	size = size + sizeof(_data);

	ptr = _buf;
	memcpy(ptr, &size, sizeof(size));

	size = size + sizeof(size);
	return size;
}

void UnPackPacket(const char* _buf, char* _str1, int& _data1)//파일이름 & 파일 크기
{
	const char* ptr = _buf + sizeof(PROTOCOL);
	int strsize;

	memcpy(&strsize, ptr, sizeof(strsize));
	ptr = ptr + sizeof(strsize);

	memcpy(_str1, ptr, strsize);
	ptr = ptr + strsize;

	memcpy(&_data1, ptr, sizeof(_data1));
	ptr = ptr + sizeof(_data1);
}
void UnPackPacket(const char* _buf, int& _size, char* _targetbuf) //파일 전송 데이터
{
	const char* ptr = _buf + sizeof(PROTOCOL);

	memcpy(&_size, ptr, sizeof(_size));
	ptr = ptr + sizeof(_size);

	memcpy(_targetbuf, ptr, _size);
}
#pragma endregion



DWORD WINAPI Client_Thread(LPVOID arg)
{
	_ClientInfo* _ptr = (_ClientInfo*)arg;
	bool endflag = false;

	while (1)
	{

		switch (_ptr->state)
		{
		case INITE_STATE:
			InitProcess(_ptr);
			break;
		case FILE_TRANS_WAIT_STATE:
			WaitProcess(_ptr);
			break;
		case FILE_TRANS_READY_STATE:
			ReadyProcess(_ptr);
			break;
		case FILE_TRANS_STATE:
			FileTransProcess(_ptr);
			break;
		case FILE_TRANS_COMPLETE_STATE:
			FileTransCompleteProcess(_ptr);
			break;
		case DISCONNECTED_STATE:
			if (_ptr->Next_Event != nullptr)
				SetEvent(_ptr->Next_Event);

			ReMoveClientInfo(_ptr);
			endflag = true;
			break;

		}//switch end

		if (endflag)
		{
			break;
		}

	}

	return 0;
}

void InitProcess(_ClientInfo* _ptr)
{
	char filename[FILENAMESIZE];
	int filesize;

	int size = PackPacket(_ptr->send_buf, INTRO, INTRO_MSG);

	int retval = send(_ptr->sock, _ptr->send_buf, size, 0);
	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		_ptr->state = DISCONNECTED_STATE;
		return;
	}

	_ptr->state = FILE_TRANS_WAIT_STATE;
}

void WaitProcess(_ClientInfo* _ptr)
{
	// 사전에 같은 이름의 파일이 전송되고 있는지를 판단하기위해 미리 이름과 사이즈를 받아옵니다.
	if (!PacketRecv(_ptr->sock, _ptr->recv_buf))
	{
		_ptr->state = DISCONNECTED_STATE;
		return;
	}

	UnPackPacket(_ptr->recv_buf, _ptr->file_info.filename, _ptr->file_info.filesize);
	bool flag = false;
	for (int i = 0; i < count; i++)
	{
		if (ClientInfo[i] != _ptr)
		{
			// 만약 지금까지 접속한 클라이언트중에 자신이 전송하고자 하는 파일과 같은이름을 가진 클라이언트가 있고
			if (!strcmp(ClientInfo[i]->file_info.filename, _ptr->file_info.filename))
			{
				if (ClientInfo[i]->file_info.filesize == _ptr->file_info.filesize)
				{
					if (ClientInfo[i]->Next_Event == nullptr)
					{	/* 해당클라이언트의 next_event가 없다면 이벤트를 전송중인 클라이언트의 next로 만들어서 
						전송중인 클라이언트가 종료되면 자신 클라이언트의 이벤트를 켜서 실행시킵니다.*/
						int size = PackPacket(_ptr->send_buf, FILE_TRANS_DENY, FILEWAIT, FILE_WAITING_MSG);
						int retval = send(_ptr->sock, _ptr->send_buf, size, 0);
						if (retval == SOCKET_ERROR)
						{
							err_display("send()");
							_ptr->state = DISCONNECTED_STATE;
							return;
						}
						
						ClientInfo[i]->Next_Event = _ptr->Turn_Event;
						flag = true;
						break;
					}
				}
			}
		}
	}

	if (!flag)
		SetEvent(_ptr->Turn_Event);

	WaitForSingleObject(_ptr->Turn_Event, INFINITE);
	_ptr->state = FILE_TRANS_READY_STATE;
}

void ReadyProcess(_ClientInfo* _ptr)
{
	char filename[FILENAMESIZE];
	int filesize;
	int retval;

	
	
	PROTOCOL protocol = GetProtocol(_ptr->recv_buf);

	switch (protocol)
	{
	case FILE_INFO:
		memset(filename, 0, sizeof(filename));

		UnPackPacket(_ptr->recv_buf, filename, filesize);

		printf("-> 받을 파일 이름: %s\n", filename);
		printf("-> 받을 파일 크기: %d\n", filesize);

		if (SearchFile(filename))
		{
			FILE* fp = fopen(filename, "rb");
			fseek(fp, 0, SEEK_END);
			int fsize = ftell(fp);
			fclose(fp);

			if (filesize == fsize)
			{
				printf("존재하는 파일 전송 요구\n");

				int size = PackPacket(_ptr->send_buf, FILE_TRANS_DENY, FILEEXIST, FILE_DUPLICATE_MSG);

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
			}
			else
			{	// 이어받기
				strcpy(_ptr->file_info.filename, filename);
				_ptr->file_info.filesize = filesize;
				_ptr->file_info.nowsize = fsize;
			}
		}
		else
		{// 처음부터 받기
			strcpy(_ptr->file_info.filename, filename);
			_ptr->file_info.filesize = filesize;
			_ptr->file_info.nowsize = 0;
		}

		int size = PackPacket(_ptr->send_buf, FILE_TRANS_START_POINT, _ptr->file_info.nowsize);

		retval = send(_ptr->sock, _ptr->send_buf, size, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			_ptr->state = DISCONNECTED_STATE;
			return;
		}
		_ptr->state = FILE_TRANS_STATE;
		break;
	}

}

void FileTransProcess(_ClientInfo* _ptr)
{
	static FILE* fp = nullptr;
	char buf[BUFSIZE];

	if (!PacketRecv(_ptr->sock, _ptr->recv_buf))
	{
		_ptr->state = FILE_TRANS_COMPLETE_STATE;
		if(_ptr->Next_Event != nullptr)
			SetEvent(_ptr->Next_Event);
		fclose(fp);
		fp = nullptr;
		return;
	}

	PROTOCOL protocol = GetProtocol(_ptr->recv_buf);

	switch (protocol)
	{
	case FILE_TRANS_DATA:
		if (fp == nullptr)
			fp = fopen(_ptr->file_info.filename, "ab");

		int transsize;
		UnPackPacket(_ptr->recv_buf, transsize, buf);
		fwrite(buf, 1, transsize, fp);
		if (ferror(fp))
		{
			perror("파일 입출력 오류");
			_ptr->state = FILE_TRANS_COMPLETE_STATE;
			fclose(fp);
			return;
		}
		_ptr->file_info.nowsize += transsize;
		break;
	}
	
}

void FileTransCompleteProcess(_ClientInfo* _ptr)
{
	printf("-> 받은 데이터 : %d\n", _ptr->file_info.nowsize);

	if (_ptr->file_info.filesize != 0 && _ptr->file_info.filesize == _ptr->file_info.nowsize)
	{
		printf("-> 전송완료!!\n");
	}
	else
	{
		if (_ptr->Next_Event != nullptr)
			printf("-> 이어받기 시작!!\n");
		else
			printf("-> 전송실패!!\n");
	}
	

	_ptr->state = DISCONNECTED_STATE;
}