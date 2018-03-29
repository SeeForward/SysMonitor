#include "string_util.h"


bool IsNumber(const char* cp) 
{
	if (!cp) {
		return false;
	}

	while (*cp) 
	{
		if(*cp < '0' || *cp > '9') {
			return false;
		}
		++cp;
	}
	return true;
}

