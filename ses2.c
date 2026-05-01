#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char type; // 'H' or 'P'
    char name[10]; // اسم العملية إذا كان Process
    int start;
    int length;
    struct Node* next;
} Node;

// إنشاء Node
Node* create_node(char type, char name[], int start, int length) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->type = type;
    strcpy(n->name, name);
    n->start = start;
    n->length = length;
    n->next = NULL;
    return n;
}

// طباعة ML
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

// طباعة Bit Map
void print_bit_map(int bit_map[], int total_units) {
    printf("Bit Map: ");
    for (int i = 0; i < total_units; i++)
        printf("%d", bit_map[i]);
    printf("\n");
}

// Best Fit مع تقسيم Hole
void best_fit(Node** head, char pname[], int size, int bit_map[]) {

    Node *curr = *head, *best = NULL;

    // البحث عن أفضل Hole
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

    // حالة 1: نفس الحجم
    if (best->length == size) {
        best->type = 'P';
        strcpy(best->name, pname);
    }
    // حالة 2: Hole أكبر → تقسيم
    else {
        Node* newHole = create_node('H', "", start + size, best->length - size);

        Node* newProcess = create_node('P', pname, start, size);

        // إدخال process مكان hole
        newProcess->next = newHole;
        newHole->next = best->next;

        // ربط القائمة
        if (*head == best) {
            *head = newProcess;
        } else {
            curr = *head;
            while (curr->next != best) curr = curr->next;
            curr->next = newProcess;
        }

        free(best);
    }

    printf("Process %s allocated at %d\n", pname, start);
}

// ================== MAIN ==================
int main() {

    int memory_size, unit_size;
    printf("Enter memory size (Ko): ");
    scanf("%d", &memory_size);

    printf("Enter unit size (Ko): ");
    scanf("%d", &unit_size);

    int total_units = memory_size / unit_size;

    int* bit_map = (int*)calloc(total_units, sizeof(int));

    // ML كما طلبت الأستاذة
    Node* ML = create_node('H', "", 0, 2);
    ML->next = create_node('H', "", 2, 2);
    ML->next->next = create_node('H', "", 4, 4);
    ML->next->next->next = create_node('H', "", 8, 4);
    ML->next->next->next->next = create_node('H', "", 12, 10);
    ML->next->next->next->next->next = create_node('H', "", 22, 3);
    ML->next->next->next->next->next->next = create_node('H', "", 25, 3);

    print_ML(ML);
    print_bit_map(bit_map, total_units);

    int n;
    printf("\nEnter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        char name[10];
        int size, exe;

        printf("\nProcess name: ");
        scanf("%s", name);

        printf("Size (in units): ");
        scanf("%d", &size);

        printf("Execution time: ");
        scanf("%d", &exe);

        best_fit(&ML, name, size, bit_map);

        print_ML(ML);
        print_bit_map(bit_map, total_units);
    }

    printf("\nFINAL =\n");
    print_ML(ML);
    print_bit_map(bit_map, total_units);

    return 0;
}