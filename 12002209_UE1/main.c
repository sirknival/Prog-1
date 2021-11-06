#include <stdio.h>
#include <stdlib.h>



int main()
{
    //Defining variables
    const short int day = 22, month = 11, year = 1999;
    const long int stud_num = 12002209;
    const char forename [] = "Florian";
    const char surname [] = "Langer";

    //TASK A
    printf("*********************************\n");
    printf("*\tProgrammieren 1\t\t*\n");
    printf("*\t\t\t\t*\n");
    printf("*\t%s, %s\t\t*\n", forename, surname);
    printf("*\t%ld\t\t*\n", stud_num);
    printf("*\t%hd.%hd.%hd\t\t*\n", day, month, year);
    printf("*********************************\n\n\n");

    //TASK B
    /* Definition von Konstanten */
    const double PI = 3.14159;

    /* Definition der Variablen */
    double hauptachse;
    double nebenachse;
    double flaeche;
    double flaeche_inkreis;
    double flaeche_umkreis;

    /* Wertezuweisungen */
    hauptachse = 3.9;
    nebenachse = 1.4;

    /* Berechnungen */
    flaeche = hauptachse * nebenachse * PI / 4.0;
    flaeche_inkreis = nebenachse * nebenachse * PI;
    flaeche_umkreis = hauptachse * hauptachse * PI;

    /* Ausgabe */
    printf("Ellipse:\n");
    printf("\tL\x84nge der %s = %.1f\n", "Hauptachse", hauptachse);
    printf("\tL\x84nge der %s = %.1f\n", "Nebenachse", nebenachse);
    printf("\tFl\x84 \bche = %.2f\n", flaeche);
    printf("\tFl\x84 \bche Inkreis = %.2f\n", flaeche_inkreis);
    printf("\tFl\x84 \bche Umkreis = %.2f\n", flaeche_umkreis);

    return 0;
}
