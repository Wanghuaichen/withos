#include "stringutils.h"

void int2str(char *str, unsigned int n)
{
	char *p = str;
	while(n){
		*str++ = '0' + n % 10;
		n /= 10;
	}
	*str = '\0';
	--str;
	while(str > p){
		*p ^= *str;
		*str ^= *p;
		*p ^= *str;
		--str;++p;
	}
	if(!n){
			*p++ = '0';
			*p = 0;
	}
}