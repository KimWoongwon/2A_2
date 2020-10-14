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

// °¢Á¾ ¸Ş½ÃÁö
#define ID_ERROR_MSG "¾ø´Â ¾ÆÀÌµğÀÔ´Ï´Ù\n"
#define PW_ERROR_MSG "ÆĞ½º¿öµå°¡ Æ²·È½À´Ï´Ù\n"
#define INPUT_OVERLAP_MSG "Áßº¹µÈ ÀÔ·ÂÀÔ´Ï´Ù. Áßº¹µÇÁö¾Ê°Ô ÀÔ·ÂÇØÁÖ¼¼¿ä.\n"
#define INPUT_VALUE_MSG "¹üÀ§¹Û ÀÔ·ÂÀÔ´Ï´Ù. (1~9)¹üÀ§ÀÇ ¼ıÀÚ¸¦ ÀÔ·ÂÇØÁÖ¼¼¿ä.\n"

#define LOGIN_SUCCESS_MSG "·Î±×ÀÎ¿¡ ¼º°øÇß½À´Ï´Ù.\n"
#define JOIN_SUCCESS_MSG "°¡ÀÔ¿¡ ¼º°øÇß½À´Ï´Ù.\n"
#define ID_EXIST_MSG "ÀÌ¹ÌÀÖ´Â ¾ÆÀÌµğÀÔ´Ï´Ù.\n"
#define GAME_INTRO_MSG "<< ¼ıÀÚ¾ß±¸ °ÔÀÓ >>\n1~9 »çÀÌÀÇ ¼ıÀÚ¸¦ Áßº¹µÇÁö¾Ê°Ô 3°³ ÀÔ·ÂÇØÁÖ¼¼¿ä.\n"

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
	GAME_START,	// °ÔÀÓ½ÃÀÛ ¸Ş´º
	GAME_END,	// °ÔÀÓÁ¾·á ¸Ş´º

	LOGIN_RESULT,
	JOIN_RESULT,
	GAME_RESULT	// °ÔÀÓÁøÇàÁß¿¡ ³ª¿À´Â °á°ú°ªÀ» Å¬¶ó¿¡°Ô ³Ñ°ÜÁÙ¶§ ÇØ´ç ÇÁ·ÎÅäÄİ·Î ¼³Á¤
};

enum RES_LOGIN	// ·Î±×ÀÎ½Ã °á°ú
{
	ID_ERROR = 1,
	PW_ERROR,
	LOGIN_SUCCESS
};

enum RES_JOIN	// È¸¿ø°¡ÀÔ½Ã °á°ú
{
	JOIN_SUCCESS = 1,
	ID_EXIST
};

enum RES_GAME	// °ÔÀÓ ÁøÇàÁß ³ª¿À´Â °á°ú
{
	INPUT_ERROR = -1,	// ÀÔ·Â ¿À·ù¸¦ Ã³¸®ÇÏ±â À§ÇÑ °á°ú

	CORRECT = 1,		// Á¤´äÀ» ¸Â­ŸÀ»½Ã °á°ú
	INCORRECT,			// Á¤´äÀ» ¸ÂÃßÁö ¸øÇßÀ»½Ã °á°ú
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