#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    4096

// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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

enum PROTOCOL
{
	FILE_NAME = 1,
	FILE_CONTENT
};

int Packing(char* _buf, PROTOCOL _protocol, int totalsize, const char* _msg)
{
	char* ptr = _buf + sizeof(int);
	int size = 0;

	memcpy(ptr, &_protocol, sizeof(PROTOCOL));
	size += sizeof(PROTOCOL);
	ptr += sizeof(PROTOCOL);

	memcpy(ptr, &totalsize, sizeof(int));
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

int main(int argc, char *argv[])
{
	int retval;

	if(argc != 2){
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		exit(1);
	}

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit("connect()");

	char buf[BUFSIZE];

	// ���� ����
	FILE *fp = fopen(argv[1], "rb");
	if(fp == NULL){
		perror("fopen()");
		return 1;
	}
	/*--- ���� �̸� ������ ---*/
	ZeroMemory(buf, sizeof(buf));
	int size = Packing(buf, FILE_NAME, NULL, argv[1]);
	retval = send(sock, buf, size, 0);
	if (retval == SOCKET_ERROR)
		err_quit("send()");

	// ���� ������ ������(���� �� ���� ����)

	/*--- ���� ũ�� ��� ---*/
	fseek(fp, 0, SEEK_END);
	int length = ftell(fp);
	rewind(fp);
	
	/*--- ���� ������ ������ ---*/
		char* contents = new char[length];
	if (contents == NULL) {
		fprintf(stderr, "malloc() error!\n");
		exit(1);
	}

	int nbytes = fread(contents, 1, length, fp);
	if(nbytes == length)
	{
		ZeroMemory(buf, sizeof(buf));
		size = Packing(buf, FILE_CONTENT, nbytes, contents);
		retval = send(sock, buf, size, 0);
		if(retval == SOCKET_ERROR)
			err_quit("send()");

		printf("-> ���� ���� �Ϸ�! (%d����Ʈ)\n", nbytes);
	}
	else
	{
		perror("fread()");
	}
	fclose(fp);

	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}