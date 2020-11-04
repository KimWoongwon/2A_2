#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE    512
#define PLAYER_COUNT 100
#define JOIN_USER_COUNT 100
#define FILE_NAME "UserList.dat"

#define ID_ERROR_MSG "없는 아이디입니다\n"
#define PW_ERROR_MSG "패스워드가 틀렸습니다\n"
#define LOGIN_SUCCESS_MSG "로그인에 성공했습니다.\n"
#define JOIN_SUCCESS_MSG "가입에 성공했습니다.\n"
#define ID_EXIST_MSG "이미있는 아이디입니다.\n"
#define LOGOUT_MSG "로그아웃되었습니다.\n"
#define DROP_MSG "탈퇴하였습니다.\n"

#pragma region Error_Function
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


enum PROTOCOL	{	JOIN = 1, LOGIN, EXIT, LOGOUT, DROP,	LOGIN_RESULT, JOIN_RESULT };
enum RES_LOGIN	{	ID_ERROR = 1, PW_ERROR, LOGIN_SUCCESS };
enum RES_JOIN	{	JOIN_SUCCESS, ID_EXIST };

struct _UserInfo
{
	char id[255];
	char pw[255];

	_UserInfo(const _UserInfo& info)
	{
		strcpy(id, info.id);
		strcpy(pw, info.pw);
	}
	_UserInfo()	{}
};

struct _ClientInfo
{
	SOCKET sock;
	SOCKADDR_IN addr;
	_UserInfo* info;
	bool login;

	char buf[BUFSIZE + 1];
};

_ClientInfo* ClientInfo[PLAYER_COUNT];
int Count = 0;

_UserInfo* UserInfo[JOIN_USER_COUNT];
int JoinCount = 0;

CRITICAL_SECTION cs;

_ClientInfo* AddClientInfo(SOCKET sock, SOCKADDR_IN addr);
void ReMoveClientInfo(_ClientInfo* ptr);

void FileLoad();					
void FileSave();		

bool PacketRecv(SOCKET _sock, char* _buf);
PROTOCOL GetProtocol(char* _buf);
int Packing(char* _buf, PROTOCOL _protocol, int _result, const char* _msg);
int Packing(char* _buf, PROTOCOL _protocol, const char* _msg);
void UnPacking(const char* _buf, char* _str1, char* _str2);

int JoinProcess(_UserInfo _info);
int LoginProcess(_UserInfo _info);
void DropProcess(_ClientInfo* _ptr);
_UserInfo* SearchUserInfo(const char* _Id);

// 클라이언트와 데이터 통신
DWORD WINAPI ProcessClient(LPVOID arg)
{
	_ClientInfo* ptr = (_ClientInfo*)arg;
	int retval;
	int size;
	while (1)
	{
		if (!PacketRecv(ptr->sock, ptr->buf))
			break;
		
		PROTOCOL protocol = GetProtocol(ptr->buf);

		_UserInfo info;
		int result;

		switch (protocol)
		{
		case JOIN:
			ZeroMemory(&info, sizeof(_UserInfo));
			UnPacking(ptr->buf, info.id, info.pw);
			result = JoinProcess(info);
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

		retval = send(ptr->sock, ptr->buf, size, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}
	}


	

	// closesocket()
	ReMoveClientInfo(ptr);


	return 0;
}

int main(int argc, char *argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return 1;

	FileLoad();

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

	HANDLE Client_thread[PLAYER_COUNT];
	int threadCount = 0;
	HANDLE hThread;

	InitializeCriticalSection(&cs);

	while(1){
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if(client_sock == INVALID_SOCKET){
			err_display("accept()");
			break;
		}

		_ClientInfo* ptr = AddClientInfo(client_sock, clientaddr);

		// 스레드 생성
		Client_thread[threadCount] = CreateThread(NULL, 0, ProcessClient, (LPVOID)ptr, 0, NULL);
		if (Client_thread[threadCount] == NULL)
			ReMoveClientInfo(ptr);
		else
			++threadCount;
	}
	WaitForMultipleObjects(threadCount, Client_thread, TRUE, INFINITE);
	DeleteCriticalSection(&cs);
	FileSave();
	
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
#pragma endregion

#pragma region Login_System_Function
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
	EnterCriticalSection(&cs);

	for (int i = 0; i < JoinCount; i++)
	{
		if (!strcmp(UserInfo[i]->id, _info.id))
		{
			if (!strcmp(UserInfo[i]->pw, _info.pw))
			{
				LeaveCriticalSection(&cs);
				return LOGIN_SUCCESS;
			}
			else
			{
				LeaveCriticalSection(&cs);
				return PW_ERROR;
			}
		}
	}
	LeaveCriticalSection(&cs);
	return ID_ERROR;
}
void DropProcess(_ClientInfo* _ptr)
{
	EnterCriticalSection(&cs);

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

	LeaveCriticalSection(&cs);
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
#pragma endregion

#pragma region File_Function
void FileLoad()
{
	FILE* fp = nullptr;
	fp = fopen(FILE_NAME, "rb");
	if (fp == NULL)
	{
		return;
	}

	while (1)
	{
		_UserInfo info;
		ZeroMemory(&info, sizeof(info));
		int retval = fread(&info, 1, sizeof(_UserInfo), fp);
		if (retval != sizeof(_UserInfo))
		{
			break;
		}

		_UserInfo* ptr = new _UserInfo();
		memcpy(ptr, &info, sizeof(_UserInfo));
		UserInfo[JoinCount++] = ptr;
	}
	fclose(fp);
}
void FileSave()
{
	FILE* fp = nullptr;
	fp = fopen("UserInfo.dat", "wb");
	if (fp == NULL)
	{
		err_quit("파일저장실패");
	}

	for (int i = 0; i < JoinCount; i++)
	{
		int retval = fwrite(UserInfo[i], 1, sizeof(_UserInfo), fp);
		if (retval != sizeof(_UserInfo))
		{
			err_quit("파일저장실패");
		}
	}
}
#pragma endregion
