#pragma comment(lib, "ws2_32.lib")
#include "Global.h"

_ClientInfo* ClientInfo[PLAYER_COUNT];
int Count = 0;

_UserInfo* UserInfo[JOIN_USER_COUNT];
int JoinCount = 0;

_ClientInfo* AddClientInfo(SOCKET sock, SOCKADDR_IN addr);		// 클라이언트 생성 함수
void ReMoveClientInfo(_ClientInfo* ptr);						// 클라이언트 삭제 함수

int recvn(SOCKET sock, char* buf, int len, int flags);			// 길이 지정 recv함수

int JoinProcess(_UserInfo info);		// 회원가입 과정 함수
int LoginProcess(_UserInfo info);		// 로그인 과정 함수

_UserInfo* SearchUserInfo(const char* _Id);	// 유저리스트에서 ID에 맞는 유저 찾는 함수

int Packing(char* _buf, PROTOCOL _protocol, int _result, const char* str1);	// 프로토콜과 결과값, 메세지를 패킹하는 과정 함수
int Packing(char* _buf, PROTOCOL _protocol, const char* str1);				// 프로토콜과 메시지를 패킹하는 과정 함수
void UnPacking(const char* _buf, char* str1, char* str2);					// 클라이언트에서 보낸 ID와 PW를 언패킹하는 함수
void  UnPacking(const char* _buf, int* _a, int* _b, int* _c);				// 클라이언트에서 보낸 야구게임에 필요한 int값 언패킹 하는 함수

void FileLoad();					// 파일에서 유저정보를 불러오는 함수
void FileSave(_UserInfo* info);		// 파일에 유저정보를 추가 작성하는 함수

bool Game_Input_OverLap_Error(int* Input);				// 클라이언트에서 보낸 숫자가 중복되지 않는지 체크하는 함수
bool Game_Input_Value_Error(int arrsize, int* Input);	// 클라이언트에서 보낸 숫자가 범위를 벗어나지 않는지 체크하는 함수

int BaseBallGame(int arrsize, const int* Answer, const int* Input, int& s, int& b, int& o);	// 야구게임 로직 함수
void SettingAnswer(int arrsize, int* Answer);	// 랜덤으로 정답값을 셋팅하는 함수

