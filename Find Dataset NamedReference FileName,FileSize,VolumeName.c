
// Find Dataset NamedReference FileName,FileSize,VolumeName
// We want dataset files data for to compare before and after migration 


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
#include <tccore/aom_prop.h>

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



#include"Header.h"

int ITK_user_main(int argc, char* argv[])
{
	int iSec_obj_count = 0;
	int iNamedRefCount = 0;
	int ipropCount = 0;
	tag_t tItemRevTag = NULLTAG;
	tag_t* tSec_obj_list = NULLTAG;
	tag_t* tNamedRefList = NULLTAG;
	tag_t tClassId = NULLTAG;
	char* cClassName = NULL;
	char* cDatasetType = NULL;
	char* cValue = NULL;
	char* cFileSize = NULL;
	char* cVolumeName = NULL;
	char* cFileName = NULL;
	char** cpropNames = NULL;
	char* cUserID = ITK_ask_cli_argument("-u=");
	char* cPassword = ITK_ask_cli_argument("-p=");
	char* cGroup = ITK_ask_cli_argument("-g=");
	char* cItemID = ITK_ask_cli_argument("-item_id=");

	//#define MAX_LINE_LENGTH 1024
	printf("123");
	FILE* fptr;
	fptr = TC_fopen("C:\\Users\\AdminV\\Desktop\\data.csv", "w+");
	if (fptr != NULL)
	{
		TC_fprintf(fptr, "Item_id,DatasetType,FileName,FileSize,FileVolume\n");

		Report_Error(ITK_init_module(cUserID, cPassword, cGroup));
		printf("\nLogin Success\n");

		Report_Error(ITEM_find_rev(cItemID, "A", &tItemRevTag));
		printf("\nItem Revision Tag found Success\n");

		Report_Error(GRM_list_secondary_objects_only(tItemRevTag, NULLTAG, &iSec_obj_count, &tSec_obj_list));
		printf("\nSecondary object list found\n");

		for (int i = 0; i < iSec_obj_count; i++)
		{

			Report_Error(POM_class_of_instance(tSec_obj_list[i], &tClassId));
			printf("\n%d\n", tClassId);

			Report_Error(POM_name_of_class(tClassId, &cClassName));
			printf("\n%s\n", cClassName);

			if (tc_strcmp(cClassName, "Dataset") == 0)
			{

				Report_Error(WSOM_ask_object_type2(tSec_obj_list[i], &cDatasetType));
				printf("\n%s\n", cDatasetType);

				Report_Error(AE_ask_dataset_named_refs(tSec_obj_list[i], &iNamedRefCount, &tNamedRefList));
				for (int k = 0; k < iNamedRefCount; k++)
				{

					Report_Error(AOM_UIF_ask_value(tNamedRefList[k], "volume_tag", &cVolumeName));
					Report_Error(AOM_UIF_ask_value(tNamedRefList[k], "object_string", &cFileName));
					Report_Error(AOM_UIF_ask_value(tNamedRefList[k], "file_size", &cFileSize));

					TC_fprintf(fptr, "%s,%s,%s,%s,%s \n", cItemID, cDatasetType, cFileName, cFileSize, cVolumeName);

				}	

			}


		}

	}

	Report_Error(ITK_exit_module(TRUE));
	printf("\n\n Logout Success");
	fclose(fptr);
	return 0;
}
