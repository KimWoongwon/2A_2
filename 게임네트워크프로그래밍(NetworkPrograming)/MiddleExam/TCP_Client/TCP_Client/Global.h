#pragma once
#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512
#define ARRAY_SIZE 3

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
	GAME_START,
	GAME_END,

	LOGIN_RESULT,
	JOIN_RESULT,
	GAME_RESULT
};

enum RES_LOGIN
{
	ID_ERROR = 1,
	PW_ERROR,
	LOGIN_SUCCESS
};

enum RES_JOIN
{
	JOIN_SUCCESS = 1,
	ID_EXIST
};

enum RES_GAME
{
	INPUT_ERROR = -1,

	CORRECT = 1,
	INCORRECT,
	INTRO
};

struct _UserInfo
{
	char id[255];
	char pw[255];

	_UserInfo(const _UserInfo& info)
	{
		strcpy(id, info.id);
		strcpy(pw, info.pw);
	}
	_UserInfo()
	{

	}
};