#include "dbfunc.h"

void initItems(Data_t *items, long len){
    for (long i = 0; i< len; i++){
        strcpy((items+i)->invNr, "");
    }
    return;
}

void printItem(Data_t *item){
    printf("  Name: \t\t%s\n", item->name);
    printf("  Brand: \t\t%s\n", item->brand);
    printf("  Inv-No: \t\t%s\n", item->invNr);
    printf("  Year of purchase: \t%ld\n", item->year);
    return;
}

void printDB(Data_t *items, long len){
    printf("All valid entries: \n");
    for (long i = 0; i< len; i++){
        if (strcmp((items+i)->invNr, "") != 0){
            printf("---------------ENTRY %ld---------------\n", i);
            printItem(items+i);
        }
    }
    printf("-------------------------------------\n");
    return;
}

long readValue(char *name) {
    long int_val = 0;
    int flag_error = 0;

    while(1) {
        fprintf(stdout, ">>%-*s: ", 10, name);
        flag_error = scanf("%ld", &int_val);
        fflush(stdin);

        if(flag_error == 1)
            break;
        fprintf(stderr, "ERROR: Nnvalid input for %s. \n", name);
    }
    return int_val;
}

void readString(char *name, char *text, long len){
    long length = 0;
    char temp_char = ' ';
    fprintf(stdout, ">>%-*s: ", 10, name);
    temp_char = getchar();
    while (temp_char != '\n' && length +1 < len){
        if(!isalnum(temp_char) && temp_char !='_' && temp_char !='.'){
            fprintf(stderr, "WARNING: Invalid char found. \"%c\" will be replaced with \"_\"\n", temp_char);
            temp_char = '_';
        }
        *(text+length) = temp_char;
        length++;
        temp_char = getchar();
    }
    *(text+length) = '\0';
    fflush(stdin);
    return;
}

long addItem(Data_t *items, long len){
    //Gets first free position
    long index = h_first_valid_index(items, len);
    items+=index;
    //Checks if DB is already full
    if (index == len){
        fprintf(stderr, "ERROR: Database is already full\n");
        return -1;
    }
    //Otherwise save entered data
    else{
        printf("------------ADD NEW ENTRY------------\n");
        readString("Inv-No", items->invNr, TEXT_LEN);
        //If InvNr-String is empty, error will be raised and rest skipped
        if(strncmp(items->invNr, "", 1)==0){
            fprintf(stderr,"ERROR: Invalid inventory id\n");
            return -1;
        }
        //Read and store name, brand and year
        readString("Name", items->name, TEXT_LEN);
        readString("Brand", items->brand, TEXT_LEN);
        items->year = readValue("Year");
    }
    printf("-------------------------------------\n");
    return 0;
}

long deleteItem(Data_t *items, long len) {
    long index = 0;
    printf("------------REMOVE ENTRY-------------\n");
    printf("Please enter the index of the concerning item\n");
    index = readValue("Index");
    items += index;
    //Check if index exceeds the array
    if (index >= len){
        fprintf(stderr, "ERROR: Index of out range.\n");
        return -1;
    }
    //Check if a valid entry is at stated index
    else if (strncmp(items->invNr, "", 1)== 0){
        fprintf(stderr, "ERROR: no valid database entry at this index\n");
        return -1;
    }
    //Reset invNr of entry and remove gaps
    while(strncmp((items+1)->invNr, "", 1)){
        memmove(items, (items+1), sizeof(Data_t));
        items++;
    }
    strcpy(items->invNr, "");
    printf("-------------------------------------\n");
    return 0;
}

long saveDB(Data_t *items, long len, char *output_file){
    FILE *file;
    if((file = fopen(output_file, "w"))){
        fprintf(file, "%-*s%-*s%-*s%-*s\n", TEXT_LEN, "#Name", TEXT_LEN, "#Brand",TEXT_LEN, "#Inv-No.", TEXT_LEN, "#Year");
        while(strncmp((items)->invNr, "", 1)){
            fprintf(file, "%-*s%-*s%-*s%-*ld\n", TEXT_LEN, items->name, TEXT_LEN, items->brand, TEXT_LEN, items->invNr,TEXT_LEN, items->year);
            items++;
        }
        fclose(file);
    }
    else {
        fprintf(stderr, "ERROR: Can't open file\n");
        return -1;
    }
    return 0;
}

