#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max 6
#define maxPart 10

// Process
struct process{
    char name[20];
    int size;
    int et;
};

// Partition
struct partition{

    int nbr;
    int sadress;
    int size;

    int free;

    char pname[20];

    int remaining_time;
};

// Queue
struct queue{

    struct process p[max];

    int begin, end, count;
};

// Create queue
void creat(struct queue *q){

    q->begin=0;
    q->end=-1;
    q->count=0;
}

// Empty?
int IsEmpty(struct queue *q){

    return q->count==0;
}

// Full?
int IsFull(struct queue *q){

    return q->count==max;
}

// Enqueue
void Enqueue(struct queue *q, struct process pr){

    if(IsFull(q)){

        printf("Queue Full\n");
        return;
    }

    q->end++;

    q->p[q->end]=pr;

    q->count++;

    printf("Process added to queue\n");
}

// Dequeue
struct process Dequeue(struct queue *q){

    struct process pr=q->p[q->begin];

    q->begin++;

    q->count--;

    return pr;
}

// Update time and free partitions
void updateTime(struct partition pdt[], int n){

    for(int i=0;i<n;i++){

        if(pdt[i].free==0){

            pdt[i].remaining_time--;

            if(pdt[i].remaining_time<=0){

                printf("Process %s finished -> partition %d FREE\n",
                pdt[i].pname,pdt[i].nbr);

                pdt[i].free=1;

                strcpy(pdt[i].pname,"");
            }
        }
    }
}

// Allocate First Fit
void allocate(struct queue *q,
              struct partition pdt[], int n){

    // update time first
    updateTime(pdt,n);

    if(IsEmpty(q)){

        printf("Queue empty\n");
        return;
    }

    struct process pr=Dequeue(q);

    for(int i=0;i<n;i++){

        if(pdt[i].free==1 &&
           pdt[i].size>=pr.size){

            pdt[i].free=0;

            strcpy(pdt[i].pname,pr.name);

            pdt[i].remaining_time=pr.et;

            printf("Process %s allocated in partition %d\n",
            pr.name,pdt[i].nbr);

            return;
        }
    }

    printf("No partition available\n");
}

// Display memory
void displayMemory(struct partition pdt[], int n){

    printf("\nPartition | Size | Status | Process | Remaining time\n");

    for(int i=0;i<n;i++){

        printf(" %d | %d | ",
        pdt[i].nbr,pdt[i].size);

        if(pdt[i].free==1){

            printf("FREE | --- | ---\n");
        }

        else{

            printf("ALLOCATED | %s | %d\n",
            pdt[i].pname,
            pdt[i].remaining_time);
        }
    }
}

// Main
int main(){

    struct queue q;

    creat(&q);

    struct partition pdt[maxPart];

    int n;

    printf("Enter number of partitions: ");

    scanf("%d",&n);

    // User enters partitions
    for(int i=0;i<n;i++){

        printf("\nPartition %d\n",i+1);

        pdt[i].nbr=i+1;

        printf("Start address: ");
        scanf("%d",&pdt[i].sadress);

        printf("Size: ");
        scanf("%d",&pdt[i].size);

        pdt[i].free=1;

        strcpy(pdt[i].pname,"");
    }

    int choice;

    do{

        printf("\nMENU\n");

        printf("1. Add process\n");

        printf("2. Allocate First Fit\n");

        printf("3. Display memory\n");

        printf("0. Exit\n");

        printf("Choice: ");

        scanf("%d",&choice);

        if(choice==1){

            struct process pr;

            printf("Process name: ");
            scanf("%s",pr.name);

            printf("Size: ");
            scanf("%d",&pr.size);

            printf("Execution time: ");
            scanf("%d",&pr.et);

            Enqueue(&q,pr);
        }

        else if(choice==2){

            allocate(&q,pdt,n);
        }

        else if(choice==3){

            displayMemory(pdt,n);
        }

    }while(choice!=0);

    return 0;
}