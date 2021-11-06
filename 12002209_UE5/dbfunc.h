#ifndef DBFUNC_H_INCLUDED
#define DBFUNC_H_INCLUDED

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TEXT_LEN 20+1
#define LINE_LEN (TEXT_LEN+1)*4

struct Data_s {
    char name[TEXT_LEN];
    char brand[TEXT_LEN];
    char invNr[TEXT_LEN];
    long year;
};

typedef struct Data_s Data_t;

void initItems(Data_t *items, long len);
void printItem(Data_t *item);
void printDB(Data_t *items, long len);
long readValue(char *name);
void readString(char *name, char *text, long len);
long addItem(Data_t *items, long len);
long deleteItem(Data_t *items, long len);
long saveDB(Data_t *items, long len, char *output_file);
long loadDB(Data_t *items, long len, char *input_file);
long searchFile(char *inputFile, char *key, long ignoreCase);
long sortItems(Data_t *items, long len, long which);
long addFromFile(Data_t *items, long len, char *inputFile, char *key);
long h_first_valid_index(Data_t *items, long len);
char* h_strchr_noncase(char* text, char * key, long cmp_len);


#endif // DBFUNC_H_INCLUDED
