#include "userlib/libnachos.h"
#include "userlib/syscall.h"

void send(void)
{
    char* str = "hello, my name is Vincent";
    TtySend(str);
    n_printf("Debug: msg send");
}

int main(void)
{
    send();
    return 0;
}