int main()
{
	WSADATA  wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	FileLoad();		// 프로그램 시작시 파일을 불러와 프로그램에 쓰는 함수.

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	int retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_quit("bind()");
	}

	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		err_quit("bind()");
	}

	SOCKADDR_IN clientaddr;
	SOCKET client_sock;
	char buf[BUFSIZE + 1];

	

	while (1)
	{
		int addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}

		_ClientInfo* ptr = AddClientInfo(client_sock, clientaddr);

		int GamePlayCount = 0;		// 진행한 게임이 몇게임인지 체크하는 변수
		int fast = 0;				// 가장 빨리 맞췄을때 기회를 몇번 썻는지 체크하기 위한 변수
		int last = 0;				// 가장 늦게 맞췄을때 기회를 몇번 썻는지 체크하기 위한 변수

		while (1)
		{
			int size;

			// 클라이언트 실행시 가입, 로그인, 게임시작, 게임종료 메뉴를 선택시 받아오는 부분
			retval = recvn(ptr->sock, (char*)&size, sizeof(int), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("recv()");
				break;
			}
			else if (retval == 0)
			{
				break;
			}

			retval = recvn(ptr->sock, ptr->buf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("recv()");
				break;
			}
			else if (retval == 0)
			{
				break;
			}

			// 서버와 클라이언트가 약속한 프로토콜을 변수에 셋팅후 switch문에서 검사한다.
			PROTOCOL protocol;
			memcpy(&protocol, ptr->buf, sizeof(PROTOCOL));

			_UserInfo info;

			// 게임의 결과값 메시지를 서식에 맞게 채워주기 위한 문자열
			char Correct_Msg[BUFSIZE];		// 정답을 맞췄을떄
			char Incorrect_Msg[BUFSIZE];	// 정답을 틀렸을때
			char GameEnd_Msg[BUFSIZE];		// 게임을 종료 했을때

			int Answer[ARRAY_SIZE];		// 서버에서 정해주는 정답을 저장할 배열 변수
			int Input[ARRAY_SIZE];		// 클라이언트에서 입력한 값을 저장할 배열 변수

			int strike = 0;			// 숫자와 위치를 맞췄을때 증가
			int ball = 0;			// 숫자는 맞았는데 위치가 틀렸을때 증가
			int out = 0;			// 숫자, 위치 둘다 틀렸을때 증가

			int result;

			int tryCount = 0;		// 정답을 맞추기 위해 몇번 입력했는지 체크하는 변수
			bool isEndGame = false;	// 정답을 맞췄을때 루프를 빠져나오기 위한 변수

			switch (protocol)
			{
			case JOIN:		// 회원가입
				ZeroMemory(&info, sizeof(_UserInfo));
				UnPacking(ptr->buf, info.id, info.pw);
				result = JoinProcess(info);
				switch (result)
				{
				case ID_EXIST:
					size = Packing(ptr->buf, JOIN_RESULT, ID_EXIST, ID_EXIST_MSG);
					break;
				case JOIN_SUCCESS:
					FileSave(&info);
					size = Packing(ptr->buf, JOIN_RESULT, JOIN_SUCCESS, JOIN_SUCCESS_MSG);
					break;
				}
				break;
			case LOGIN:		// 로그인
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
			case GAME_START:	// 게임시작
				SettingAnswer(ARRAY_SIZE, Answer);	// 랜덤한 값으로 정답 셋팅
				printf("[Server] Answer : %d %d %d\n", Answer[0], Answer[1], Answer[2]);	// 서버측 콘솔창에서 정답을 확인하기 위해서..

				++GamePlayCount;
				// 처음에 인트로 메시지를 보낼때 클라측에선 프로토콜을 먼저 받고 메시지를 받게끔 해서 본인이 선택한 프로토콜을 다시 보내주게끔 설정
				size = size = Packing(ptr->buf, GAME_START, GAME_INTRO_MSG);	

				// 인트로 메시지 보내기
				retval = send(ptr->sock, ptr->buf, size, 0);
				if (retval == SOCKET_ERROR)
				{
					err_display("send()");
					break;
				}

				// 변수 초기화
				tryCount = 0;
				isEndGame = false;

				while (1)
				{
					// 클라이언트의 3개의 int값을 받는다.
					retval = recvn(ptr->sock, (char*)&size, sizeof(int), 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					else if (retval == 0)
						break;

					retval = recvn(ptr->sock, ptr->buf, size, 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("recv()");
						break;
					}
					else if (retval == 0)
						break;

					// 들어온 값을 언패킹해서 변수에 저장한다.
					UnPacking(ptr->buf, &Input[0], &Input[1], &Input[2]);
					// 서버 콘솔에서 클라이언트의 입력값을 보기 위해서 printf문으로 출력
					printf("[Client] Input : %d %d %d\n", Input[0], Input[1], Input[2]);

					// 클라이언트에서 보낸 입력값이 지정된 범위 내의 값인지 체크
					if (Game_Input_Value_Error(ARRAY_SIZE, Input))
					{
						// 범위 바깥의 값이라면 Game_Result 프로토콜을 동일하게 보내지만 Input_Error라는 결과값을 통해 에러메시지만 출력하게끔 설정
						size = Packing(ptr->buf, GAME_RESULT, INPUT_ERROR, INPUT_VALUE_MSG);
						retval = send(ptr->sock, ptr->buf, size, 0);
						if (retval == SOCKET_ERROR)
						{
							err_display("send()");
							break;
						}
						break;
					}
					// 클라이언트에서 보낸 입력값이 중복된 값이 있는지 체크
					if (Game_Input_OverLap_Error(Input))
					{
						// 중복된 값이 있다면 Game_Result 프로토콜을 동일하게 보내지만 Input_Error라는 결과값을 통해 에러메시지만 출력하게끔 설정
						size = Packing(ptr->buf, GAME_RESULT, INPUT_ERROR, INPUT_OVERLAP_MSG);
						retval = send(ptr->sock, ptr->buf, size, 0);
						if (retval == SOCKET_ERROR)
						{
							err_display("send()");
							break;
						}
						break;
					}
					// 위의 조건을 통과했다면 게임로직의 아웃풋전용 값인 strike, ball, out에 결과값 설정
					// 또한 result값에 Game_result의 correct, incorrect를 넘겨줘서 클라이언트 쪽에서 해당 결과값에 맞는 행동을 하도록 설정
					result = BaseBallGame(ARRAY_SIZE, Answer, Input, strike, ball, out);
					
					++tryCount;	// 게임기회 1만큼 상승
					switch (result)
					{
					case CORRECT:
						// 정답을 맞췄다면 wsprintf를 통해 서식에 맞게 문자열을 만든후
						wsprintf(Correct_Msg, "축하합니다. 정답입니다.\n시도횟수 : %d\n", tryCount);

						// 후에 게임을 종료할때 최대로 사용한 횟수, 최소로 사용한 횟수등을 체크할 변수를 셋팅해주고
						if (GamePlayCount == 1)
						{
							fast = tryCount;
							last = tryCount;
						}
						else
						{
							fast = tryCount <= fast ? tryCount : fast;
							last = tryCount >= last ? tryCount : last;
						}
						// Packing시 새로만든 문자열과 GAME_RESULT 프로토콜에 결과값으론 CORRECT를 넘겨서 클라쪽에서 처리할수 있게끔 설정
						size = Packing(ptr->buf, GAME_RESULT, CORRECT, Correct_Msg);
						isEndGame = true;
						break;
					case INCORRECT:
						// 만약 입력값이 정답이 아니라면 서식에 맞춰서 위의 게임로직함수에서 설정해준 strike, ball, out값을 넣어서 문자열을 만든후
						wsprintf(Incorrect_Msg, "%d Strike   %d Ball   %d Out\n", strike, ball, out);
						// Packing할때 넘겨서 클라쪽에 출력할 수 있게끔 설정
						size = Packing(ptr->buf, GAME_RESULT, INCORRECT, Incorrect_Msg);
						break;
					}

					if (isEndGame)	// 정답을 맞췄을때 이 조건을 통해 루프문을 탈출할 수 있게끔 설정
						break;

					// 만약 루프문을 통과하지 못한경우 INCORRECT에서 설정한 문자열을 보내준다.
					// 따라서 정답을 맞출경우 이 send는 실행되지 않는다.
					retval = send(ptr->sock, ptr->buf, size, 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
				}
				break;
			case GAME_END:
				// 게임종료를 선택한경우 서식에 맞게 게임 진행 횟수, 가장 빠르게 맞춘 횟수, 가장 늦게 맞춘 횟수를 서식에 맞게
				// 문자열로 만들어 Packing해서 보내준다.
				wsprintf(GameEnd_Msg, "진행한 게임 횟수 : %d\t가장 빠르게 맞춘 횟수 : %d\t가장 늦게 맞춘 횟수 : %d\n",
					GamePlayCount, fast, last);
				size = Packing(ptr->buf, GAME_END, GameEnd_Msg);
				break;
			}

			// 만약 게임의 정답을 맞춘경우나, 게임종료를 선택한경우는 문자열을 여기서 보내준다.
			retval = send(ptr->sock, ptr->buf, size, 0);
			if (retval == SOCKET_ERROR)
			{
				err_display("send()");
				break;
			}
		}

		ReMoveClientInfo(ptr);
	}

	closesocket(listen_sock);

	WSACleanup();

	return 0;
}

