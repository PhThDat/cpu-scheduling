#ifndef PCB_H
#define PCB_H

#include "utils.h"
#include "stdio.h"

#define RAND_MIN_ARRIVAL 0
#define RAND_MAX_ARRIVAL 20
#define RAND_MIN_BURST 2
#define RAND_MAX_BURST 12

typedef struct PCB {
    int iPID;
    int iArrival, iBurst;
    int iStart, iFinish, iWaiting, iResponse, iTaT;
} PCB;

PCB newPCB(bool isRandom) {
    PCB pcb;
    if (isRandom) {
        pcb.iArrival = randomBetween(RAND_MIN_ARRIVAL, RAND_MAX_ARRIVAL);
        pcb.iBurst = randomBetween(RAND_MIN_BURST, RAND_MAX_BURST);
    } else {
        printf("Arrival time: ");
        scanf("%d", &pcb.iArrival);

        printf("Burst time: ");
        scanf("%d", &pcb.iBurst);
    }

    pcb.iStart = pcb.iFinish = -1;
    pcb.iResponse = pcb.iWaiting = pcb.iTaT = 0;
    return pcb;
}

#endif