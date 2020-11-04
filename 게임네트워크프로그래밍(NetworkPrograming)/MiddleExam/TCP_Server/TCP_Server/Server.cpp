#pragma comment(lib, "ws2_32.lib")
#include "Global.h"

_ClientInfo* ClientInfo[PLAYER_COUNT];
int Count = 0;

_UserInfo* UserInfo[JOIN_USER_COUNT];
int JoinCount = 0;

_ClientInfo* AddClientInfo(SOCKET sock, SOCKADDR_IN addr);		// Ŭ���̾�Ʈ ���� �Լ�
void ReMoveClientInfo(_ClientInfo* ptr);						// Ŭ���̾�Ʈ ���� �Լ�

int recvn(SOCKET sock, char* buf, int len, int flags);			// ���� ���� recv�Լ�

int JoinProcess(_UserInfo info);		// ȸ������ ���� �Լ�
int LoginProcess(_UserInfo info);		// �α��� ���� �Լ�

_UserInfo* SearchUserInfo(const char* _Id);	// ��������Ʈ���� ID�� �´� ���� ã�� �Լ�

int Packing(char* _buf, PROTOCOL _protocol, int _result, const char* str1);	// �������ݰ� �����, �޼����� ��ŷ�ϴ� ���� �Լ�
int Packing(char* _buf, PROTOCOL _protocol, const char* str1);				// �������ݰ� �޽����� ��ŷ�ϴ� ���� �Լ�
void UnPacking(const char* _buf, char* str1, char* str2);					// Ŭ���̾�Ʈ���� ���� ID�� PW�� ����ŷ�ϴ� �Լ�
void  UnPacking(const char* _buf, int* _a, int* _b, int* _c);				// Ŭ���̾�Ʈ���� ���� �߱����ӿ� �ʿ��� int�� ����ŷ �ϴ� �Լ�

void FileLoad();					// ���Ͽ��� ���������� �ҷ����� �Լ�
void FileSave(_UserInfo* info);		// ���Ͽ� ���������� �߰� �ۼ��ϴ� �Լ�

bool Game_Input_OverLap_Error(int* Input);				// Ŭ���̾�Ʈ���� ���� ���ڰ� �ߺ����� �ʴ��� üũ�ϴ� �Լ�
bool Game_Input_Value_Error(int arrsize, int* Input);	// Ŭ���̾�Ʈ���� ���� ���ڰ� ������ ����� �ʴ��� üũ�ϴ� �Լ�

int BaseBallGame(int arrsize, const int* Answer, const int* Input, int& s, int& b, int& o);	// �߱����� ���� �Լ�
void SettingAnswer(int arrsize, int* Answer);	// �������� ���䰪�� �����ϴ� �Լ�

