#include <string.h>


unsigned char isNum(char ch)
{
		return ch >= '0' && ch <= '9';
}
int istr2num(unsigned *res, char * str)
{
	char *head = str;	
	*res = 0;


	while(!isNum(*str)){
		if(*str == '\0'){
			return -1;
		}
		str++;
	}
	while(isNum(*str)){
		*res = 10 * (*res) + (*str++ - '0');
	}

	return str - head;

}