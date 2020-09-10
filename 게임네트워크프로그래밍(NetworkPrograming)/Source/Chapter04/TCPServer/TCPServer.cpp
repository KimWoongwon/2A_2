#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE    512

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

int main(int argc, char *argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	// WSAStartup() << dll�� �޸𸮿� �ε��ϰ� �ʱ�ȭ ��Ű�� �Լ�
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)	
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;															// _IN << IPv4���� ���� (_IN6 << IPv6���� ����)
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);									// INADDR_ANY ���� ���α׷��� ���� ȣ��Ʈ�� IP�ּҸ� �������� IP�ּҰ� �������� ��� � IP�ε� ������ �ְԲ� ����
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));		// ����(�ּұ���ü)�� �����Ͱ� �Լ��� ���ڰ����� �� ��� �� �ڿ� �׻� ����ü�� ũ�Ⱚ�� ����.
	if(retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	// listen()�Լ��� �Ѱ��� ������ �����û, �װ͸� �޴� ��������������� ��ȯ�Ѵ�.
	// �������Ͽ��� �����ûť�� ����� SOMAXCONN�� �ý����� ������ �ִ��� �����û�� ������ �� �ְ� �϶�� ��.
	retval = listen(listen_sock, SOMAXCONN);										
	if(retval == SOCKET_ERROR) err_quit("listen()");

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE+1];		// ��Ź��ۿ� �� ������ �ְų�, ���Ź��ۿ��� �����͸� ������ ����	char���ΰ��� ���ڿ������� ������ �ƴ�, ����Ʈ������ �����ϰڴٴ� �ǹ��̴�.

	while(1)		//
	{
		// accept()
		// ���� Ŭ��� ����Ǽ� ����� ������ ������ִ� �۾�.
		addrlen = sizeof(clientaddr);												// ó������ �غ��� ������ �ּҰ��� ũ�⸦ �ְ� accept���� ���ϵǸ� ���� �ּ��� ũ�⸦ �޴� �����̴�. (Input, Output��� ����)
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);		// ���������� �����ûť���� �����û��Ŷ�� �����ͼ� clientaddr�� �����ϰ� client_sock�� �����Ѵ�.
		if(client_sock == INVALID_SOCKET)											// 
		{
			err_display("accept()");
			break;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",			
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));			// sin_addr�� long�ε� �츮�� ��� ���� xxx.xxx.xxx.xxx�� ���ڿ� ���·� �ٲ��ַ��� ��Ʈ��ũ����Ʈ ���ĵ� �����͸� inet_ntoa�� �ٲ��ش�.
																					// sin_port ���� ��Ʈ��ũ����Ʈ ���� ���ֱ� ������ ntohs ȣ��Ʈ����Ʈ���ķ� �������ش�.
		// Ŭ���̾�Ʈ�� ������ ���
		while(1){
			// ������ �ޱ�
			retval = recv(client_sock, buf, BUFSIZE, 0);				// ���Ź��ۿ��� �����͸� ������ buf �޸𸮿� �Ȱ� ��´�. BUFSIZE�� ���Ź��۰� �����ü� �ִ� �ִ����� �����´ٴ� �ǹ�.
			if(retval == SOCKET_ERROR)									// recv�� ���ϰ��� �����Ѵٸ� 1�̻��� ������� ��ȯ�Ѵ� �̰��� �޾ƿ� �������� ũ���̴�.
			{									
				err_display("recv()");
				break;
			}
			else if(retval == 0)										// retval�� 0�϶� Ŭ���̾�Ʈ ������ �������� �ɶ� SOCKET_ERROR(-1)�϶� �������� �����϶� �߻��Ѵ�.
				break;

			// ���� ������ ���
			buf[retval] = '\0';
			printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
				ntohs(clientaddr.sin_port), buf);

			// ������ ������
			// send() << �۽Ź��ۿ� �����͸� ���� �Լ� 
			retval = send(client_sock, buf, retval, 0);					// ���ڼ����̱� ������ �޾ƿ� �뷮��ŭ �ٽ� �ǵ����ְԲ� �����Ǿ��ִ�.
			if(retval == SOCKET_ERROR)									// �۽��� ������ ���� ��� �� ������ ������ ������ ������ ���°�쿡 SOCKET_ERROR(-1)�� �����Ѵ�.
			{															// retval�� ������ ũ�⸸ŭ�� �۽Ź��۰� �������� block �����. �۽Ź��ۿ� ���ϴ� ũ�⸸ŭ ������ ������ �۽Ź��ۿ� �����͸� ����.
				err_display("send()");	
				break;
			}
		}

		// closesocket()
		closesocket(client_sock);
		printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	// closesocket()
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();			// WSAStartup���� �ҷ��� dll�� �����ϴ� �Լ�.
	return 0;
}