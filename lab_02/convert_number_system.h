#ifndef __CONVERT_NUMBER_SYSTEM_H
#define __CONVERT_NUMBER_SYSTEM_H

#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 100

void convert_number_system(char result[BUFFER_SIZE], const char number[BUFFER_SIZE], int basis);
int is_number(char number[BUFFER_SIZE]);

#endif // __CONVERT_NUMBER_SYSTEM_H
