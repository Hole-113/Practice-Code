
// The program reads the Item ID from the CSV file, creates a new Item in Teamcenter, generates five datasets for each item, and attaches all the datasets to the Item Revision using the IMAN_specification relation.


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<tc/tc_startup.h>
#include<tcinit/tcinit.h>
#include<tc/emh.h>
#include<tc/item.h>
#include<ctcore/workspaceobject.h>
#include<ctcore/grm.h>
#include<AE/ae.h>
#include<AE/dataset.h>
#include<TC/folder.h>
#include<SA/user.h>
#include<ctcore/aom.h>
#include<tcclasses/tc_string.h>
#include<ctcore/project.h>
#include<ctcore/workspaceobject.h>
#include<ctcore/aom_prop.h>
#include<ctcore/tctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<tcclasses/tc_string.h>
#include<ctcore/tctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_FIELD_LENGTH 256

typedef struct {
    char* cItem_ID;
    char* cItem_Type;
    char* cItem_Name;
    char* cItem_Rev;
} input_t;

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







#include "Header.h"

int ITK_user_main(int argc, char* argv[])
{
    int iFail = 0;
    int iSec_obj_count = 0;
    int iNamedRefCount = 0;
    int irow_count = 0;

    tag_t tItemRevTag = NULLTAG;
    tag_t* tSec_obj_list = NULLTAG;
    tag_t* tNamedRefList = NULLTAG;
    tag_t tClassId = NULLTAG;
    char* cClassName = NULL;
    char* cDatasetType = NULL;

    char* cUserID   = ITK_ask_cli_argument("-u=");
    char* cPassword = ITK_ask_cli_argument("-p=");
    char* cGroup    = ITK_ask_cli_argument("-g=");
    char* cInputFile = ITK_ask_cli_argument("-input_file=");

    input_t record;
    char buf[1024];
    const char delim[2] = ",";
    char* Token = NULL;
    char* cError = NULL;

    char* relation_name = "IMAN_specification";
    tag_t tItem = NULLTAG;
    tag_t tRev  = NULLTAG;
    tag_t t_relation_type = NULLTAG;
    tag_t datatype = NULLTAG;
    tag_t new_dataset = NULLTAG;
    tag_t relation = NULLTAG;

    // Array of dataset types to create
    char* dataset_types[] = {
        "UGMASTER",
        "UGPART",
        "PDF",
        "MSEXECEL",
        "MSWORD"
    };

    // Array of dataset descriptions
    char* dataset_descriptions[] = {
        "UGMASTER dataset description",
        "UGPART dataset description",
        "PDF dataset description",
        "MSEXECEL dataset description",
        "MSWORD dataset description"
    };

    int num_datasets = 5;
    int i = 0;

    FILE* inputFile = TC_fopen(cInputFile, "r");
    if (!inputFile)
    {
        printf("Cant open file\n");
        exit(1);
    }
    else
    {
        Report_Error(ITK_init_module(cUserID, cPassword, cGroup));
        printf("\nLogin Success\n");

        while (fgets(buf, 1024, inputFile))
        {
            irow_count++;
            if (irow_count == 1)
                continue;

            char* cItem_ID   = strtok_s(buf, delim, &Token);
            char* cItem_Name = strtok_s(NULL, delim, &Token);
            char* cItem_Type = strtok_s(NULL, delim, &Token);
            char* cItem_Rev  = strtok_s(NULL, delim, &Token);

            record.cItem_ID = cItem_ID;
            record.cItem_Name = cItem_Name;
            record.cItem_Type = cItem_Type;
            record.cItem_Rev = cItem_Rev;

            Report_Error(iFail = ITEM_create_item(record.cItem_ID,
                                                  record.cItem_Name,
                                                  record.cItem_Type,
                                                  record.cItem_Rev,
                                                  &tItem, &tRev));

            // Save item
            Report_Error(iFail = ITEM_save_item(tItem));
            printf("\nItem ID %s Successfully created", record.cItem_ID);

            // Get relation type IMAN_specification
            Report_Error(iFail = GRM_find_relation_type(relation_name, &t_relation_type));

            // Find item revision
            Report_Error(iFail = ITEM_find_revision(tItem, record.cItem_Rev, &tRev));

            // Loop through all dataset types
            for (i = 0; i < num_datasets; i++)
            {
                // Find dataset type
                Report_Error(iFail = AE_find_datasettype2(dataset_types[i], &datatype));

                // Create dataset
                Report_Error(iFail = AE_create_dataset_with_id(datatype,
                                                               record.cItem_ID,
                                                               dataset_descriptions[i],
                                                               "",
                                                               "",
                                                               &new_dataset));

                // Save dataset
                Report_Error(iFail = AE_save_myself(new_dataset));

                // Create relation between revision & dataset
                Report_Error(iFail = GRM_create_relation(tRev, new_dataset, t_relation_type,
                                                         NULLTAG, &relation));

                // Save relation
                Report_Error(iFail = GRM_save_relation(relation));

                datatype = NULLTAG;
                new_dataset = NULLTAG;
                relation = NULLTAG;
            }
        }

        printf("\n\nCompleted processing all dataset types");
        printf("\nItem creation with multiple datasets completed successfully");
        fclose(inputFile);
        return 0;
    }
}
