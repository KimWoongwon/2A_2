#pragma once
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SERVERPORT 9000
#define BUFSIZE 512
#define PLAYER_COUNT 1
#define JOIN_USER_COUNT 100
#define ARRAY_SIZE 3
#define FILE_NAME "UserList.dat"

// ���� �޽���
#define ID_ERROR_MSG "���� ���̵��Դϴ�\n"
#define PW_ERROR_MSG "�н����尡 Ʋ�Ƚ��ϴ�\n"
#define INPUT_OVERLAP_MSG "�ߺ��� �Է��Դϴ�. �ߺ������ʰ� �Է����ּ���.\n"
#define INPUT_VALUE_MSG "������ �Է��Դϴ�. (1~9)������ ���ڸ� �Է����ּ���.\n"

#define LOGIN_SUCCESS_MSG "�α��ο� �����߽��ϴ�.\n"
#define JOIN_SUCCESS_MSG "���Կ� �����߽��ϴ�.\n"
#define ID_EXIST_MSG "�̹��ִ� ���̵��Դϴ�.\n"
#define GAME_INTRO_MSG "<< ���ھ߱� ���� >>\n1~9 ������ ���ڸ� �ߺ������ʰ� 3�� �Է����ּ���.\n"

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
	JOIN = 1,
	LOGIN,
	EXIT,
	GAME_START,	// ���ӽ��� �޴�
	GAME_END,	// �������� �޴�

	LOGIN_RESULT,
	JOIN_RESULT,
	GAME_RESULT	// ���������߿� ������ ������� Ŭ�󿡰� �Ѱ��ٶ� �ش� �������ݷ� ����
};

enum RES_LOGIN	// �α��ν� ���
{
	ID_ERROR = 1,
	PW_ERROR,
	LOGIN_SUCCESS
};

enum RES_JOIN	// ȸ�����Խ� ���
{
	JOIN_SUCCESS = 1,
	ID_EXIST
};

enum RES_GAME	// ���� ������ ������ ���
{
	INPUT_ERROR = -1,	// �Է� ������ ó���ϱ� ���� ���

	CORRECT = 1,		// ������ �­����� ���
	INCORRECT,			// ������ ������ �������� ���
};

struct _UserInfo
{
	char id[255];
	char pw[255];

	_UserInfo() {}
	_UserInfo(const _UserInfo& info)
	{
		strcpy(id, info.id);
		strcpy(pw, info.pw);
	}
};

struct _ClientInfo
{
	SOCKET sock;
	SOCKADDR_IN addr;
	_UserInfo* info;
	bool login;

	char buf[BUFSIZE];
};