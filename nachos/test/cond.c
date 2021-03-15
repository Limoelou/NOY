#include "userlib/libnachos.h"
#include "userlib/syscall.h"

int counter; 
CondId cond; 
  
void foo(void) 
{ 
    CondWait(cond);
    n_printf("foo starting :");

    unsigned long i = 0; 
    counter += 1; 
    n_printf("\n Job 1 has started\n"); 
  
    for (i = 0; i < 1000; i++);
  
    n_printf("\n Job 1 has finished\n"); 
  
  
} 
  
void func2(void)
{
    CondWait(cond);
    n_printf("func 2 starting :");
    unsigned long i = 0; 
    counter += 1; 
    n_printf("\n Job 2 has started\n"); 
  
    for (i = 0; i < 1000; i++);
  
    n_printf("\n Job 2 has finished\n"); 

}

void func3(void)
{
    n_printf("func 3 starting :");
    unsigned long i = 0; 
    n_printf("\n Job 3 has started\n"); 
  
    for (i = 0; i < 1000; i++);
  
    n_printf("\n Job 3 has finished\n"); 
   CondBroadcast(cond);
   // CondSignal(cond);
    //CondSignal(cond);
    //CondSignal(cond);
}

void func4(void)
{
    CondWait(cond);
    n_printf("func 4 starting :");
    unsigned long i = 0; 
    n_printf("\n Job 4 has started\n");
  
    for (i = 0; i < 1000; i++);
  
    n_printf("\n Job 4 has finished\n"); 

}

int main(void) 
{ 
  
    cond = CondCreate("testcond");

    ThreadId tid1 = threadCreate("tid1", foo);
    ThreadId tid2 = threadCreate("tid2", func2); 
    ThreadId tid3 = threadCreate("tid3", func3);
    ThreadId tid4 = threadCreate("tid4", func4);

    Join(tid1);
    Join(tid2);
    Join(tid3);
    Join(tid4);

    CondDestroy(cond);

    return 0; 
} 
