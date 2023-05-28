#ifndef CPU_SCHEDULING_DEP
#define CPU_SCHEDULING_DEP

#include <stdbool.h>
#include "stdlib.h"
#include "PCB.h"

#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3

void inputProcess(int n, PCB P[]) {
    int isRandom = 0;
    printf("Randomize? (1/0) ");
    scanf("%d", &isRandom);

    for (int i = 0; i < n; i++) {
        if (!isRandom) {
            printf("\n\x1b[34mPCB %d:\x1b[0m\n", i + 1);
        }

        P[i] = newPCB(isRandom);
        P[i].iPID = i + 1;
    }
}

void printProcess(int n, PCB P[]) {
    printf("\x1b[34m(PID, Arrival, Burst)\x1b[35m(Start, Finish, Response, Waiting, TaT)\x1b[0m\n");
    for (int i = 0; i < n; i++) {
        printf("\x1b[34m(%d, %d, %d)\x1b[35m(%d, %d, %d, %d, %d)\x1b[0m\n", P[i].iPID, P[i].iArrival, P[i].iBurst, P[i].iStart, P[i].iFinish, P[i].iResponse, P[i].iWaiting, P[i].iTaT);
    }
}

void exportGanttChart(int n, PCB P[]);

void pushProcess(int *n, PCB P[], PCB Q) {
    P[*n] = Q;
    *n += 1;
}

void removeProcess(int *n, int index, PCB P[]) {
    *n -= 1;
    for (int i = index; i < *n; i++) {
        P[i] = P[i + 1];
    }
}

int swapProcess(PCB *P, PCB *Q) {
    PCB temp = *Q;
    *Q = *P;
    *P = temp;
}

int chooseCriteria(PCB P, int iCriteria) {
    switch (iCriteria) {
        case SORT_BY_ARRIVAL:
            return P.iArrival;
        case SORT_BY_BURST:
            return P.iBurst;
        case SORT_BY_PID:
            return P.iPID;
        case SORT_BY_START:
            return P.iStart;
    }
}

int partition (PCB P[], int low, int high, int iCriteria) {
    int pivot = chooseCriteria(P[high], iCriteria);
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        int jCri = chooseCriteria(P[j], iCriteria);

        if (jCri < pivot) {
            i++;
            swapProcess(&P[i], &P[j]);
        }
    }

    swapProcess(&P[i + 1], &P[high]);
    return (i + 1);
}

void quickSort(PCB P[], int low, int high, int iCriteria) {
    if (low < high) {
        int pivot = partition(P, low, high, iCriteria);

        quickSort(P, low, pivot - 1, iCriteria);
        quickSort(P, pivot + 1, high, iCriteria);
    }
}

float calculateAWT(int n, PCB P[]) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += P[i].iWaiting;
    }

    return (float)sum / n;
}

float calculateATaT(int n, PCB P[]) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += P[i].iTaT;
    }

    return (float)sum / n;
}

void pushToReady(int* iRemain, PCB Input[], int* iReady, PCB Ready[], int* passedTime) {
    // If CPU is idle (no Ready process available)
    if (*iReady == 0 && Input[0].iArrival > *passedTime) {
        *passedTime = Input[0].iArrival;
    }

    // Push processes from Input to Ready
    while (*iRemain > 0 && Input[0].iArrival <= *passedTime) {
        pushProcess(iReady, Ready, Input[0]);
        removeProcess(iRemain, 0, Input);
    }
}



PCB* schedule(
    int n, 
    PCB Input[], 
    int howLongSessionLasts(PCB P, PCB Ready[]),
    bool selectReadyProcessCriteria(PCB P, int index, int iReady, PCB Ready[])
) {
    PCB Ready[n];
    PCB* Terminated = malloc(sizeof(PCB) * n);
    int memoBurst[n];
    for (int i = 0; i < n; i++) {
        memoBurst[i] = Input[i].iBurst;
    }

    int iRemain = n;
    int iReady = 0;
    int iTerminated = 0;

    quickSort(Input, 0, n-1, SORT_BY_ARRIVAL);

    printf("After sorting:\n");
    printProcess(n, Input);

    int passedTime = 0;
    printf("GANTT CHART: \x1b[34m%d \x1b[35m", Input[0].iArrival);

    pushToReady(&iRemain, Input, &iReady, Ready, &passedTime);

    // Process until all processes are finished
    while (iTerminated < n) {
        // The first process in the ReadyArray will be selected by default
        PCB* currentCpuProcess = &Ready[0];
        // Iterate over the whole Ready processes, whichever matches the criteria will be chosen
        int i = 0;
        while (i < iReady) {
            if (selectReadyProcessCriteria(Ready[i], i, iReady, Ready)) {
                currentCpuProcess = &Ready[i];

                // If chosen process's Start time not set, set one
                if (currentCpuProcess->iStart == -1) {
                    currentCpuProcess->iStart = passedTime;
                }
                // Calculate Response time
                currentCpuProcess->iResponse = passedTime - currentCpuProcess->iArrival;

                break;
            }

            i++;
        }

        // Calculate remaining Burst of chosen process
        int sessionTime = howLongSessionLasts(*currentCpuProcess, Ready);
        passedTime += sessionTime;
        currentCpuProcess->iBurst -= sessionTime;
        printf("P%d \x1b[34m%d \x1b[35m", currentCpuProcess->iPID, passedTime);

        if (currentCpuProcess->iBurst == 0) {
            currentCpuProcess->iFinish = passedTime;
            // printProcess(iReady, Ready);
            pushProcess(&iTerminated, Terminated, *currentCpuProcess);
            removeProcess(&iReady, i, Ready);
            pushToReady(&iRemain, Input, &iReady, Ready, &passedTime);
        } else {
            pushToReady(&iRemain, Input, &iReady, Ready, &passedTime);
            pushProcess(&iReady, Ready, *currentCpuProcess);
            removeProcess(&iReady, i, Ready);
        }
        // printProcess(iReady, Ready);
    }

    for (int i = 0; i < iTerminated; i++) {
        PCB proc = Terminated[i];
        proc.iBurst = memoBurst[proc.iPID - 1];
        proc.iTaT = proc.iFinish - proc.iArrival;
        proc.iWaiting = proc.iTaT - proc.iBurst;
        proc.iResponse = proc.iStart - proc.iArrival;
        Terminated[i] = proc;
    }

    return Terminated;
}

#endif