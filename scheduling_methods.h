#ifndef SCHED_METHODS_H
#define SCHED_METHODS_H

#include "PCB.h"

// FCFS
int FCFS_sessionTime(PCB p, PCB Ready[], int iReady) {
    return p.iBurst;
}

bool FCFS_schedCriteria(PCB p, int index, int iReady, PCB Ready[]) {
    return index == 0;
}

// SJF
int SJF_sessionTime(PCB p, PCB Ready[], int iReady) {
    return p.iBurst;
}

bool SJF_schedCriteria(PCB p, int index, int iReady, PCB Ready[]) {
    int minIndex = 0;
    for (int i = 1; i < iReady; i++) {
        if (Ready[i].iBurst <= Ready[minIndex].iBurst) {
            minIndex = i;
        }
    }
    return index == minIndex;
}

// SRTF
int SRTF_sessionTime(PCB p, PCB Ready[], int iReady) {
    return 1;
}

bool SRTF_schedCriteria(PCB p, int index, int iReady, PCB Ready[]) {
    int minIndex = 0;
    for (int i = 1; i < iReady; i++) {
        if (Ready[i].iBurst <= Ready[minIndex].iBurst) {
            minIndex = i;
        }
    }
    return index == minIndex;

}

// RR
int RR_sessionTime(PCB p, PCB Ready[], int iReady) {
    return p.iBurst < 5 ? p.iBurst : 5;
}

bool RR_schedCriteria(PCB p, int index, int iReady, PCB Ready[]) {
    return index == 0;
}

#endif