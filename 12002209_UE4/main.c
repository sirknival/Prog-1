#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define TEXT_LEN 100

void my_getline(char input[], long len);
long get_letters(char text[]);
long get_others(char tex[]);
void analyze_text(char text[]);
char *get_letters_ptr(char text[]);
char *get_others_ptr(char text[]);
void analyze_text_ptr(char text[]);
void trim_text(char text[]);

int main()
{
    //Init all variables
    char input[TEXT_LEN] = "";
    long num_letters = 0, num_others = 0;
    int flag_valid_input = 0, flag_terminate = 0;

    printf("\t\tSTRING ANALYZER\n "
           "=============================================\n");
    //Looping until terminte
    do{
        my_getline(input, TEXT_LEN);

        num_letters = get_letters(input);
        num_others = get_others(input);
        if(num_letters != 0)
            printf("Letters at the beginning: %ld", num_letters);
        else
            printf("Non-letters at the beginning: %ld", num_others);

        //analyze_text(input);
        analyze_text_ptr(input);

        trim_text(input);

        //Terminating dialoge and logic
        flag_valid_input = 0;
        printf("\n------------------------------------\n"
                "Terminate? (y|n):\t");
        do{
            switch(getchar()){
                case 'y': flag_terminate = 1;
                case 'n': flag_valid_input = 1; break;
                default: printf("Yes or no? (y|n): \t");
            }
            fflush(stdin);
        }while(!flag_valid_input);
    }while(!flag_terminate);

    printf("\n=================END================\n\n");
    return 0;
}
/* Reading char by char until buffer is empty or max string length -1 (due to terminating char) is reached */
void my_getline(char input[], long len){
    long index = 0;
    char cur_char = ' ';

    printf("Please enter your string. If your string exceeds a length of %ld, it will be cropped. \n>", len-1);
    for(index = 0; index+1 < len; index++){
        cur_char = getchar();
        if(cur_char == '\n')
            break;
        input[index] = cur_char;
    }
    input[index] = '\0';
    fflush(stdin);
    return;
}
/*Read char by char until a non letter is reached*/
long get_letters(char text[]) {
    long index = 0, str_len = strlen(text);
    while(isalpha(text[index])&&  index < str_len)
        index++;
    return index;
}
/*Read char by char until a letter is reached*/
long get_others(char text[]) {
    long index = 0, str_len = strlen(text);
    while(!isalpha(text[index]) &&  index < str_len)
        index++;
    return index;
}
/*Counting number of words. Function:
Call get letters, if return value is bigger than 0 words is found, store length and move starting point.
If return value of get_letters is 0, than get length of non letter sequence and move starting point by that number*/
void analyze_text(char text[]){
    long length_count[10]={0};
    long snippet_length = 0, word_count = 0, i = 0;
    char *text_part = text;

    do{
        snippet_length = get_letters(text_part);
        if(snippet_length==0)
            snippet_length = get_others(text_part);
        else{
            snippet_length = (snippet_length > 10 ? 10 : snippet_length);
            length_count[snippet_length-1]++;
            word_count++;
        }
        text_part += snippet_length;
    }while(*text_part != '\0');

    printf("\nTotal number of words: \t%ld", word_count);
    printf("\nStats:\n"
           "------------------------------------\n"
           " Length | Frequency\n"
           "--------|-------------\n");
    for (i = 1; i < 10; i++)
        printf("%4ld\t|%6ld\n", i, length_count[i-1]);
    printf(">=10\t|%6ld\n", length_count[i-1]);
    return;
}
/*Read char by char using a pointer ans set the pointer address to the address after the last letter.
If end of char array is reached return Nullbyte*/
char* get_letters_ptr(char text[]){
    char *cur_ptr = text;
    while (isalpha(*cur_ptr) && *cur_ptr != '\0')
        cur_ptr++;
    cur_ptr = (*cur_ptr == '\0' ? NULL : cur_ptr);
    return cur_ptr;
}
/*Read char by char using a pointer ans set the pointer address to the address after the non last letter.
If end of char array is reached return Nullbyte*/
char* get_others_ptr(char text[]){
    char *cur_ptr = &text[0];
    while (!isalpha(*cur_ptr) && *cur_ptr != '\0')
        cur_ptr++;
    cur_ptr = (*cur_ptr == '\0' ? NULL : cur_ptr);
    return cur_ptr;
}
/*Counting number of words. Function:
Call get_letters_ptr, if return value(ptr) differs from start value, calculate ans store length and move starting address of text.
If return value of get_letters_ptr equal to the address of the begin of the text,  than move the starting address to the next letter address*/
void analyze_text_ptr(char text[]){
    long length_count[10]={0};
    long snippet_length = 0, word_count = 0, i = 0;
    char *text_part = text;
    char *cur_ptr = text;

    while(cur_ptr!= NULL){
        cur_ptr = get_letters_ptr(text_part);
        if(cur_ptr == text_part){
            cur_ptr = get_others_ptr(text_part);
        }
        else{
            if (cur_ptr == NULL)
                snippet_length = &text[strlen(text)]-text_part;
            else
                snippet_length = cur_ptr - text_part;
            snippet_length = (snippet_length > 10 ? 10 : snippet_length) - 1 ;
            length_count[snippet_length]++;
            word_count++;
        }
        text_part = cur_ptr;
    }

    printf("\nTotal number of words: \t%ld", word_count);
    printf("\nStats:\n"
           "------------------------------------\n"
           " Length | Frequency\n"
           "--------|-------------\n");
    for (i = 1; i < 10; i++)
        printf("%4ld\t|%6ld\n", i, length_count[i-1]);
    printf(">=10\t|%6ld\n", length_count[i-1]);
    return;
}
/*Trim all non letters from text. Function: replace all non letter char with a space char.
Using memomove shift all words(continous letter sequences) together that all the sequences are only seperated by a single space*/
void trim_text(char text[]) {
    long index = 0, text_len = strlen(text);
    char *text_ptr = text;
    char *cur_ptr = text;
    char *end_ptr = &text[text_len];
    for(index = 0; index < text_len; index++)
        text[index] = (isalpha(text[index])? text[index] : ' ');

    do{
        cur_ptr = get_letters_ptr(text_ptr);
        if(cur_ptr != text_ptr)
            text_ptr = cur_ptr;
        else{
            cur_ptr = get_others_ptr(text_ptr);
            if(cur_ptr == NULL)
                break;
            memmove(text_ptr, cur_ptr-1, end_ptr-text_ptr);
            text_ptr = get_letters_ptr(text_ptr)+1;
        }
    }while(cur_ptr != NULL);
    printf("Words only:%s", text);

    return;

}

