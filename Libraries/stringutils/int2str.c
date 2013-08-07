#include "stringutils.h"

void int2str(char *str, unsigned int n)
{
	while(n){
		*str++ = '0' + n % 10;
		n /= 10;
	}
	*str = '\0';
}