#include <stdio.h>

#define ELMZ 10

int main(void) 
{
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

    printf("\n #   hex  == dec    running sum(hex == dec)\n");
    printf("--------------------------------------------\n");
    for (int ii = 0; ii < ELMZ; ii++)
    {
        temp = temp + hexx[ii];
        printf("%2d)  %04x == %3d  ,  >>>       %4x == %4d\n", 
            ii+1, hexx[ii], hexx[ii], temp, temp);
    }

    return 0;
}
