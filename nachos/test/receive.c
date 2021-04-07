#include "userlib/libnachos.h"
#include "userlib/syscall.h"


int main(void)
{
    char str[256];

    n_printf("en attente de send ...\n");
    int len = TtyReceive(str,256);
    n_printf("length received: %d\n message received: %s\n", len, str);
    return 0;
}
