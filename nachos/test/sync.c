#include "userlib/libnachos.h"
#include "userlib/syscall.h"

static SemId sem_id;
static int glob;

void test1(void)
{
    n_printf("begin test 1\n");
    int i;
    P(sem_id);
    //entrée de la section critique
    n_printf("entered critical section in test 1\n");
    for(i = 0; i < 1000; i++)
    {
	glob++;    
    }
    n_printf("glob :%d exit critical section\n", glob);
    n_printf("exit critical section\n");
    V(sem_id);
}

void test2(void)
{
    n_printf("begin test 2\n");
    int i;
    P(sem_id);
    //entrée de la section critique
    n_printf("entered critical section in test 2\n");
    for(i = 0; i < 1000; i++)
    {
	glob--;    
    }
    n_printf("glob :%d exit critical section\n", glob);
    V(sem_id);
}

int main(void)
{
    glob = 10;

    sem_id = SemCreate("sem1", 1);
    n_printf("sem id : %d\n", sem_id);

    n_printf("begin test 1 in main\n");
    ThreadId Thread_1 = threadCreate("Thread 1", test1);
    n_printf("begin test 2 in main\n");
    ThreadId Thread_2 = threadCreate("Thread 2", test2);

    Join(Thread_1);
    Join(Thread_2);
    

    SemDestroy(sem_id);
    return 0;
}
