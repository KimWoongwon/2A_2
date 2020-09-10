#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000
#define BUFSIZE    512

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

int main(int argc, char *argv[])
{
	int retval;

	// 윈속 초기화
	// WSAStartup() << dll을 메모리에 로드하고 초기화 시키는 함수
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)	
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;															// _IN << IPv4버전 소켓 (_IN6 << IPv6버전 소켓)
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);									// INADDR_ANY 현재 프로그램이 속한 호스트의 IP주소를 가져오며 IP주소가 여러개일 경우 어떤 IP로도 받을수 있게끔 셋팅
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));		// 소켓(주소구조체)의 포인터가 함수의 인자값으로 들어갈 경우 그 뒤엔 항상 구조체의 크기값이 들어간다.
	if(retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	// listen()함수에 넘겨준 소켓을 연결요청, 그것만 받는 리슨전용소켓으로 변환한다.
	// 리슨소켓에는 연결요청큐가 생기며 SOMAXCONN은 시스템이 가능한 최대의 연결요청을 저장할 수 있게 하라는 뜻.
	retval = listen(listen_sock, SOMAXCONN);										
	if(retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE+1];		// 통신버퍼에 쓸 데이터 넣거나, 수신버퍼에서 데이터를 가져올 버퍼	char형인것은 문자열저장의 목적이 아닌, 바이트단위로 관리하겠다는 의미이다.

	while(1)		//
	{
		// accept()
		// 실제 클라와 연결되서 통신할 소켓을 만들어주는 작업.
		addrlen = sizeof(clientaddr);												// 처음에는 준비한 소켓의 주소값의 크기를 넣고 accept에서 리턴되면 실제 주소의 크기를 받는 변수이다. (Input, Output겸용 변수)
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);		// 리슨소켓의 연결요청큐에서 연결요청패킷을 가져와서 clientaddr에 셋팅하고 client_sock를 셋팅한다.
		if(client_sock == INVALID_SOCKET)											// 
		{
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",			
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));			// sin_addr은 long인데 우리가 평소 보는 xxx.xxx.xxx.xxx의 문자열 형태로 바꿔주려면 네트워크바이트 정렬된 데이터를 inet_ntoa로 바꿔준다.
																					// sin_port 또한 네트워크바이트 정렬 되있기 때문에 ntohs 호스트바이트정렬로 변경해준다.
		// 클라이언트와 데이터 통신
		while(1){
			// 데이터 받기
			retval = recv(client_sock, buf, BUFSIZE, 0);				// 수신버퍼에서 데이터를 가져다 buf 메모리에 옴겨 담는다. BUFSIZE는 수신버퍼가 가져올수 있는 최대사이즈를 가져온다는 의미.
			if(retval == SOCKET_ERROR)									// recv의 리턴값은 성공한다면 1이상의 양수값을 반환한다 이것은 받아온 데이터의 크기이다.
			{									
				err_display("recv()");
				break;
			}
			else if(retval == 0)										// retval이 0일땐 클라이언트 소켓이 정상종료 될때 SOCKET_ERROR(-1)일땐 강제적인 종료일때 발생한다.
				break;

			// 받은 데이터 출력
			buf[retval] = '\0';
			printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
				ntohs(clientaddr.sin_port), buf);

			// 데이터 보내기
			// send() << 송신버퍼에 데이터를 쓰는 함수 
			retval = send(client_sock, buf, retval, 0);					// 에코서버이기 때문에 받아온 용량만큼 다시 되돌려주게끔 구현되어있다.
			if(retval == SOCKET_ERROR)									// 송신할 소켓이 없는 경우 즉 연결이 끊어져 소켓의 정보가 없는경우에 SOCKET_ERROR(-1)을 리턴한다.
			{															// retval에 쓰여진 크기만큼이 송신버퍼가 빌때까지 block 멈춘다. 송신버퍼에 원하는 크기만큼 여유가 생길경우 송신버퍼에 데이터를 쓴다.
				err_display("send()");	
				break;
			}
		}

		// closesocket()
		closesocket(client_sock);
		printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	// closesocket()
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();			// WSAStartup으로 불러온 dll을 정리하는 함수.
	return 0;
}