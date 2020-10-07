#include "convert_number_system.h"

char symbol(int number)
{
    switch(number) {
        case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8:
            return (char)('0' + number);
        case 10:
            return 'a';
        case 11:
            return 'b';
        case 12:
            return 'c';
        case 13:
            return 'd';
        case 14:
            return 'e';
        case 15:
            return 'f';
        case 16:
            return 'g';
        case 17:
            return 'h';
        case 18:
            return 'i';
    }

    return '\0';
}

void convert_number_system(char result[BUFFER_SIZE], const char number[BUFFER_SIZE], int basis)
{
    int num = atoi(number);
    int i = 0;

    if (num == 0)
    {
        result = "0";
        return;
    }

    while (num)
    {
        result[i++] = symbol(num % basis);
        num /= basis;
    }

    result[i] = '\0';

    for (int j = 0; j < i / 2; ++j)
    {
        char tmp = result[j];
        result[j] = result[i - j - 1];
        result[i - j - 1] = tmp;
    }
}

int is_number(char number[BUFFER_SIZE])
{
    for (int i = 0; number[i] != '\0'; ++i)
    {
        if (number[i] - '0' > 9 || number[i] - '0' < 0)
            return 0;
    }
    return 1;
}