int main()
{
	WSADATA  wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	FileLoad();		// ���α׷� ���۽� ������ �ҷ��� ���α׷��� ���� �Լ�.

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

		int GamePlayCount = 0;		// ������ ������ ��������� üũ�ϴ� ����
		int fast = 0;				// ���� ���� �������� ��ȸ�� ��� ������ üũ�ϱ� ���� ����
		int last = 0;				// ���� �ʰ� �������� ��ȸ�� ��� ������ üũ�ϱ� ���� ����

		while (1)
		{
			int size;

			// Ŭ���̾�Ʈ ����� ����, �α���, ���ӽ���, �������� �޴��� ���ý� �޾ƿ��� �κ�
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

			// ������ Ŭ���̾�Ʈ�� ����� ���������� ������ ������ switch������ �˻��Ѵ�.
			PROTOCOL protocol;
			memcpy(&protocol, ptr->buf, sizeof(PROTOCOL));

			_UserInfo info;

			// ������ ����� �޽����� ���Ŀ� �°� ä���ֱ� ���� ���ڿ�
			char Correct_Msg[BUFSIZE];		// ������ ��������
			char Incorrect_Msg[BUFSIZE];	// ������ Ʋ������
			char GameEnd_Msg[BUFSIZE];		// ������ ���� ������

			int Answer[ARRAY_SIZE];		// �������� �����ִ� ������ ������ �迭 ����
			int Input[ARRAY_SIZE];		// Ŭ���̾�Ʈ���� �Է��� ���� ������ �迭 ����

			int strike = 0;			// ���ڿ� ��ġ�� �������� ����
			int ball = 0;			// ���ڴ� �¾Ҵµ� ��ġ�� Ʋ������ ����
			int out = 0;			// ����, ��ġ �Ѵ� Ʋ������ ����

			int result;

			int tryCount = 0;		// ������ ���߱� ���� ��� �Է��ߴ��� üũ�ϴ� ����
			bool isEndGame = false;	// ������ �������� ������ ���������� ���� ����

			switch (protocol)
			{
			case JOIN:		// ȸ������
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
			case LOGIN:		// �α���
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
			case GAME_START:	// ���ӽ���
				SettingAnswer(ARRAY_SIZE, Answer);	// ������ ������ ���� ����
				printf("[Server] Answer : %d %d %d\n", Answer[0], Answer[1], Answer[2]);	// ������ �ܼ�â���� ������ Ȯ���ϱ� ���ؼ�..

				++GamePlayCount;
				// ó���� ��Ʈ�� �޽����� ������ Ŭ�������� ���������� ���� �ް� �޽����� �ްԲ� �ؼ� ������ ������ ���������� �ٽ� �����ְԲ� ����
				size = size = Packing(ptr->buf, GAME_START, GAME_INTRO_MSG);	

				// ��Ʈ�� �޽��� ������
				retval = send(ptr->sock, ptr->buf, size, 0);
				if (retval == SOCKET_ERROR)
				{
					err_display("send()");
					break;
				}

				// ���� �ʱ�ȭ
				tryCount = 0;
				isEndGame = false;

				while (1)
				{
					// Ŭ���̾�Ʈ�� 3���� int���� �޴´�.
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

					// ���� ���� ����ŷ�ؼ� ������ �����Ѵ�.
					UnPacking(ptr->buf, &Input[0], &Input[1], &Input[2]);
					// ���� �ֿܼ��� Ŭ���̾�Ʈ�� �Է°��� ���� ���ؼ� printf������ ���
					printf("[Client] Input : %d %d %d\n", Input[0], Input[1], Input[2]);

					// Ŭ���̾�Ʈ���� ���� �Է°��� ������ ���� ���� ������ üũ
					if (Game_Input_Value_Error(ARRAY_SIZE, Input))
					{
						// ���� �ٱ��� ���̶�� Game_Result ���������� �����ϰ� �������� Input_Error��� ������� ���� �����޽����� ����ϰԲ� ����
						size = Packing(ptr->buf, GAME_RESULT, INPUT_ERROR, INPUT_VALUE_MSG);
						retval = send(ptr->sock, ptr->buf, size, 0);
						if (retval == SOCKET_ERROR)
						{
							err_display("send()");
							break;
						}
						break;
					}
					// Ŭ���̾�Ʈ���� ���� �Է°��� �ߺ��� ���� �ִ��� üũ
					if (Game_Input_OverLap_Error(Input))
					{
						// �ߺ��� ���� �ִٸ� Game_Result ���������� �����ϰ� �������� Input_Error��� ������� ���� �����޽����� ����ϰԲ� ����
						size = Packing(ptr->buf, GAME_RESULT, INPUT_ERROR, INPUT_OVERLAP_MSG);
						retval = send(ptr->sock, ptr->buf, size, 0);
						if (retval == SOCKET_ERROR)
						{
							err_display("send()");
							break;
						}
						break;
					}
					// ���� ������ ����ߴٸ� ���ӷ����� �ƿ�ǲ���� ���� strike, ball, out�� ����� ����
					// ���� result���� Game_result�� correct, incorrect�� �Ѱ��༭ Ŭ���̾�Ʈ �ʿ��� �ش� ������� �´� �ൿ�� �ϵ��� ����
					result = BaseBallGame(ARRAY_SIZE, Answer, Input, strike, ball, out);
					
					++tryCount;	// ���ӱ�ȸ 1��ŭ ���
					switch (result)
					{
					case CORRECT:
						// ������ ����ٸ� wsprintf�� ���� ���Ŀ� �°� ���ڿ��� ������
						wsprintf(Correct_Msg, "�����մϴ�. �����Դϴ�.\n�õ�Ƚ�� : %d\n", tryCount);

						// �Ŀ� ������ �����Ҷ� �ִ�� ����� Ƚ��, �ּҷ� ����� Ƚ������ üũ�� ������ �������ְ�
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
						// Packing�� ���θ��� ���ڿ��� GAME_RESULT �������ݿ� ��������� CORRECT�� �Ѱܼ� Ŭ���ʿ��� ó���Ҽ� �ְԲ� ����
						size = Packing(ptr->buf, GAME_RESULT, CORRECT, Correct_Msg);
						isEndGame = true;
						break;
					case INCORRECT:
						// ���� �Է°��� ������ �ƴ϶�� ���Ŀ� ���缭 ���� ���ӷ����Լ����� �������� strike, ball, out���� �־ ���ڿ��� ������
						wsprintf(Incorrect_Msg, "%d Strike   %d Ball   %d Out\n", strike, ball, out);
						// Packing�Ҷ� �Ѱܼ� Ŭ���ʿ� ����� �� �ְԲ� ����
						size = Packing(ptr->buf, GAME_RESULT, INCORRECT, Incorrect_Msg);
						break;
					}

					if (isEndGame)	// ������ �������� �� ������ ���� �������� Ż���� �� �ְԲ� ����
						break;

					// ���� �������� ������� ���Ѱ�� INCORRECT���� ������ ���ڿ��� �����ش�.
					// ���� ������ ������ �� send�� ������� �ʴ´�.
					retval = send(ptr->sock, ptr->buf, size, 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}
				}
				break;
			case GAME_END:
				// �������Ḧ �����Ѱ�� ���Ŀ� �°� ���� ���� Ƚ��, ���� ������ ���� Ƚ��, ���� �ʰ� ���� Ƚ���� ���Ŀ� �°�
				// ���ڿ��� ����� Packing�ؼ� �����ش�.
				wsprintf(GameEnd_Msg, "������ ���� Ƚ�� : %d\t���� ������ ���� Ƚ�� : %d\t���� �ʰ� ���� Ƚ�� : %d\n",
					GamePlayCount, fast, last);
				size = Packing(ptr->buf, GAME_END, GameEnd_Msg);
				break;
			}

			// ���� ������ ������ �����쳪, �������Ḧ �����Ѱ��� ���ڿ��� ���⼭ �����ش�.
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

#pragma region ClientInfo_Function	Ŭ���̾�Ʈ ������ ���� �� ����
_ClientInfo* AddClientInfo(SOCKET sock, SOCKADDR_IN addr)
{
	printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
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
	printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
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

#pragma region Login_Function	ȸ������, �α��� ���μ���
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

#pragma region Packing_Function	��ŷ ���� �Լ�
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

#pragma region File_Function	���� Load, Save�Լ�
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
	fp = fopen(FILE_NAME, "ab");	// ����Ⱑ �ƴ� �߰��� �������⶧���� "ab"�ɼ� ���
	if (fp == NULL)
	{
		err_quit("�����������");
	}

	int retval = fwrite(info, 1, sizeof(_UserInfo), fp);
	if (retval != sizeof(_UserInfo))
	{
		err_quit("�����������");
	}
	fclose(fp);
}
#pragma endregion

#pragma region GameLogic_Function	���ӷ��� �Լ�
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