// Generate report for Dataset and NamedRefrence count attach to ItemRevision



#include"Header.h"

int ITK_user_main(int argc, char* argv[])
{
	int iSec_obj_count = 0;
	int iNamedRefCount = 0;

	tag_t tItemRevTag = NULLTAG;
	tag_t* tSec_obj_list = NULLTAG;
	tag_t* tNamedRefList = NULLTAG;

	char* cDatasetType = NULL;
	char* cUserID = ITK_ask_cli_argument("-u=");
	char* cPassword = ITK_ask_cli_argument("-p=");
	char* cGroup = ITK_ask_cli_argument("-g=");
	char* cItemID = ITK_ask_cli_argument("-item_id=");
    //#define MAX_LINE_LENGTH 1024
	printf("123");
	FILE *fptr;
	fptr = TC_fopen("data.csv", "w+");
	if (fptr != NULL)
	{
		TC_fprintf(fptr, "Item_id,DatasetType,NamedRefCount\n");

		Report_Error(ITK_init_module(cUserID, cPassword, cGroup));
		printf("\nLogin Success\n");

		Report_Error(ITEM_find_rev(cItemID, "A", &tItemRevTag));
		printf("\nItem Revision Tag found Success\n");

		Report_Error(GRM_list_secondary_objects_only(tItemRevTag, NULLTAG, &iSec_obj_count, &tSec_obj_list));
		printf("\nSecondary object list found\n");

		for (int i = 0; i < iSec_obj_count; i++)
		{
			Report_Error(WSOM_ask_object_type2(tSec_obj_list[i], &cDatasetType));
			printf("\n%s\n", cDatasetType);
			char arr[2][10] = { "PDF", "Text" };
			for (int j = 0; j < 2; j++)
			{

				if (tc_strcmp(cDatasetType, arr[j]) == 0)
				{
					Report_Error(AE_ask_dataset_named_refs(tSec_obj_list[i], &iNamedRefCount, &tNamedRefList));

					if (iNamedRefCount > 0)
					{
						TC_fprintf(fptr, "%s,%s,%d\n", cItemID,cDatasetType ,iNamedRefCount);
					}
					else
					{
						TC_fprintf(fptr, "%s,%s,%d\n", cItemID,cDatasetType ,iNamedRefCount);
					}
				}

			}

		}
		
	}

	Report_Error( ITK_exit_module(TRUE));
	printf("\n\n Logout Success");
	fclose(fptr);
	return 0;
}
