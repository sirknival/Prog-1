#include "dbfunc.h"


#define NUM_ENTRY 10
#define FILE_NAME_LEN 50
#define SPACER_H "====================================="

int main(int argc, char* argv[])
{
    long arg_i = 0;
    long menu_ctrl = 0;
    long number_param = 0;
    long case_offset = 0;
    int flag_terminate = 0;
    char keyword[TEXT_LEN];
    char file_name[FILE_NAME_LEN];
    Data_t inventory_db[NUM_ENTRY] = {0};

    if (argc > 2){
        if(strlen(argv[1])>TEXT_LEN)
            fprintf(stderr, "ERROR: Keyword %s exceeds max. length of %i characters.\n", argv[1], TEXT_LEN-1);
        else{
            if(strcmp(argv[1], "-i")==0)
                case_offset = 1;
            strcpy(keyword, argv[1+case_offset]);
            arg_i += 2 + case_offset;
            for(; arg_i < argc; arg_i++){
                if(strlen(argv[arg_i])>FILE_NAME_LEN){
                    fprintf(stderr, "ERROR: Filename %s exceeds max. length of %i characters.\n", argv[arg_i], FILE_NAME_LEN-1);
                    continue;
                }
                if(searchFile(argv[arg_i], keyword, case_offset)==-1)
                    fprintf(stderr,"ERROR: Search was not conducted sucessfully. \n");
            }
        }
        return 0;
    }

    initItems(inventory_db, NUM_ENTRY);
    loadDB(inventory_db, NUM_ENTRY, "export.txt");

    printf("%s\n\t\t%s\n%s\n", SPACER_H, "Database", SPACER_H);
    printf("Limitations: \n"
           "\tEntered numbers need to be integers\n"
           "\tAllowed characters for strings are [a-zA-Z1-9_]\n"); //TODO explain limits of db

    while(!flag_terminate){
        printf("Options: \n"
           "\t1 - Print all database entries\n"
           "\t2 - Add entry\n"
           "\t3 - Delete entry\n"
           "\t4 - Export to file\n"
           "\t5 - Import from file\n"
           "\t6 - Search in file\n"
           "\t7 - Sort database\n"
           "\t8 - Add from file\n"
           "\t0 - End program\n");

        menu_ctrl = readValue("Next action");
        fflush(stdin);
        switch(menu_ctrl){
            case 0: flag_terminate = 1; break;
            case 1: printDB(inventory_db, NUM_ENTRY); break;
            case 2: addItem(inventory_db, NUM_ENTRY); break;
            case 3: deleteItem(inventory_db, NUM_ENTRY); break;
            case 4:
                readString("file", file_name, FILE_NAME_LEN);
                if(saveDB(inventory_db, NUM_ENTRY, file_name)==-1)
                    fprintf(stderr, "ERROR: Database was not exported to file.\n");
                break;
            case 5:
                readString("file", file_name, FILE_NAME_LEN);
                if(loadDB(inventory_db, NUM_ENTRY, file_name))
                    fprintf(stderr, "ERROR: Database was not imported correctly from file.\n");
                break;
            case 6:
                readString("Keyword", keyword, TEXT_LEN);
                readString("file", file_name, FILE_NAME_LEN);
                number_param = readValue("Ignorecase No/Yes? (0|1)");
                if(searchFile(file_name, keyword, number_param)==-1)
                    fprintf(stderr,"ERROR: Search was not conducted sucessfully. \n");
                break;
            case 7:
                number_param = readValue("Sorting attribute Inv-No/Year? (1/2)");
                sortItems(inventory_db, NUM_ENTRY, number_param);
                break;
            case 8:
                readString("Keyword", keyword, TEXT_LEN);
                readString("file", file_name, FILE_NAME_LEN);
                addFromFile(inventory_db, NUM_ENTRY, file_name, keyword);
                break;
            default: fprintf(stderr, "ERROR: This Option does not exist.\n");
        }
    }
    printf("\n=================END================\n\n");
    return 0;
}
