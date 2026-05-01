#include <stdio.h>

#define MAX 100

int bitmap[MAX];

void display(int n) {
    printf("\nMemory:\n");
    for(int i=0;i<n;i++)
        printf("%d ", bitmap[i]);
    printf("\n");
}

void allocate(int n, int units) {
    int bestStart=-1, bestSize=MAX;

    for(int i=0;i<n;) {
        if(bitmap[i]==0) {
            int j=i;
            while(j<n && bitmap[j]==0) j++;

            int hole=j-i;

            if(hole>=units && hole<bestSize) {
                bestSize=hole;
                bestStart=i;
            }
            i=j;
        } else i++;
    }

    if(bestStart==-1) {
        printf("No space!\n");
        return;
    }

    for(int i=bestStart;i<bestStart+units;i++)
        bitmap[i]=1;

    printf("Allocated at %d\n", bestStart);
}

void freeMem(int start, int len) {
    for(int i=start;i<start+len;i++)
        bitmap[i]=0;
}

int main() {
    int memSize, unit, n;
    int choice;

    printf("Memory size: ");
    scanf("%d",&memSize);

    printf("Allocation unit: ");
    scanf("%d",&unit);

    n=memSize/unit;

    for(int i=0;i<n;i++)
        bitmap[i]=0;

    do {
        printf("\n1 Allocate\n2 Free\n3 Display\n4 Exit\nChoice: ");
        scanf("%d",&choice);

        if(choice==1) {
            int size, u;
            printf("Program size: ");
            scanf("%d",&size);
            u=size/unit;
            allocate(n,u);
        }

        if(choice==2) {
            int s,l;
            printf("Start + length: ");
            scanf("%d%d",&s,&l);
            freeMem(s,l);
        }

        if(choice==3)
            display(n);

    } while(choice!=4);

    return 0;
}