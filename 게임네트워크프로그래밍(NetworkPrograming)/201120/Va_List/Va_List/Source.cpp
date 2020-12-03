#include <stdio.h>
#include <stdarg.h>
#include <string.h>


// 하나의 함수 프로토 타입으로 여러 인자를 받고싶다.

/*
int sum()
sum(1, 2);
sum(1, 2, 3);
sum(1, 2, 3, 4);
*/

// 이경우 함수 오버로딩을 이용하는 방법이 있지만,
// 우리는 가변인자 리스트를 이용해서 구현해본다.

int sum(int num_args, ...);
void ErrorPrint(const char* fmt, ...);

int main()
{
	printf("%d\n", sum(5, 1, 2, 3, 4, 5));

	int a = 0;

	if(a == 0)
		ErrorPrint(" %d\n", a);
}

// va_list 는 기본적으로 포인터를 사용해서 다음 위치의 인자값을 찾아온다.
// 첫번째 인자를 고정인수 (가변인자의 갯수)로 설정한다.
int sum(int num_args, ...)	
{
	va_list ap;
	va_start(ap, num_args);	// va_start 에서 고정인수를 넣는이유는 주소를 찾기 위함이다.
	int arg = 0, result = 0;
	for (int i = 0; i < num_args; i++)
	{
		arg = va_arg(ap, int);	// va_arg va_list의 포인터를 다음가변인자로 이동시켜주는 메크로
		result += arg;
	}
	va_end(ap);	// 사용한 가변인자 변수를 끝낼때 사용하는 메크로
	return result;
}

// 
void ErrorPrint(const char* fmt, ...)
{
	char buf[512] = { 0, };
	va_list ap;

	strcpy(buf, "[ERROR] ");
	va_start(ap, fmt);
	vsprintf(buf + strlen(buf), fmt, ap);
	va_end(ap);

	puts(buf);
}