#include "strblob.h"

int main()
{
	StrBlob sb1;
	sb1.push_back("a");
	StrBlob sb2(sb1);
	return 0;
}