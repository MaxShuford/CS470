// Max Shuford

#include <stdio.h>
#define PAGE_SIZE 1024
#define MAX_PAGES 16

int main(void) {
    // our fixed pages
    int page_table[4] = {5, 2, 9, 1};
    int N;

    // request logical addresses
    printf("Enter number of logical addresses (N):\n");
    scanf("%d", &N);

    int logical[N];

    printf("Enter logical address(es), one per line:\n");

    for(int i = 0; i < N; i++) {
        scanf("%d", &logical[i]);
    }

    for (int i = 0; i < N; i++) {

        int page = logical[i] / PAGE_SIZE;
        int offset = logical[i] % PAGE_SIZE;


        if (page < 0 || page > 3) {
            printf("Logical: %d | INVALID (page out of range)\n", logical[i]);
        } 
        else {
            int frame = page_table[page];
            int physical = frame * PAGE_SIZE + offset;

            printf("Logical: %d | Page: %d | Offset: %d | Frame: %d | Physical: %d\n",
                   logical[i], page, offset, frame, physical);
        }
    }

    return 0;
}