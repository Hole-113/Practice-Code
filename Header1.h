
#include<stdio.h>
#include<stdlib.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>
#include<tccore/item.h>
#include<sa/user.h>
#include<tc/folder.h>
#include<tccore/aom.h>
#include<tccore/grm.h>
#include<ae/dataset.h>
#include<fclasses/tc_string.h>
#include<tccore/tctype.h>
int Report_Error(int iFail)
{
	char* cError = NULL;
	if (iFail != ITK_ok)
	{
		EMH_ask_error_text(iFail, &cError);
		printf("\n\n Error is : %s", cError);
		MEM_free(cError);
		exit(0);
	}
	return 0;
}