long loadDB(Data_t *items, long len, char * input_file){
    FILE *file;
    Data_t *db_start = items;
    char file_snippet[LINE_LEN];
    long line = 0;
    int error_flag = 0;
    long parameter = 0;
    char delimiter[] = " ";
    char *ptr_token;

    //Clears old entries from DB
    initItems(items, len);

    //Check if file can be successfully opened
    if((file = fopen(input_file, "r"))){
        //Check if file is empty
        if(fgets(file_snippet, LINE_LEN, file) == NULL){
            //Empty file - raise error
            fprintf(stderr, "ERROR: Empty file\n");
            return -1;
        }
        //For every line until the end of the array items is reached
        while(line < len && error_flag == 0){
            //Read line and if empty return; Whole file has been read
            if(fgets(file_snippet, LINE_LEN, file) == NULL)
                return 0;
            //Set splitting pointer to position
            ptr_token = strtok(file_snippet, delimiter);
            parameter = 0;
            //While pointer references a valid string
            while(ptr_token != NULL && error_flag == 0){
                //End of line reached, break
                if (strncmp(ptr_token, "\n", 1) == 0)
                    break;
                //Check if string is not longer than allowed
                if(strlen(ptr_token) >= TEXT_LEN){
                    fprintf(stderr, "ERROR: File %s, line %ld, param %ld"
                            " - Entry exceeds allowed length of %d char\n", input_file,  line+2, parameter+1,  TEXT_LEN-1);
                    error_flag = 1;
                    break;
                }
                //assign string to member of items
                switch(parameter){
                    case 0: strcpy(items->name, ptr_token, ); break;
                    case 1: strcpy(items->brand, ptr_token); break;
                    case 2: strcpy(items->invNr, ptr_token); break;
                    case 3:
                        if(atol(ptr_token)!= 0)
                            items->year = atol(ptr_token);
                        else{
                            fprintf(stderr, "ERROR: File %s, line %ld, param %ld"
                                    " - Invalid value for year (0)\n", input_file,  line+2, parameter+1);
                            error_flag = 1;
                        }
                        break;
                    default:

                        fprintf(stderr, "ERROR: File %s, line %ld, param %ld"
                                " - Too many entries per row.\n", input_file,  line+2, parameter+1);
                                error_flag = 1;
                }
                //Increase pointer and parameter count
                ptr_token = strtok(NULL, delimiter);
                parameter++;
            }
            // Raise error if number of entered parameter does not match the number of bers(4) of struct
             if(0 < parameter && parameter < 4){
                fprintf(stderr, "ERROR: File %s, line %ld, param %ld"
                        " - Too less entries per row. \n", input_file,  line+2, parameter);
                error_flag = 1;
            }
            //At this point, reading of line was successfull, increase pointer items to read next line
            items += 1;
            line++;
        }
        //Check if databse is full
        if (line-1 == len){
            fprintf(stderr, "WARNING: Database is full.\n");
        }
        fclose(file);
        //Check if an error was raised at any point
        if (error_flag == 1){
            fprintf(stderr, "WARNING: Due to invalid entries has the DB been initialised with no entries.\n");
            initItems(db_start, len);
            return -1;
        }
        return 0;
    }
    //Inform user that file was not successfully opened
    else {
        fprintf(stderr, "ERROR: Can't open file\n");
        initItems(db_start, len);
        fprintf(stderr, "WARNING: Due to invalid entries has the DB been initialised with no entries.\n");
        return -1;
    }
}

long searchFile(char *inputFile, char *key, long ignoreCase){
    FILE *file;
    long line;
    char file_line[LINE_LEN];
    char *match_begin;
    int flag_match = 0;
    int success = 0;
    //Open file and check if file exists
    if((file= fopen(inputFile, "r"))){
        //Read line by line until end of file
        while(fgets(file_line, LINE_LEN, file) != 0){
            //Set search pointer to first letter
            match_begin = &file_line[0];
            //Look for first letter of keyword until end of line
            while(match_begin != 0){
                switch(ignoreCase){
                case 0:
                    match_begin = strchr(match_begin, key[0]);
                    break;
                case 1:
                    match_begin = h_strchr_noncase(match_begin, key, 1);
                    break;
                default:
                    fprintf(stderr, "ERROR: Wrong parameter for ignorecase\n");
                    return -1;
                }
                if(match_begin == NULL)
                    break;
                //Check whether whole keyword is at current position
                switch(ignoreCase) {
                    case 0:
                        if(strncmp(match_begin, key, strlen(key)) == 0)
                            success = 1;
                        break;
                    case 1:
                        if(h_strchr_noncase(match_begin, key, strlen(key)) == &match_begin[0])
                            success = 1;
                        break;
                }

                if (success == 1){
                    printf("%s line: %ld, %s", inputFile, line,file_line);
                    flag_match = 1;
                    success = 0;
                    break;
                }
            //Increase search pointer, othwewise same letter would be found everytime
            match_begin +=1;
            }
            line++;
        }
        fclose(file);
    }
    //Print error warning
    else {
        fprintf(stderr, "ERROR: Can't open file %s\n", inputFile);
        return -1;
    }
    //Print warning that no match was found
    if(!flag_match){
        fprintf(stderr, "WARNING: No match has been found using \"%s\" in %s\n", key, inputFile);
    }
    return 0;
}

