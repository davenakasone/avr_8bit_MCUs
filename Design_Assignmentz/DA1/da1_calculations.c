/*
    calculations hex->decimal
    DA 1, tasks: 1, 2, 3, 4
*/

#include <stdio.h>
#include <stdlib.h>

#define ELMZ 10

int main(void) 
{
    int num1 = 0x1234;
    int num2 = 0x5678;
    int sum = num1 + num2;
    printf("\nTask 1, 2, 3  >>>");
    printf("\n\ttask 1 number: $%x    %d", num1, num1);
    printf("\n\ttask 2 number: $%x    %d", num2, num2);
    printf("\n\tsum: $%x    %d\n\n", sum, sum);

    int temp = 0x0;
    int hexx[10] = 
    {
        0x0111,
        0x0122,
        0x0133,
        0x0144,
        0x0155,
        0x0166,
        0x0177,
        0x0188,
        0x0199,
        0x01AA
    };

    printf("\nTask 4  >>>");
    printf("\n\t  #        number                 running sum\n");
    printf("\t----------------------------------------------\n");
    for (int ii = 0; ii < ELMZ; ii++)
    {
        temp = temp + hexx[ii];
        printf("\t%2d)    $%04x ::%4d    >>>    $%4x  ::  %4d\n", 
            ii+1, hexx[ii], hexx[ii], temp, temp);
    }

    printf("\n\n\t\t ~ ~ ~ PROGRAM COMPLETE ~ ~ ~\n\n");
    return EXIT_SUCCESS;
}


////////~~~~~~~~END>  da1_calculations.c
