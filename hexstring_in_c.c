#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**STEP 2 (Convert HexString to Integer)**/
float hexstringtodigit(char str[]){
    int i=0;
    int value;
    int power=0;
    float total=0;
    for(i=0; i<strlen(str); i++){

        switch(str[i]){
            case '0':
                value = 0;
                break;
            case '1':
                value = 1;
                break;
            case '2':
                value = 2;
                break;
            case '3':
                value = 3;
                break;
            case '4':
                value = 4;
                break;
            case '5':
                value = 5;
                break;
            case '6':
                value = 6;
                break;
            case '7':
                value = 7;
                break;
            case '8':
                value = 8;
                break;
            case '9':
                value = 9;
                break;
            case 'a':
            case 'A':
                value = 10;
                break;
            case 'b':
            case 'B':
                value = 11;
                break;
            case 'c':
            case 'C':
                value = 12;
                break;
            case 'd':
            case 'D':
                value = 13;
                break;
            case 'e':
            case 'E':
                value = 14;
                break;
            case 'f':
            case 'F':
                value = 15;
                break;
            default:
                printf("Something has gone badly wrong\n");
                break;
            }
        // to multiply the base from LSB up to MSB
        power = strlen(str)-1-i;
        total = total+value*pow(16, power);
        printf("%c\t%d\t\t%f\n", str[i], value, value*pow(16, power));
    }
    printf("equivalent number is: %f\n", total);
    return total;

}



int main(void){
    char str[12];
    int i;
    int value;
    char hexDigit;
    float convertedF=0;
    int converted=0;
    printf("Please enter a hex-string: ");
    scanf("%s", &str);

    printf("String length is %d\n", strlen(str));

    /**STEP 1**/
    for(i=0; i<strlen(str); i++){
        if(!((str[i]>='0' && str[i]<='9')||
            (str[i]>='A' && str[i]<='F')||
            (str[i]>='a' && str[i]<='f'))){
            printf("Invalid Hex Digit, system must exit\n");
            system("pause");
            return 1;
        }
    }
    printf("Hex is: %s\n",str);
    /**STEP  2**/

    convertedF = hexstringtodigit(str);
    converted = (int)convertedF;
    printf("Converted: %d\n", converted);

    return 0;
}
