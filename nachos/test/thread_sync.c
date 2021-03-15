#include <stdio.h>
#include <stdlib.h>
#include "userlib/libnachos.h"
#include "userlib/syscall.h"

static SemId sem_id;

void test1(void)
{

    P(sem_id);
    //entrée de la section critique
    printf("entered critical section");
    
    printf("exit critical section");
    V(sem_id);
}

int main(void)
{
    sem_id = SemCreate("Semaphore", 1);

    ThreadId Thread_1 = newThread("Thread 1", test1, 0);
    ThreadId Thread_2 = newThread("Thread 2", test1, 0);

    Join(Thread_1);
    Join(Thread_2);
    

    SemDestroy(sem_id);

}
