#pragma comment(lib, "ws2_32.lib")
#include "Global.h"

int recvn(SOCKET sock, char* buf, int len, int flags);

int Packing(char* _buf, PROTOCOL _protocol, const char* _id, const char* _pw);
int Packing(char* _buf, PROTOCOL _protocol);
int Packing(char* _buf, PROTOCOL _protocol, const int* inputarr, int arrsize);
void UnPacking(const char* _buf, int& _result, char* str1);
void UnPacking(const char* _buf, char* str1);

int main()
{
	WSADATA  wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	int retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_quit("connect()");
	}

	char buf[BUFSIZE];

	while (1)
	{
		int select;
		do
		{
			printf("<<메뉴>>\n");
			printf("1.가입\n");
			printf("2.로그인\n");
			printf("3.종료\n");
			printf("선택:");
			scanf("%d", &select);
			if (select < 1 || select > 3)		// 범위내 값이 아닐경우 예외 처리
			{
				printf("잘못된 입력입니다.\n\n");
				getchar();		// 문자열 입력시 입력버퍼 제거하기 위해서
			}
				
		} while (select < 1 || select > 3);
		
		if (select == EXIT)
			break;

		_UserInfo info;
		printf("ID:");
		scanf("%s", info.id);
		printf("PW:");
		scanf("%s", info.pw);

		int size = Packing(buf, (PROTOCOL)select, info.id, info.pw);

		// 로그인, 회원가입시 ID, PW 값 보내주는 부분
		retval = send(sock, buf, size, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}

		// Login_Result, Join_Result 에 해당하는 프로토콜을 받아오기위한 부분
		retval = recvn(sock, (char*)&size, sizeof(int), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			break;
		}
		else if (retval == 0)
		{
			break;
		}

		retval = recvn(sock, buf, size, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			break;
		}
		else if (retval == 0)
		{
			break;
		}

		// 받아온 패킷에서 프로토콜 분리
		PROTOCOL protocol;
		memcpy(&protocol, buf, sizeof(PROTOCOL));

		char msg[BUFSIZE];
		ZeroMemory(msg, sizeof(msg));
		int result;
		bool endflag = false;
		bool isEndGame = false;
		switch (protocol)
		{
		case LOGIN_RESULT:
			UnPacking(buf, result, msg);	// 받아온 패킷에서 프로토콜 너머의 결과값을 받아오기
			switch (result)
			{
			case PW_ERROR:
			case ID_ERROR:
				printf("%s\n", msg);
				break;
			case LOGIN_SUCCESS:
				printf("%s\n", msg);
				while (!endflag)
				{
					do
					{
						printf("<<숫자야구 게임>>\n");
						printf("1.게임 시작\n");
						printf("2.종료\n");
						printf("선택:");
						scanf("%d", &select);
						if (select < 1 || select > 2)		// 예외처리 문자열입력시 입력버퍼 제거 포함
						{
							printf("잘못된 입력입니다.\n\n");
							getchar();
						}
					} while (select < 1 || select > 2);
					
					select = select == 1 ? GAME_START : GAME_END;	// 입력값이랑 enum의 값이 다르기때문에 처리하는 부분

					size = Packing(buf, (PROTOCOL)select);		// 선택한 값을 packing해서 보내주기
					retval = send(sock, buf, size, 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}

					// 인트로 메시지 받기
					retval = recvn(sock, (char*)&size, sizeof(int), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					else if (retval == 0)
					{
						break;
					}

					retval = recvn(sock, buf, size, 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					else if (retval == 0)
					{
						break;
					}

					PROTOCOL protocol;		// GAME_START, GAME_END 값을 구별하기 위한 protocol
					memcpy(&protocol, buf, sizeof(PROTOCOL));

					switch (protocol)
					{
					case GAME_START:
						ZeroMemory(msg, sizeof(msg));
						UnPacking(buf, msg);		// 프로토콜 너머의 GAME_RESULT값 받아오기
						printf("%s\n", msg);

						isEndGame = false;
						while (!isEndGame)
						{
							int input[3];
							for (int i = 0; i < ARRAY_SIZE;i++)
							{
								printf("%d번째 숫자 : ", i + 1);
								scanf("%d", &input[i]);
							}
							// Packing할때 제일 앞쪽의 값이 Protocol인데 이부분에서 구조상 프로토콜을 사용하지 않기 때문에 NULL값으로 생성
							size = Packing(buf, (PROTOCOL)NULL, input, 3);	// 내부에서 Input 배열을 패킷에 집어넣어 준다.
							retval = send(sock, buf, size, 0);				// packing한 buf를 보내준다.
							if (retval == SOCKET_ERROR)
							{
								err_display("send()");
								break;
							}
							// buf에 첫부분은 사이즈 값이기때문에 때어서 받는다. 후에 밑에서 사용한다.
							retval = recvn(sock, (char*)&size, sizeof(int), 0);
							if (retval == SOCKET_ERROR)
							{
								err_display("recv()");
								break;
							}
							else if (retval == 0)
							{
								break;
							}
							// 게임로직을 통해 나온 GAME_RESULT내부의 결과값을 받아온다.
							retval = recvn(sock, buf, size, 0);
							if (retval == SOCKET_ERROR)
							{
								err_display("recv()");
								break;
							}
							else if (retval == 0)
							{
								break;
							}

							// 결과값과 메시지를 buf에서 복사해온다.
							// switch문으로 처리한다.
							ZeroMemory(msg, sizeof(msg));
							UnPacking(buf, result, msg);
							switch (result)
							{
							case CORRECT:
								printf("%s\n", msg);
								isEndGame = true;
								break;
							case INPUT_ERROR:		// input, incorrect 모두 처리할 내용없이 메시지만 출력해주면 된다.
							case INCORRECT:
								printf("%s\n", msg);
								break;
							}

						}
						break;
					case GAME_END:
						ZeroMemory(msg, sizeof(msg));
						UnPacking(buf, msg);
						printf("%s\n", msg);
						endflag = true;
						break;
					}
				}
				
				break;
			}
			break;
		case JOIN_RESULT:
			UnPacking(buf, result, msg);
			switch (result)
			{
			case ID_EXIST:
			case JOIN_SUCCESS:
				printf("%s\n", msg);
				break;
			}
			break;
		}
	}

	closesocket(sock);

	WSACleanup();
	return 0;
}

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

#pragma region Packing_Function
int Packing(char* _buf, PROTOCOL _protocol, const char* _id, const char* _pw)
{
	char* ptr = _buf + sizeof(int);
	int size = 0;

	memcpy(ptr, &_protocol, sizeof(PROTOCOL));
	size = size + sizeof(PROTOCOL);
	ptr = ptr + sizeof(PROTOCOL);

	int strsize1 = strlen(_id);
	memcpy(ptr, &strsize1, sizeof(int));
	size = size + sizeof(int);
	ptr = ptr + sizeof(int);

	memcpy(ptr, _id, strsize1);
	size = size + strsize1;
	ptr = ptr + strsize1;

	int strsize2 = strlen(_pw);
	memcpy(ptr, &strsize2, sizeof(int));
	size = size + sizeof(int);
	ptr = ptr + sizeof(int);

	memcpy(ptr, _pw, strsize2);
	size = size + strsize2;
	ptr = ptr + strsize2;

	ptr = _buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}

int Packing(char* _buf, PROTOCOL _protocol)
{
	char* ptr = _buf + sizeof(int);
	int size = 0;

	memcpy(ptr, &_protocol, sizeof(PROTOCOL));
	size = size + sizeof(PROTOCOL);
	ptr = ptr + sizeof(PROTOCOL);

	ptr = _buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}

int Packing(char* _buf, PROTOCOL _protocol, const int* inputarr, int arrsize)
{
	char* ptr = _buf + sizeof(int);
	int size = 0;

	memcpy(ptr, &_protocol, sizeof(PROTOCOL));
	size = size + sizeof(PROTOCOL);
	ptr = ptr + sizeof(PROTOCOL);

	for (int i = 0; i < arrsize; i++)
	{
		memcpy(ptr, &inputarr[i], sizeof(int));
		size = size + sizeof(int);
		ptr = ptr + sizeof(int);
	}

	ptr = _buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}

void UnPacking(const char* _buf, int& _result, char* str1)
{
	const char* ptr = _buf + sizeof(PROTOCOL);
	int strsize1;

	memcpy(&_result, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(&strsize1, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(str1, ptr, strsize1);
	ptr = ptr + strsize1;
}
void UnPacking(const char* _buf, char* str1)
{
	const char* ptr = _buf + sizeof(PROTOCOL);
	int strsize1;

	memcpy(&strsize1, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(str1, ptr, strsize1);
	ptr = ptr + strsize1;
}
#pragma endregion

