#include <stdio.h>

int main() {
    int memory_size, page_size, num_frames, num_pages, page_faults = 0;

    // إدخال أحجام الذاكرة والصفحات لحساب عدد الإطارات 
    printf("Enter Memory Size : ");
    scanf("%d", &memory_size);
    printf("Enter Page Size : ");
    scanf("%d", &page_size);

    // حساب عدد الإطارات 
    num_frames = memory_size / page_size;
    printf("Number of Frames: %d\n", num_frames);

    printf("Enter number of pages to reference: ");
    scanf("%d", &num_pages);

    int pages[num_pages], frames[num_frames], counters[num_frames];
    int memory_state[num_frames][num_pages];

    printf("Enter the page reference sequence: \n");
    for (int i = 0; i < num_pages; i++) {
        scanf("%d", &pages[i]);
    }

    // تهيئة الإطارات والعدادات
    for (int i = 0; i < num_frames; i++) {
        frames[i] = -1;
        counters[i] = 0;
    }

    // تطبيق خوارزمية LRU باستخدام طريقة العداد [cite: 8, 16]
    for (int j = 0; j < num_pages; j++) {
        int hit = 0;
        for (int i = 0; i < num_frames; i++) {
            if (frames[i] == pages[j]) {
                hit = 1;
                counters[i] = j + 1; // تحديث العداد (أحدث استخدام)
                break;
            }
        }

        if (hit == 0) { // Page Fault
            int victim_index = 0;
            int found_empty = 0;

            // البحث عن إطار فارغ أولاً
            for (int i = 0; i < num_frames; i++) {
                if (frames[i] == -1) {
                    victim_index = i;
                    found_empty = 1;
                    break;
                }
            }

            // إذا كانت الذاكرة ممتلئة، نختار الصفحة ذات العداد الأصغر (الأبعد في الماضي) [cite: 8]
            if (!found_empty) {
                int min_val = counters[0];
                for (int i = 1; i < num_frames; i++) {
                    if (counters[i] < min_val) {
                        min_val = counters[i];
                        victim_index = i;
                    }
                }
            }

            frames[victim_index] = pages[j];
            counters[victim_index] = j + 1;
            page_faults++;
        }

        // حفظ حالة الذاكرة لعرضها لاحقاً 
        for (int i = 0; i < num_frames; i++) {
            memory_state[i][j] = frames[i];
        }
    }

    // عرض الجدول النهائي (الصفوف هي الإطارات، والأعمدة هي الصفحات) 
    printf("\nRESULT =\n");
    printf("Page Ref | ");
    for (int i = 0; i < num_pages; i++) printf("%d ", pages[i]);
    printf("\n---------|---------------------\n");

    for (int i = 0; i < num_frames; i++) {
        printf("Frame %d  | ", i + 1);
        for (int j = 0; j < num_pages; j++) {
            if (memory_state[i][j] == -1) printf("- ");
            else printf("%d ", memory_state[i][j]);
        }
        printf("\n");
    }

    printf("\nTotal Page Faults: %d\n", page_faults); // 
    return 0;
}