#include "error.h"

int error()
{
    fprintf(stderr, "%s\n", strerror(errno));
    return errno;
}
