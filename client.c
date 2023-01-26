#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ipc.h"
#include "types.h"

int main(int argc, char** argv) {
    int nb_chefs, shmid, *niveau_occupation_chef;
    key_t cle;

    if (argc < 4) {
        printf("USAGE : %s <numero_ordre> <nb_1> <nb_2> <nb_3> <nb_4>", argv[0]);
        return 1;
    }

    shmid = get_segment_memoire(cle, nb_chefs * sizeof(int));

    if ((niveau_occupation_chef =  shmat(shmid, NULL, 0)) == -1) {
        perror("client shmat");
    }

    

    printf("[CHEF %d] FIN\n", numero_ordre);
    return 0;
}