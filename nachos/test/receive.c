#include "userlib/libnachos.h"
#include "userlib/syscall.h"


int main(void)
{
    char str[256];
    unsigned int len = TtyReceive(str,256);
    n_printf("length received: %lu\nmessage received: %s", len, str);
    return 0;
}