#pragma region ClientInfo_Function	클라이언트 생성후 셋팅 및 삭제
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

#pragma region Login_Function	회원가입, 로그인 프로세스
int JoinProcess(_UserInfo info)
{
	for (int i = 0; i < JoinCount; i++)
	{
		if (!strcmp(UserInfo[i]->id, info.id))
		{
			return ID_EXIST;
		}
	}

	UserInfo[JoinCount] = new _UserInfo(info);
	JoinCount++;
	return JOIN_SUCCESS;
}

int LoginProcess(_UserInfo info)
{

	for (int i = 0; i < JoinCount; i++)
	{
		if (!strcmp(UserInfo[i]->id, info.id))
		{
			if (!strcmp(UserInfo[i]->pw, info.pw))
			{
				return LOGIN_SUCCESS;
			}
			else
			{
				return PW_ERROR;
			}
		}
	}

	return ID_ERROR;
}
#pragma endregion

#pragma region Packing_Function	패킹 과정 함수
int Packing(char* _buf, PROTOCOL _protocol, int _result, const char* str1)
{
	char* ptr = _buf + sizeof(int);
	int size = 0;

	memcpy(ptr, &_protocol, sizeof(PROTOCOL));
	size = size + sizeof(PROTOCOL);
	ptr = ptr + sizeof(PROTOCOL);

	memcpy(ptr, &_result, sizeof(int));
	size = size + sizeof(int);
	ptr = ptr + sizeof(int);

	int strsize1 = strlen(str1);
	memcpy(ptr, &strsize1, sizeof(int));
	size = size + sizeof(int);
	ptr = ptr + sizeof(int);

	memcpy(ptr, str1, strsize1);
	size = size + strsize1;
	ptr = ptr + strsize1;

	ptr = _buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}

