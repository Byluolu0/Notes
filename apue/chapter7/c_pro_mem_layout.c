#include <stdio.h>
#include <stdlib.h>

static const int a = 10;
static int car = 10;

int main()
{
	printf("%p %p\n", &a, &car);

	char str1[] = "abcd";
	char *str2 = "abcd";
	printf("%p %p\n", str1, &str1);
	printf("%p %p\n", str2, &str2);

	const char *str3 = "abcd";
	printf("%p %p\n", str3, &str3);

	char * const str4 = "abcd";
	printf("%p %p\n", str4, &str4);

	const char * const str5 = "abcd";
	printf("%p %p\n", str5, &str5);
	return 0;
}
