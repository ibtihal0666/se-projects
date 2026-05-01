#include <stdio.h>

int main() {
    int n, mem_size, page_size, m, pointer = 0, faults = 0;

    printf("Memory Size: "); scanf("%d", &mem_size);
    printf("Page Size: "); scanf("%d", &page_size);
    m = mem_size / page_size;
    printf("Number of frames: %d\n", m);

    printf("Number of pages: "); scanf("%d", &n);
    int pages[n], table[m][n], frames[m], reference_bits[m];
    
    for (int i = 0; i < m; i++) {
        frames[i] = -1;
        reference_bits[i] = 0;
    }

    printf("Enter the pages: ");
    for (int i = 0; i < n; i++) scanf("%d", &pages[i]);

    for (int i = 0; i < n; i++) {
        int found = 0;
        for (int j = 0; j < m; j++) {
            if (frames[j] == pages[i]) {
                reference_bits[j] = 1;
                found = 1;
                break;
            }
        }
        if (!found) {
            while (1) {
                if (reference_bits[pointer] == 0) {
                    frames[pointer] = pages[i];
                    reference_bits[pointer] = 0;
                    pointer = (pointer + 1) % m;
                    break;
                } else {
                    reference_bits[pointer] = 0;
                    pointer = (pointer + 1) % m;
                }
            }
            faults++;
        }
        for (int j = 0; j < m; j++) table[j][i] = frames[j];
    }

    // رسم الجدول المنظم
    printf("\n--- Second Chance Table ---\n\nRef: ");
    for (int i = 0; i < n; i++) printf("| %d ", pages[i]);
    printf("|\n");
    printf("-----"); for (int i = 0; i < n; i++) printf("----");
    printf("\n");

    for (int i = 0; i < m; i++) {
        printf("F%d : ", i + 1);
        for (int j = 0; j < n; j++) {
            if (table[i][j] != -1) printf("| %d ", table[i][j]);
            else printf("| - ");
        }
        printf("|\n");
    }
    printf("-----"); for (int i = 0; i < n; i++) printf("----");
    printf("\nTotal Page Faults: %d\n", faults);

    return 0;
}