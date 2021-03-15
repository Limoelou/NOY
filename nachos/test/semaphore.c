#include "userlib/syscall.h"

#include "userlib/libnachos.h"

#define NB_TAMPONS 20

int tab [] = {0};
SemId s_plein;
SemId s_vide;

void Consommateur () {
    while (1) {
        int ret = P(s_plein);
        if (tab[0] <= 0) {
            n_printf("Problem with semaphores \n");
        }
        tab[0]--;
        if (tab[0] < 0) {
            n_printf("Problem with semaphores \n");
        }
        int ret2 = V(s_vide);
    }

}

void Produire () {
    int no_times = 0;
    while (1) {
        int ret = P(s_vide);
        tab[0]++;
        no_times++;
        int ret2 = V(s_plein);
        if (no_times > 30) {
            return;
        }

    }

}

int main () {
    n_printf("** ** ** SEMAPHORE STARTING ** ** **\n");
    s_plein= SemCreate("s_plein", 0);
    s_vide = SemCreate("s_vide", NB_TAMPONS);
    ThreadId t = threadCreate("Consommateur", Consommateur);
    ThreadId t2 = threadCreate("Consommateur2", Consommateur);

    Produire ();

    n_printf("** ** ** Program terminated successfully ** ** **\n");


    return 0;

}
