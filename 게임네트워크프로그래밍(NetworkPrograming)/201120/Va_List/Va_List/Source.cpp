#include <stdio.h>
#include <stdarg.h>
#include <string.h>


// �ϳ��� �Լ� ������ Ÿ������ ���� ���ڸ� �ް�ʹ�.

/*
int sum()
sum(1, 2);
sum(1, 2, 3);
sum(1, 2, 3, 4);
*/

// �̰�� �Լ� �����ε��� �̿��ϴ� ����� ������,
// �츮�� �������� ����Ʈ�� �̿��ؼ� �����غ���.

int sum(int num_args, ...);
void ErrorPrint(const char* fmt, ...);

int main()
{
	printf("%d\n", sum(5, 1, 2, 3, 4, 5));

	int a = 0;

	if(a == 0)
		ErrorPrint(" %d\n", a);
}

// va_list �� �⺻������ �����͸� ����ؼ� ���� ��ġ�� ���ڰ��� ã�ƿ´�.
// ù��° ���ڸ� �����μ� (���������� ����)�� �����Ѵ�.
int sum(int num_args, ...)	
{
	va_list ap;
	va_start(ap, num_args);	// va_start ���� �����μ��� �ִ������� �ּҸ� ã�� �����̴�.
	int arg = 0, result = 0;
	for (int i = 0; i < num_args; i++)
	{
		arg = va_arg(ap, int);	// va_arg va_list�� �����͸� �����������ڷ� �̵������ִ� ��ũ��
		result += arg;
	}
	va_end(ap);	// ����� �������� ������ ������ ����ϴ� ��ũ��
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