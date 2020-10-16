#include "convert_number_system.h"

char symbol(int number)
{
    if (number <= 9)
    {
        return '0' + number;
    }
    else
    {
        return 'a' - 10 + number;
    }
}

void convert_number_system(char result[BUFFER_SIZE], const char number[BUFFER_SIZE], int basis)
{
    int num = atoi(number);
    int i = 0;
    short negative = 0;

    if (num == 0)
    {
        result[0] = '0';
        result[1] = '\0';
        return;
    }
    else if (num < 0)
    {
        negative = 1;
        num *= -1;
    }

    while (num)
    {
        result[i++] = symbol(num % basis);
        num /= basis;
    }

    if (negative)
    {
        result[i++] = '-';
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
        if ((number[i] - '0' > 9 || number[i] - '0' < 0) && number[i] != '-')
            return 0;
    }
    return 1;
}
