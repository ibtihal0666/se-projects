#include <stdio.h>
#include <stdbool.h>

// دالة مساعدة لطباعة حالة الذاكرة
void print_memory(int frames[], int num_frames) {
    for (int i = 0; i < num_frames; i++) {
        if (frames[i] == -1) printf(" - ");
        else printf(" %d ", frames[i]);
    }
}

int main() {
    int memory_size, page_size, num_frames, num_pages, choice;

    // Session 1: إعدادات الذاكرة وحساب الإطارات [cite: 14]
    printf("Enter Memory Size: ");
    scanf("%d", &memory_size);
    printf("Enter Page Size: ");
    scanf("%d", &page_size);

    num_frames = memory_size / page_size;
    printf("Number of Frames: %d\n", num_frames);

    printf("Enter number of pages: ");
    scanf("%d", &num_pages);

    int pages[num_pages];
    printf("Enter the page reference sequence: ");
    for (int i = 0; i < num_pages; i++) scanf("%d", &pages[i]);

    // Session 1 & 2: قائمة اختيار الخوارزمية [cite: 15]
    printf("\n--- Choose Algorithm ---\n1. LRU (Counter Method)\n2. Optimal (with First Fit for ties)\n3. Second Chance (R-bit)\nChoice: ");
    scanf("%d", &choice);

    int frames[num_frames], counters[num_frames], r_bits[num_frames];
    int page_faults = 0;
    int pointer = 0; // لمبدأ FIFO في Second Chance

    for (int i = 0; i < num_frames; i++) {
        frames[i] = -1;
        counters[i] = 0;
        r_bits[i] = 0;
    }

    printf("\nSimulation Table:\nRef | Memory State\n----|--------------\n");

    for (int j = 0; j < num_pages; j++) {
        int hit = -1;
        // البحث هل الصفحة موجودة مسبقاً (Hit)
        for (int i = 0; i < num_frames; i++) {
            if (frames[i] == pages[j]) {
                hit = i;
                break;
            }
        }

        if (hit != -1) {
            if (choice == 1) counters[hit] = j + 1; // تحديث عداد LRU [cite: 8]
            if (choice == 3) r_bits[hit] = 1;      // تحديث بت المرجع [cite: 10]
        } else {
            page_faults++;
            int victim_index = -1;

            // ملء الإطارات الفارغة أولاً (First Fit)
            for (int i = 0; i < num_frames; i++) {
                if (frames[i] == -1) {
                    victim_index = i;
                    break;
                }
            }

            // إذا كانت الذاكرة ممتلئة، نبحث عن الضحية
            if (victim_index == -1) {
                if (choice == 1) { // LRU [cite: 16]
                    int min = counters[0]; victim_index = 0;
                    for (int i = 1; i < num_frames; i++) {
                        if (counters[i] < min) { min = counters[i]; victim_index = i; }
                    }
                } 
                else if (choice == 2) { // Optimal [cite: 18]
                    int farthest = -1;
                    victim_index = 0; // افتراض الأول كـ First Fit في حال التساوي

                    for (int i = 0; i < num_frames; i++) {
                        int k;
                        // البحث عن الصفحة في المستقبل 
                        for (k = j + 1; k < num_pages; k++) {
                            if (frames[i] == pages[k]) break;
                        }
                        
                        // إذا وجدنا صفحة تُستخدم في وقت أبعد من الحالية، نحدث الضحية
                        // "k" ستكون مساوية لـ num_pages إذا لم تُطلب الصفحة أبداً
                        if (k > farthest) {
                            farthest = k;
                            victim_index = i;
                        }
                    }
                } 
                else if (choice == 3) { // Second Chance [cite: 10, 18]
                    while (true) {
                        if (r_bits[pointer] == 0) {
                            victim_index = pointer;
                            pointer = (pointer + 1) % num_frames;
                            break;
                        }
                        r_bits[pointer] = 0; // تغيير R إلى 0 وإعطاء فرصة [cite: 10]
                        pointer = (pointer + 1) % num_frames;
                    }
                }
            }
            frames[victim_index] = pages[j];
            if (choice == 1) counters[victim_index] = j + 1;
            if (choice == 3) r_bits[victim_index] = 1;
        }

        // عرض حالة الذاكرة الحالية [cite: 15, 19]
        printf("%d   | ", pages[j]);
        print_memory(frames, num_frames);
        printf("\n");
    }

    printf("\nTotal Page Faults: %d\n", page_faults);
    return 0;
}