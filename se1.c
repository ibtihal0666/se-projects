#include<stdio.h>
#include<stdlib.h>
#define max 4
struct process{
    char name[20];
    int size,et;
};
struct partition{
    int nbr,sadress,size,free;
};
struct queue{
    struct process p[max];
    int begin,end,count;
};
void creat(struct queue*q){
    q->begin=0;
    q->end=-1;
    q->count=0;
}
int IsEmpty(struct queue*q){
   return (q->count==0);
}
void Enqueue(struct queue*q, struct process pr){
 if (q->count==max)
 {
    printf("queue is full!\n");
    return;
 }
 q->end++;
 q->p[q->end]=pr; 
 q->count++;
}
struct process Dequeue(struct queue *q){
    struct process pr=q->p[q->begin];
    q->begin++;
    q->count--;
    return pr;
}
int main(){
    struct queue q;
    creat(&q);
    struct partition pdt[4]={
        {1,0,600,0},
        {2,200,300,1},
        {3,400,400,0},
        {4,600,400,0},
        
    };
 struct process p1={"p1",250,3};
 struct process p2={"p2",350,1};
 struct process p2={"p2",100,10};
 struct process p2={"p2",500,10};
 Enqueue(&q,p1);
 Enqueue(&q,p2);
 while (!IsEmpty(&q))
 {
    struct process currentP=Dequeue(&q);
    int found=0;
     for (int i = 0; i < 6; i++)
     {
        if (pdt[i].free==1 && pdt[i].size>=currentP.size)
        {
            
         pdt[i].free=0;
         printf("Process %s loaded in partition %d\n",currentP.name,pdt[i].nbr);
            found=1;
            break;
        }   
    }
        
    
    if (!found)
    {
        printf("No space found for this process %s\n",currentP.name);

    }
    
    
 }
 printf("\nFinal memmory status\n");
 printf(" partition |size| status \n");
 for (int i = 0; i < 6; i++)
 {
 printf("partition %d |%d| ",pdt[i].nbr,pdt[i].size);
    if (pdt[i].free==1)
    
        printf("FREE\n");
    else
    
        printf("ALLOCATED\n"); 
 }
 return 0;
}