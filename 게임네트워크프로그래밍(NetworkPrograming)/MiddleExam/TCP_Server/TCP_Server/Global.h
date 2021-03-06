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

// 각종 메시지
#define ID_ERROR_MSG "없는 아이디입니다\n"
#define PW_ERROR_MSG "패스워드가 틀렸습니다\n"
#define INPUT_OVERLAP_MSG "중복된 입력입니다. 중복되지않게 입력해주세요.\n"
#define INPUT_VALUE_MSG "범위밖 입력입니다. (1~9)범위의 숫자를 입력해주세요.\n"

#define LOGIN_SUCCESS_MSG "로그인에 성공했습니다.\n"
#define JOIN_SUCCESS_MSG "가입에 성공했습니다.\n"
#define ID_EXIST_MSG "이미있는 아이디입니다.\n"
#define GAME_INTRO_MSG "<< 숫자야구 게임 >>\n1~9 사이의 숫자를 중복되지않게 3개 입력해주세요.\n"

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
	GAME_START,	// 게임시작 메뉴
	GAME_END,	// 게임종료 메뉴

	LOGIN_RESULT,
	JOIN_RESULT,
	GAME_RESULT	// 게임진행중에 나오는 결과값을 클라에게 넘겨줄때 해당 프로토콜로 설정
};

enum RES_LOGIN	// 로그인시 결과
{
	ID_ERROR = 1,
	PW_ERROR,
	LOGIN_SUCCESS
};

enum RES_JOIN	// 회원가입시 결과
{
	JOIN_SUCCESS = 1,
	ID_EXIST
};

enum RES_GAME	// 게임 진행중 나오는 결과
{
	INPUT_ERROR = -1,	// 입력 오류를 처리하기 위한 결과

	CORRECT = 1,		// 정답을 맞췃을시 결과
	INCORRECT,			// 정답을 맞추지 못했을시 결과
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