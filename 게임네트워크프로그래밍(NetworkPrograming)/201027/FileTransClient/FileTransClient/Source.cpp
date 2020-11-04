#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <ws2tcpip.h> 
#include <time.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    4096
#define FILENAMESIZE 256

#pragma region Error_Function
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

enum PROTOCOL
{
	INTRO = 1,
	FILE_INFO,
	FILE_TRANS_DENY,
	FILE_TRANS_START_POINT,
	FILE_TRANS_DATA
};

enum DENY_CODE
{
	FILEEXIST = -1
};

struct _File_info
{
	char filename[FILENAMESIZE];
	int  filesize;
	int  nowsize;
};

struct _ServerInfo
{
	SOCKET sock;
	SOCKADDR_IN addr;
	_File_info  file_info;
	char recv_buf[BUFSIZE];
	char send_buf[BUFSIZE];
};

int recvn(SOCKET sock, char* buf, int len, int flags);
bool PacketRecv(SOCKET _sock, char* _buf);
PROTOCOL GetProtocol(char* _buf);

int PackPacket(char*, PROTOCOL, char*, int);	// Filename, size
int PackPacket(char*, PROTOCOL, int, char*);	// fileInfo, totalsize

void UnPackPacket(const char*, DENY_CODE*, char*);
void UnPackPacket(const char*, char*);
void UnPackPacket(const char*, int&);

bool IntroProcess(_ServerInfo*);
bool DenyProcess(_ServerInfo*);
bool TransProcess(_ServerInfo*);

int main()
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	int retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		err_quit("connect()");

	_ServerInfo* ptr = new _ServerInfo;
	ZeroMemory(ptr, sizeof(_ServerInfo));
	ptr->sock = sock;
	memcpy(&ptr->addr, &serveraddr, sizeof(serveraddr));

	char msg[BUFSIZE];
	bool endFlag = true;
	while (endFlag)
	{
		ZeroMemory(ptr->recv_buf, sizeof(ptr->recv_buf));
		PacketRecv(sock, ptr->recv_buf);
		PROTOCOL protocol = GetProtocol(ptr->recv_buf);

		switch (protocol)
		{
		case INTRO:
			endFlag = IntroProcess(ptr);
			break;
		case FILE_TRANS_DENY:
			endFlag = DenyProcess(ptr);
			break;
		case FILE_TRANS_START_POINT:
			endFlag = TransProcess(ptr);
			break;
		}
		
	}
	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
	
}



#pragma region __Define_Packet_Function__
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
#pragma endregion

#pragma region __Define_Packing_Function__

int PackPacket(char* _buf, PROTOCOL _protocol, int length, char* _str)	// In_File_Data
{
	char* ptr = _buf;
	int size = 0;

	ptr = ptr + sizeof(size);

	memcpy(ptr, &_protocol, sizeof(PROTOCOL));
	ptr = ptr + sizeof(PROTOCOL);
	size = size + sizeof(PROTOCOL);

	memcpy(ptr, &length, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, _str, length);
	ptr = ptr + length;
	size = size + length;

	ptr = _buf;
	memcpy(ptr, &size, sizeof(size));

	size = size + sizeof(size);
	return size;
}

int PackPacket(char* _buf, PROTOCOL _protocol, char* _str, int data)	// File	Name & Size
{
	char* ptr = _buf;
	int strsize = strlen(_str);
	int size = 0;

	ptr = ptr + sizeof(size);

	memcpy(ptr, &_protocol, sizeof(PROTOCOL));
	ptr = ptr + sizeof(PROTOCOL);
	size = size + sizeof(PROTOCOL);

	memcpy(ptr, &strsize, sizeof(int));
	ptr = ptr + sizeof(int);
	size = size + sizeof(int);

	memcpy(ptr, _str, strsize);
	ptr = ptr + strsize;
	size = size + strsize;

	memcpy(ptr, &data, sizeof(int));
	ptr = ptr + sizeof(int);
	size += sizeof(int);

	ptr = _buf;
	memcpy(ptr, &size, sizeof(size));

	size = size + sizeof(size);
	return size;
}

void UnPackPacket(const char* _buf, int& _data)//Trans_Start
{
	const char* ptr = _buf + sizeof(PROTOCOL);

	memcpy(&_data, ptr, sizeof(int));
	ptr = ptr + sizeof(int);
}

void UnPackPacket(const char* _buf, DENY_CODE* _code, char* _str1)//Deny
{
	const char* ptr = _buf + sizeof(PROTOCOL);
	int strsize;

	memcpy(_code, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(&strsize, ptr, sizeof(strsize));
	ptr = ptr + sizeof(strsize);

	memcpy(_str1, ptr, strsize);
	ptr = ptr + strsize;

}

void UnPackPacket(const char* _buf, char* _str1)//Intro Message
{
	const char* ptr = _buf + sizeof(PROTOCOL);
	int strsize;

	memcpy(&strsize, ptr, sizeof(strsize));
	ptr = ptr + sizeof(strsize);

	memcpy(_str1, ptr, strsize);
	ptr = ptr + strsize;

}
#pragma endregion

#pragma region __Define_Process__
bool IntroProcess(_ServerInfo* ptr)
{
	char msg[BUFSIZE];

	ZeroMemory(msg, sizeof(msg));
	UnPackPacket(ptr->recv_buf, msg);
	printf("%s : ", msg);
	scanf("%s", ptr->file_info.filename);

	FILE* fp = fopen(ptr->file_info.filename, "rb");
	if (fp == NULL)
	{
		printf("해당 파일 없음\n");
		return false;
	}
	fseek(fp, 0, SEEK_END);
	ptr->file_info.filesize = ftell(fp);
	fclose(fp);

	ZeroMemory(ptr->send_buf, sizeof(ptr->send_buf));
	int size = PackPacket(ptr->send_buf, FILE_INFO,
		ptr->file_info.filename, ptr->file_info.filesize);

	int retval = send(ptr->sock, ptr->send_buf, size, 0);
	if (retval == SOCKET_ERROR)
		err_display("send()");

	return true;
}
bool DenyProcess(_ServerInfo* ptr)
{
	DENY_CODE code;
	char msg[BUFSIZE];
	ZeroMemory(msg, sizeof(msg));
	UnPackPacket(ptr->recv_buf, &code, msg);

	switch (code)
	{
	case FILEEXIST:
		printf("%s", msg);
		break;
	}
	return false;
}
bool TransProcess(_ServerInfo* ptr)
{
	char msg[BUFSIZE];
	ZeroMemory(msg, sizeof(msg));

	UnPackPacket(ptr->recv_buf, ptr->file_info.nowsize);
	FILE* fp = fopen(ptr->file_info.filename, "rb");
	fseek(fp, ptr->file_info.nowsize, SEEK_SET);
	int fsize = fread(msg, 1, ptr->file_info.filesize - ptr->file_info.nowsize, fp);

	ZeroMemory(ptr->send_buf, sizeof(ptr->send_buf));
	int size = PackPacket(ptr->send_buf, FILE_TRANS_DATA, fsize, msg);
	int retval = send(ptr->sock, ptr->send_buf, size, 0);
	if (retval == SOCKET_ERROR)
		err_display("send()");

	printf("%d바이트 전송!", fsize);

	return false;
}
#pragma endregion

