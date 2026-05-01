#include <stdio.h>

int findOptimal(int pages[], int frames[], int n, int m, int index) {
    int res = -1, farthest = index;
    for (int i = 0; i < m; i++) {
        int j;
        for (j = index; j < n; j++) {
            if (frames[i] == pages[j]) {
                if (j > farthest) {
                    farthest = j;
                    res = i;
                }
                break;
            }
        }
        if (j == n) return i;
    }
    return (res == -1) ? 0 : res;
}

int main() {
    int n, mem_size, page_size, m, faults = 0;

    printf("Memory Size: "); scanf("%d", &mem_size);
    printf("Page Size: "); scanf("%d", &page_size);
    m = mem_size / page_size; 
    printf("Number of frames: %d\n", m);

    printf("Number of pages: "); scanf("%d", &n);
    int pages[n], table[m][n];
    int frames[m];
    for (int i = 0; i < m; i++) frames[i] = -1;

    printf("Enter the pages: ");
    for (int i = 0; i < n; i++) scanf("%d", &pages[i]);

    for (int i = 0; i < n; i++) {
        int found = 0;
        for (int j = 0; j < m; j++) {
            if (frames[j] == pages[i]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            if (i < m && frames[i] == -1) frames[i] = pages[i];
            else {
                int j = findOptimal(pages, frames, n, m, i + 1);
                frames[j] = pages[i];
            }
            faults++;
        }
        for (int j = 0; j < m; j++) table[j][i] = frames[j];
    }

    // رسم الجدول المنظم
    printf("\n--- Optimal Replacement Table ---\n\nRef: ");
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