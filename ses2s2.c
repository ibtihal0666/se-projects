#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char type; // 'H' or 'P'
    char name[10];
    int start;
    int length;
    struct Node* next;
} Node;

// ================= CREATE =================
Node* create_node(char type, char name[], int start, int length) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->type = type;
    strcpy(n->name, name);
    n->start = start;
    n->length = length;
    n->next = NULL;
    return n;
}

// ================= PRINT =================
void print_ML(Node* head) {
    printf("ML: ");
    Node* temp = head;
    while (temp) {
        if (temp->type == 'H')
            printf("H,%d,%d", temp->start, temp->length);
        else
            printf("%s,%d,%d", temp->name, temp->start, temp->length);

        temp = temp->next;
        if (temp) printf(" -> ");
    }
    printf("\n");
}

void print_bit_map(int bit_map[], int total_units) {
    printf("Bit Map: ");
    for (int i = 0; i < total_units; i++)
        printf("%d", bit_map[i]);
    printf("\n");
}

// ================= MERGE =================
void merge_holes(Node* head) {
    Node* curr = head;
    while (curr && curr->next) {
        if (curr->type == 'H' && curr->next->type == 'H') {
            curr->length += curr->next->length;
            Node* temp = curr->next;
            curr->next = temp->next;
            free(temp);
        } else {
            curr = curr->next;
        }
    }
}

// ================= BEST FIT =================
void best_fit(Node** head, char pname[], int size, int bit_map[]) {

    Node *curr = *head, *best = NULL;

    while (curr) {
        if (curr->type == 'H' && curr->length >= size) {
            if (!best || curr->length < best->length)
                best = curr;
        }
        curr = curr->next;
    }

    if (!best) {
        printf("No space for %s\n", pname);
        return;
    }

    int start = best->start;

    // تحديث Bit Map
    for (int i = start; i < start + size; i++)
        bit_map[i] = 1;

    // نفس الحجم
    if (best->length == size) {
        best->type = 'P';
        strcpy(best->name, pname);
    }
    // تقسيم Hole
    else {
        Node* newHole = create_node('H', "", start + size, best->length - size);
        Node* newProcess = create_node('P', pname, start, size);

        newProcess->next = newHole;
        newHole->next = best->next;

        if (*head == best) *head = newProcess;
        else {
            curr = *head;
            while (curr->next != best) curr = curr->next;
            curr->next = newProcess;
        }

        free(best);
    }

    printf("Process %s allocated at %d\n", pname, start);
}

// ================= FREE =================
void free_process(Node* head, char pname[], int bit_map[]) {
    Node* curr = head;

    while (curr) {
        if (curr->type == 'P' && strcmp(curr->name, pname) == 0) {

            // تحديث Bit Map
            for (int i = curr->start; i < curr->start + curr->length; i++)
                bit_map[i] = 0;

            curr->type = 'H';
            strcpy(curr->name, "");

            printf("Freed %s\n", pname);
            return;
        }
        curr = curr->next;
    }

    printf("Process not found !\n");
}

// ================= FRAGMENTATION =================
void fragmentation(Node* head, int total_units) {
    int total_free = 0;
    Node* curr = head;

    while (curr) {
        if (curr->type == 'H')
            total_free += curr->length;
        curr = curr->next;
    }

    float percent = ((float)total_free / total_units) * 100;

    printf("Total Fragmentation: %d units\n", total_free);
    printf("Fragmentation Percentage: %.2f%%\n", percent);
}

// ================= MAIN =================
int main() {

    int memory_size, unit_size;

    printf("Enter memory size (Ko): ");
    scanf("%d", &memory_size);

    printf("Enter unit size (Ko): ");
    scanf("%d", &unit_size);

    int total_units = memory_size / unit_size;

    int* bit_map = (int*)calloc(total_units, sizeof(int));

    // ML initial (كما طلبت الأستاذة)
    Node* ML = create_node('H', "", 0, 2);
    ML->next = create_node('H', "", 2, 2);
    ML->next->next = create_node('H', "", 4, 4);
    ML->next->next->next = create_node('H', "", 8, 4);
    ML->next->next->next->next = create_node('H', "", 12, 10);
    ML->next->next->next->next->next = create_node('H', "", 22, 3);
    ML->next->next->next->next->next->next = create_node('H', "", 25, 3);

    int choice;
    char name[10];
    int size;

    do {
        printf("\nMENU\n");
        printf("1. Allocate (Best Fit)\n");
        printf("2. Free Process\n");
        printf("3. Show Memory\n");
        printf("4. Fragmentation\n");
        printf("0. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch(choice) {

            case 1:
                printf("Process name: ");
                scanf("%s", name);
                printf("Size (units): ");
                scanf("%d", &size);

                best_fit(&ML, name, size, bit_map);
                print_ML(ML);
                print_bit_map(bit_map, total_units);
                break;

            case 2:
                printf("Process name to free: ");
                scanf("%s", name);

                free_process(ML, name, bit_map);
                merge_holes(ML);

                print_ML(ML);
                print_bit_map(bit_map, total_units);
                break;

            case 3:
                print_ML(ML);
                print_bit_map(bit_map, total_units);
                break;

            case 4:
                fragmentation(ML, total_units);
                break;

        }

    } while(choice != 0);

    return 0;
}