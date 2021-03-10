#include "userlib/libnachos.h"
#include "userlib/syscall.h"

int counter = 0;

LockId lock; 
  
void foo(void) 
{ 

    if(LockAcquire(lock))
    {
        n_printf("lock acquire error!\n");
        return;
    }

    unsigned long i = 0; 
    counter += 1; 
    n_printf("\n Job %d has started\n", counter); 
  
    for (i = 0; i < 1000; i++);
  
    n_printf("\n Job %d has finished\n", counter); 
  
    LockRelease(lock); 
  
} 
  
int main(void) 
{ 
  
    lock = LockCreate("test_lock");
    n_printf("lock id: %d\n", lock);

    ThreadId tid1 = threadCreate("tid1", foo);
    ThreadId tid2 = threadCreate("tid2", foo); 
  
    Join(tid1);
    Join(tid2);
    LockDestroy(lock);
  
    return 0; 
} 