long sortItems(Data_t *items, long len, long which) {
    Data_t temp_sort;
    Data_t *ptr_sort = &temp_sort;
    long last_item, cur_item;
    long num_entries = 0;

    //Prints error warning due to invalid which param
    if(which != 1 && which != 2){
        fprintf(stderr, "ERROR: Invalid Attribut. Choose 1(iInv-No) or 2(year) instead.\n");
        return -1;
    }
    //Gets first valid index
    num_entries = h_first_valid_index(items, len);

    //Bubble Sort
    switch(which){
    case 1:
        for (last_item = num_entries-1; last_item > 0; last_item--) {
            for (cur_item = 0; cur_item < last_item; cur_item++){
                if(strcmp((items+cur_item)->invNr,(items+cur_item+1)->invNr) > 0){
                    memmove(ptr_sort, (cur_item+items), sizeof(Data_t));
                    memmove((cur_item+items), (cur_item+items+1),  sizeof(Data_t));
                    memmove((cur_item+items+1), ptr_sort, sizeof(Data_t));
                }
            }
        }
        break;
    case 2:
        for (last_item = num_entries-1; last_item > 0; last_item--) {
            for (cur_item = 0; cur_item < last_item; cur_item++){
                if((items+cur_item)->year > (items+cur_item+1)->year){
                    memmove(ptr_sort, (cur_item+items), sizeof(Data_t));
                    memmove((cur_item+items), (cur_item+items+1),  sizeof(Data_t));
                    memmove((cur_item+items+1), ptr_sort, sizeof(Data_t));
                }
            }
        }
    }
    return 0;
}

long addFromFile(Data_t *items, long len, char *inputFile, char *key) {
    FILE *file;
    long line;
    long i = 0;
    char file_line[LINE_LEN];
    char *match_begin;
    Data_t temp_save;
    Data_t *ptr_temp_save = &temp_save;
    char *ptr_token = 0;
    int flag_match = 0;
    char delimiter[] = " ";

    long index = h_first_valid_index(items, len);

    if(index == len){
        fprintf(stderr, "ERROR: Database is already full\n");
        return -1;
    }

    //Open file and check if file exists
    if((file= fopen(inputFile, "r"))){
        //Read line by line until end of file
        while(fgets(file_line, LINE_LEN, file) != 0){
            //Set search pointer to first letter
            match_begin = &file_line[0];
            //Look for first letter of keyword until end of line
            while(match_begin != 0){
                match_begin = strchr(match_begin, key[0]);
                if(match_begin == NULL)
                    break;
                //Check whether whole keyword is at current position
                if(strncmp(match_begin, key, strlen(key)) == 0){
                    printf("%s line: %ld, %s", inputFile, line,file_line);
                    ptr_token = strtok(file_line, delimiter);
                    strcpy(temp_save.name, ptr_token);
                    ptr_token = strtok(NULL, delimiter);
                    strcpy(temp_save.brand, ptr_token);
                    ptr_token = strtok(NULL, delimiter);
                    strcpy(temp_save.invNr, ptr_token);
                    ptr_token = strtok(NULL, delimiter);
                    temp_save.year = atof(ptr_token);
                    for (i=0; i < index; i++){
                        if(strcmp(temp_save.invNr, (items+i)->invNr)==0){
                            fprintf(stderr, "ERROR: Duplicate found. Line will not be added to database due to duplicate inventory number\n");
                            break;
                        }

                    }
                    if (i == index && index < len)
                        memmove((items+i), ptr_temp_save, sizeof(Data_t));

                    flag_match = 1;
                    break;
                }
            //Increase search pointer, othwewise same letter would be found everytime
            match_begin +=1;
            }
            line++;
        }
        fclose(file);
    }
    //Print error warning
    else {
        fprintf(stderr, "ERROR: Can't open file %s\n", inputFile);
        return -1;
    }
    //Print warning that no match was found
    if(!flag_match){
        fprintf(stderr, "WARNING: No match has been found using \"%s\" in %s\n", key, inputFile);
    }
    return 0;
}

long h_first_valid_index(Data_t *items, long len) {
    long index;
    for (index = 0; index < len ; index++){
        if(strncmp((items+index)->invNr, "", 1)==0)
            break;
    }
    return index;
}

char* h_strchr_noncase(char* text, char * key, long cmp_len){
    long index = 0;
    while(text[0] != '\n'){
        if(tolower(text[index])==tolower(key[index])){
            if(cmp_len == index+1)
                break;
            else
                index++;
        }
        else{
            if(index == 0){
                text++;
            }
            else {
                text+=index;
                index = 0;
            }
        }
    }
    if (text[0]== '\n')
        return NULL;
    else
        return text;

}

