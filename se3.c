#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_UNITS 500
#define MAX_PROC 50

typedef struct {
    char name[20];
    int sizeKO;     
    int execTime;
    int units;
    int allocated;
} Process;

int memorySizeKO;
int unitSizeKO;
int totalUnits;

int bitmap[MAX_UNITS];
Process queue[MAX_PROC];

void printBitmap() {
    printf("\nBitmap:\n");
    for(int i=0;i<totalUnits;i++)
        printf("%d ", bitmap[i]);
    printf("\n");
}

void printMemoryList() {
    printf("\nMemory List (ML):\n");
    for(int i=0;i<totalUnits;i++){
        if(bitmap[i]==0)
            printf("Unit %d : FREE\n", i);
        else
            printf("Unit %d : USED\n", i);
    }
}

int bestFit(int neededUnits) {

    int bestIndex = -1;
    int bestSize = totalUnits + 1;
    int i = 0;

    while(i < totalUnits) {

        while(i < totalUnits && bitmap[i] == 1)
            i++;

        int start = i;
        int count = 0;

        while(i < totalUnits && bitmap[i] == 0) {
            count++;
            i++;
        }

        if(count >= neededUnits && count < bestSize) {
            bestSize = count;
            bestIndex = start;
        }
    }

    return bestIndex;
}

void allocateProcess(Process *p) {

    p->units = ceil((float)p->sizeKO / unitSizeKO);

    int index = bestFit(p->units);

    if(index == -1) {
        printf("\n❌ No space for %s\n", p->name);
        p->allocated = 0;
        return;
    }

    for(int i=index;i<index + p->units;i++)
        bitmap[i] = 1;

    p->allocated = 1;

    printf("\n✅ %s Allocated from unit %d to %d\n",
           p->name, index, index + p->units - 1);
}

void freeProcess(Process *p) {

    if(p->allocated == 0)
        return;

    for(int i=0;i<totalUnits;i++)
        bitmap[i] = 0;

    p->allocated = 0;
}

int main() {

    printf("Enter Memory Size (KO): ");
    scanf("%d", &memorySizeKO);

    printf("Enter Unit Size (KO): ");
    scanf("%d", &unitSizeKO);

    totalUnits = memorySizeKO / unitSizeKO;

    for(int i=0;i<totalUnits;i++)
        bitmap[i] = 0;

    int n;
    printf("Enter number of processes in Queue: ");
    scanf("%d", &n);

    for(int i=0;i<n;i++) {

        printf("\nProcess %d name: ", i+1);
        scanf("%s", queue[i].name);

        printf("Size (KO): ");
        scanf("%d", &queue[i].sizeKO);

        printf("Execution Time: ");
        scanf("%d", &queue[i].execTime);

        allocateProcess(&queue[i]);

        printBitmap();
        printMemoryList();
    }

    printf("\n=== Execution Simulation ===\n");

    for(int t=1;t<=10;t++) {

        printf("\nTime = %d\n", t);

        for(int i=0;i<n;i++) {
            if(queue[i].allocated) {
                queue[i].execTime--;

                if(queue[i].execTime <= 0) {
                    printf("Process %s Finished\n", queue[i].name);
                    freeProcess(&queue[i]);
                }
            }
        }

        printBitmap();
    }

    return 0;
}