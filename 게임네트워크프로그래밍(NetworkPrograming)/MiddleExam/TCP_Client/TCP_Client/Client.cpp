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
			printf("<<�޴�>>\n");
			printf("1.����\n");
			printf("2.�α���\n");
			printf("3.����\n");
			printf("����:");
			scanf("%d", &select);
			if (select < 1 || select > 3)		// ������ ���� �ƴҰ�� ���� ó��
			{
				printf("�߸��� �Է��Դϴ�.\n\n");
				getchar();		// ���ڿ� �Է½� �Է¹��� �����ϱ� ���ؼ�
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

		// �α���, ȸ�����Խ� ID, PW �� �����ִ� �κ�
		retval = send(sock, buf, size, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}

		// Login_Result, Join_Result �� �ش��ϴ� ���������� �޾ƿ������� �κ�
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

		// �޾ƿ� ��Ŷ���� �������� �и�
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
			UnPacking(buf, result, msg);	// �޾ƿ� ��Ŷ���� �������� �ʸ��� ������� �޾ƿ���
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
						printf("<<���ھ߱� ����>>\n");
						printf("1.���� ����\n");
						printf("2.����\n");
						printf("����:");
						scanf("%d", &select);
						if (select < 1 || select > 2)		// ����ó�� ���ڿ��Է½� �Է¹��� ���� ����
						{
							printf("�߸��� �Է��Դϴ�.\n\n");
							getchar();
						}
					} while (select < 1 || select > 2);
					
					select = select == 1 ? GAME_START : GAME_END;	// �Է°��̶� enum�� ���� �ٸ��⶧���� ó���ϴ� �κ�

					size = Packing(buf, (PROTOCOL)select);		// ������ ���� packing�ؼ� �����ֱ�
					retval = send(sock, buf, size, 0);
					if (retval == SOCKET_ERROR)
					{
						err_display("send()");
						break;
					}

					// ��Ʈ�� �޽��� �ޱ�
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

					PROTOCOL protocol;		// GAME_START, GAME_END ���� �����ϱ� ���� protocol
					memcpy(&protocol, buf, sizeof(PROTOCOL));

					switch (protocol)
					{
					case GAME_START:
						ZeroMemory(msg, sizeof(msg));
						UnPacking(buf, msg);		// �������� �ʸ��� GAME_RESULT�� �޾ƿ���
						printf("%s\n", msg);

						isEndGame = false;
						while (!isEndGame)
						{
							int input[3];
							for (int i = 0; i < ARRAY_SIZE;i++)
							{
								printf("%d��° ���� : ", i + 1);
								scanf("%d", &input[i]);
							}
							// Packing�Ҷ� ���� ������ ���� Protocol�ε� �̺κп��� ������ ���������� ������� �ʱ� ������ NULL������ ����
							size = Packing(buf, (PROTOCOL)NULL, input, 3);	// ���ο��� Input �迭�� ��Ŷ�� ����־� �ش�.
							retval = send(sock, buf, size, 0);				// packing�� buf�� �����ش�.
							if (retval == SOCKET_ERROR)
							{
								err_display("send()");
								break;
							}
							// buf�� ù�κ��� ������ ���̱⶧���� ��� �޴´�. �Ŀ� �ؿ��� ����Ѵ�.
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
							// ���ӷ����� ���� ���� GAME_RESULT������ ������� �޾ƿ´�.
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

							// ������� �޽����� buf���� �����ؿ´�.
							// switch������ ó���Ѵ�.
							ZeroMemory(msg, sizeof(msg));
							UnPacking(buf, result, msg);
							switch (result)
							{
							case CORRECT:
								printf("%s\n", msg);
								isEndGame = true;
								break;
							case INPUT_ERROR:		// input, incorrect ��� ó���� ������� �޽����� ������ָ� �ȴ�.
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

