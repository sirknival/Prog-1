#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Function declartaions
void clearBuffer();
long readNumber();
long isPrime(long num);
void primeFactors(long num);
long isPrimeFast(long num);
void expandNumber(long num);

int main()
{
    //Init Variables
    int terminate_flag = 0, prime_state = 0;
    long num = 0;

    //Welcome message
    printf("\t\tPRIME NUMBER TOOL\n"
           "\t-------------------------------\n"
           "Please enter only INTEGERS except zero!\n");
    do {
        //Read input and determine whether prime or not
        num = readNumber();
        prime_state = isPrimeFast(num);
        printf("%ld is", num);

        //If not prime and integer is positive, print prime factors and goldbachs conjecture
        if (prime_state == 0){
            printf(" not prime.\n");
            primeFactors(num);
            if(num % 2 == 0 && num > 2)
                expandNumber(num);
        }
        else if (prime_state == -1)
            printf(" not prime.");
        else
            printf(" prime.");

        printf("\n------------------------------------ ");

        //Ask user to terminate; Valid inputs are only y(es) and n(o)
        printf("\nTerminate? (y|n):\t");
        int valid_input = 0;
        do {
            switch(getchar()) {
                case 'y': terminate_flag = 1;
                case 'n': valid_input = 1; break;
                default: printf("Yes or no? (y|n): \t");
            }
            clearBuffer();
        }while(!valid_input);
    }while(!terminate_flag);
    printf("\n=================END================\n\n");
    return 0;
}

long readNumber() {
    /* Read input char by char and test if number is an integer
     * Append chars to array and convert to long
    */
    char char_cur = ' ';
    char str_num [20] = "";
    int error_flag = 0;

    printf("====================================");
    do {
        printf("\nNumber: \t");
        error_flag = 0;
        strcpy(str_num, "");
        while(1){
            char_cur = getchar();
            if (char_cur == '\n')
                break;
            if (isdigit(char_cur))
                strcat(str_num, &char_cur);
            else if (char_cur == '-' && strlen(str_num)==0)
                strcat(str_num, &char_cur);
            else
                error_flag = 1;
        }
        if(error_flag)
            printf("\tPlease enter an integer except zero. \n");

    }while (error_flag);

    return atol(str_num);
}

void clearBuffer() {
    /*Clear input buffer*/
    while(getchar()!='\n');
}

long isPrime(long num){
    /*Test if number is divisible by any other number greater than 1
    If so, then number is not prime
    */
    int prime_flag = 1;
    if (num <= 0) return -1;
    if (num == 1) return 0;
    for (long i = 2; i < num; i++) {
        if(num % i == 0){
            prime_flag = 0;
            break;
        }
    }
        return prime_flag;
}

void primeFactors(long num) {
    /*Factor out all prime factors by diving*/
    long int i = 2;
    printf("Prime factorization: %ld = ", num);
    while(num > i){
        while(num % i == 0){
            printf("%ld * ", i);
            num = num / i;
        }
        i++;
    }
    if(num != 1)
        printf("%ld", num);
    else
        printf("\b\b  ");
    return;
}

long isPrimeFast(long num) {
    /*Wrapper function for isPrime
    While first call, all numbers from 1 to 100 will be tested if prime or not and result is stored in array*/
    static long prime_num[100];
    static long array_generated = 0;
    long i;

    if (!array_generated){
        for (i = 0; i< 100; i++)
            prime_num[i]=isPrime(i);
        array_generated++;
    }

    if (num < 0 )   return -1;
    else if (num >= 100)   return isPrime(num);
    else
        return prime_num[num];
}

void expandNumber(long num) {
    /*Find to numbers to proof goldbachs conjecture*/
    long offset = 2;
    while ((num-offset) >= offset){
        if(isPrimeFast(num-offset) && isPrimeFast(offset))  break;
        offset++;
    }
    printf("\nGoldbach's conjecture: "
           "\t%ld = %ld + %ld.", num, offset, (num-offset));
    return;
}
