#include "errno.h"
#include "errnomsg.h"

char *__geterrno(int err)
{
    if (err < 140)
        return errnomsg[err];
    return "Internal error";
}