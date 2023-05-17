#include "cpu-scheduling-dep.h"
#include "scheduling_methods.h"
#include "time.h"
#include "stdlib.h"

#define FCFS 0
#define SJF 1
#define SRTF 2
#define RR 3

int main() {
    srand(time(NULL));
    int n = 10;
    PCB P[n];

    int method;
    printf("Choose a method:\n");
    printf("0. FCFS\n");
    printf("1. SJF\n");
    printf("2. SRTF\n");
    printf("3. RR\n");
    scanf("%d", &method);

    void* sessionTime;
    void* criteria;
    switch (method) {
        case FCFS:
            sessionTime = FCFS_sessionTime;
            criteria = FCFS_schedCriteria;
            break;
        case SJF:
            sessionTime = SJF_sessionTime;
            criteria = SJF_schedCriteria;
            break;
        case SRTF:
            sessionTime = SRTF_sessionTime;
            criteria = SRTF_schedCriteria;
            break;
        case RR:
            sessionTime = RR_sessionTime;
            criteria = RR_schedCriteria;
    }

    inputProcess(n, P);
    PCB* output = schedule(n, P, sessionTime, criteria);
    printf("\n");
    quickSort(output, 0, n-1, SORT_BY_ARRIVAL);
    printProcess(n, output);
    printf("AWT: %f\n", calculateAWT(n, output));
    printf("ATaT: %f\n", calculateATaT(n, output));
}