int Packing(char* _buf, PROTOCOL _protocol, const char* str1)
{
	char* ptr = _buf + sizeof(int);
	int size = 0;

	memcpy(ptr, &_protocol, sizeof(PROTOCOL));
	size = size + sizeof(PROTOCOL);
	ptr = ptr + sizeof(PROTOCOL);

	int strsize1 = strlen(str1);
	memcpy(ptr, &strsize1, sizeof(int));
	size = size + sizeof(int);
	ptr = ptr + sizeof(int);

	memcpy(ptr, str1, strsize1);
	size = size + strsize1;
	ptr = ptr + strsize1;

	ptr = _buf;
	memcpy(ptr, &size, sizeof(int));

	return size + sizeof(int);
}

void  UnPacking(const char* _buf, char* str1, char* str2)
{
	const char* ptr = _buf + sizeof(PROTOCOL);
	int strsize1;
	int strsize2;

	memcpy(&strsize1, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(str1, ptr, strsize1);
	ptr = ptr + strsize1;

	memcpy(&strsize2, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(str2, ptr, strsize2);
	ptr = ptr + strsize2;
}

void  UnPacking(const char* _buf, int* _a, int* _b, int* _c)
{
	const char* ptr = _buf + sizeof(PROTOCOL);

	memcpy(_a, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(_b, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

	memcpy(_c, ptr, sizeof(int));
	ptr = ptr + sizeof(int);

}
#pragma endregion

#pragma region File_Function	파일 Load, Save함수
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
void FileSave(_UserInfo* info)
{
	FILE* fp = nullptr;
	fp = fopen(FILE_NAME, "ab");	// 덮어쓰기가 아닌 추가로 씌어지기때문에 "ab"옵션 사용
	if (fp == NULL)
	{
		err_quit("파일저장실패");
	}

	int retval = fwrite(info, 1, sizeof(_UserInfo), fp);
	if (retval != sizeof(_UserInfo))
	{
		err_quit("파일저장실패");
	}
	fclose(fp);
}
#pragma endregion

#pragma region GameLogic_Function	게임로직 함수
bool Game_Input_OverLap_Error(int* Input)
{
	return Input[0] == Input[1] || Input[1] == Input[2] || Input[0] == Input[2];
}
bool Game_Input_Value_Error(int arrsize, int* Input)
{
	bool temp = false;
	for (int i = 0; i < arrsize; i++)
	{
		if (Input[i] < 1 || Input[i] > 9)
		{
			temp = true;
			break;
		}
	}

	return temp;
}

int BaseBallGame(int arrsize, const int* Answer, const int* Input, int& s, int& b, int& o)
{
	s = 0;
	b = 0;
	o = 0;

	for (int i = 0; i < arrsize; i++)
	{
		for (int j = 0; j < arrsize; j++)
		{
			if (Answer[i] == Input[j])
			{
				if (i == j)
					s += 1;
				else
					b += 1;
			}
		}
	}

	o = ARRAY_SIZE - (s + b);
	if (s == ARRAY_SIZE)
		return CORRECT;
	return INCORRECT;
}
void SettingAnswer(int arrsize, int* Answer)
{
	srand(time(NULL));

	for (int i = 0; i < arrsize; i++)
	{
		Answer[i] = rand() % 9 + 1;
		for (int j = 0; j < arrsize; j++)
		{
			if (Answer[i] == Answer[j] && i != j)
				--i;
		}
	}

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