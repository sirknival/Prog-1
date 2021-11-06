#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/****************************************
Created by:     Florian Langer
Last edit:      14.04.2021
Version:        1.1
Function:       Calculator
Features:       Add, Sub, Mult, Div, Round, Pow, Log, Cos
//TODO:         Finish Comments

****************************************/
double readOperand();
void clearBuffer();

int main()
{
    //Declare and init variables
    char operator_char = ' ';
    const char valid_operators [] = "+-/*.plcr";
    int decimal_places = 0, flag_valid_operator = 0;
    double operand = .0, result = .0;

    printf("List of all available operations:\n"
           "------------------------------------\n"
           "\t+ \t Addition\n"
           "\t- \t Subtraction\n"
           "\t* \t Multiplication\n"
           "\t/ \t Division\n"
           "\t. \t Terminate the calculator\n"
           "\tp \t Power\n"
           "\tl \t Logarithm\n"
           "\tc \t Cosine\n"
           "\tr \t Round\n"
           "====================================\n");

    //Read first operand (only once). Afterwards first operator is the last result
    printf("Operand:\t");
    result = readOperand();

    //Endless calculator loop, can only be terminated by entering char '.'
    do {
        do {
            //Read operator char. If multiple char were enter, all chars following the first one will be discarded without raising an error
            //Clearing buffer afterwards
            printf("Operator:\t");
            scanf("%c", &operator_char);
            clearBuffer();

            //Checking if operator char is valid. If not repeat previous step
            for(int i = 0; i < strlen(valid_operators); i++) {
                if(operator_char==valid_operators[i]){
                    flag_valid_operator = 1;
                    break;
                }
            }
            if (!flag_valid_operator)
                printf("\tInvalid operator. Please enter an a valid char (case sensitive)! (%s) \n", valid_operators);
        }while(!flag_valid_operator);
        flag_valid_operator = 0;

        //Selection if (second) operand is need or not
        //Also if program has to be terminated
        switch (operator_char) {
            case '.':
                printf("Program will be terminated");
                break;
            case 'r':
                printf("Decimal places:\t");
                decimal_places = readOperand();
                break;
            case 'c': break;
            case 'l': break;
            default:
                printf("Operand:\t");
                operand = readOperand();
        }
        //In case char was '.' terminate the endless loop
        if(operator_char == '.') break;

        //Do the calculations. Check if div by 0 or log(X) out of range( x has to be greater than 0)
        switch (operator_char) {
            case '+': result += operand; break;
            case '-': result -= operand; break;
            case '*': result *= operand; break;
            case '/':
                if (operand != 0)
                    result /= operand;
                else
                    printf("\tError: division by Zero! "
                           "Previous result won't be changed.\n");
                break;
            case 'p': result = pow(result, operand); break;
            case 'l':
                if(result > 0)
                    result = log(result);
                else {
                    printf("\t%.2f can't be used as input for log-function. "
                           "Previous result won't be changed.\n", result);
                }
                    break;
            case 'c': result = cos(result * M_PI /180); break; //cos function need radians
            case 'r': break;
        }
        //Print the results
        printf("====================\n");
        //Print result
        if (operator_char != 'r')
            printf("Result:\t %f\n\n", result);
        //Print rounded result if required. Result won't be changed by rounding
        else
            printf("Result:\t %.*f\n\n", decimal_places, floor(result * pow(10.0, decimal_places) + 0.5)/  pow(10.0, decimal_places));
        }while(1); //While true - calculator loop
        return 0;
    }

    //clear Buffer - until end of line char is reached
    void clearBuffer(){
        while(getchar() != '\n');
    }

    //Function for reading and correcting number input
    double readOperand() {
        char input_char = ' ';
        char input_str[20] = "0";
        int flag_commas = 0, flag_input_corrupt = 0;
        double operand = 0;

        /*Working principle
        ------------------
        Read char by char, discard everything but numbers and append chars to string
        Stop if \n is reached */
        do {
            input_char = getchar();
            if (isdigit(input_char))
                strncat(input_str, &input_char, 1);
            //Except discard: minus if string is "empty"  --> length(1)*/
            else if (input_char == '-' && strlen(input_str) == 1){
                input_str[0] = input_char;
            }
            //Except discard: first appearing dot in string
            else if (input_char == '.' && !flag_commas){
                strncat(input_str, &input_char, 1);
                flag_commas = 1;
            }

            else if (input_char == '\n')
                break;
            else
                flag_input_corrupt = 1;
    }while(1);
    //Convert string to double
    operand = atof(input_str);

    //Inform user about corrected output
    if (flag_input_corrupt)
        printf("\tAssuming you meant: %f \n", operand);

    return operand;
}



