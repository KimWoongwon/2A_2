#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE    4096
#define MAX_CLIENT 10

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

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while(left > 0){
		received = recv(s, ptr, left, flags);
		if(received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if(received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

enum PROTOCOL
{
	FILE_NAME = 1,
	FILE_CONTENT
};

struct _ClientInfo
{
	SOCKET sock;
	SOCKADDR_IN addr;

	char buf[BUFSIZE];
};

_ClientInfo* ClientInfo[MAX_CLIENT];
int Client_Count = 0;

_ClientInfo* AddClientInfo(SOCKET sock, SOCKADDR_IN addr)
{
	_ClientInfo* ptr = new _ClientInfo;
	ZeroMemory(ptr, sizeof(_ClientInfo));
	ptr->sock = sock;
	memcpy(&ptr->addr, &addr, sizeof(addr));

	ClientInfo[Client_Count++] = ptr;

	printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));

	return ptr;
}
void ReMoveClientInfo(_ClientInfo* ptr)
{
	printf("\n[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(ptr->addr.sin_addr), ntohs(ptr->addr.sin_port));

	for (int i = 0; i < Client_Count; i++)
	{
		if (ClientInfo[i] == ptr)
		{
			closesocket(ptr->sock);
			delete ptr;
			for (int j = i; j < Client_Count - 1; j++)
			{
				ClientInfo[j] = ClientInfo[j + 1];
			}
			break;
		}
	}

	Client_Count--;
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

void UnPacking(const char* _buf, char* _str)
{
	const char* ptr = _buf + sizeof(PROTOCOL) + sizeof(int);
	int str1_size = 0;

	memcpy(&str1_size, ptr, sizeof(int));
	ptr += sizeof(int);

	memcpy(_str, ptr, str1_size);
	ptr += str1_size;
}

void UnPacking(const char* _buf, int* _byte, char* contents)
{
	const char* ptr = _buf + sizeof(PROTOCOL);

	memcpy(_byte, ptr, sizeof(int));
	ptr += sizeof(int);

	int size = 0;
	memcpy(&size, ptr, sizeof(int));
	ptr += sizeof(int);

	memcpy(contents, ptr, size);
	ptr += size;

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
	

	while(1)
	{
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if(client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}

		_ClientInfo* ptr = AddClientInfo(client_sock, clientaddr);
		bool endFlag = true;
		// 클라이언트와 데이터 통신
		do 
		{
			ZeroMemory(ptr->buf, sizeof(ptr->buf));
			if (!PacketRecv(ptr->sock, ptr->buf))
			{
				printf("-> 파일 전송 실패!\n");
				break;
			}

			PROTOCOL protocol = GetProtocol(ptr->buf);

			char file[BUFSIZE];
			char contents[BUFSIZE];
			int totalbytes = 0;
			FILE* fp;

			switch (protocol)
			{
			case FILE_NAME:
				ZeroMemory(file, sizeof(file));
				UnPacking(ptr->buf, file);

				printf("-> 받을 파일 이름: %s\n", file);
				break;
			case FILE_CONTENT:
				fp = fopen(file, "wb");
				if (fp == NULL) {
					perror("fopen()");
					break;
				}
				
				UnPacking(ptr->buf, &totalbytes, contents);

				int length = fwrite(contents, 1, totalbytes, fp);

				if (length == totalbytes)
				{
					printf("-> 파일 전송 완료! (%d바이트)\n", totalbytes);
					endFlag = false;
				}
				else
				{
					printf("-> 파일 전송 실패!\n");
					endFlag = false;
				}
					
				fclose(fp);
				break;
			}

			
		} while(endFlag);

		// closesocket()
		ReMoveClientInfo(ptr);
	}

	// closesocket()
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}