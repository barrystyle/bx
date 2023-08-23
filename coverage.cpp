#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define SAMPLEWORK 1000
uint32_t work_units[SAMPLEWORK];

static void prep_workunits()
{
    for (uint32_t i = 0; i < SAMPLEWORK; i++) {
        work_units[i] = 0;
    }
}

static void print_workunits()
{
    for (int i = 0; i < SAMPLEWORK; i++) {
        printf("%d", work_units[i]);
    }
    printf("\n");
}

static void worker_thread(uint32_t thr_id, uint32_t thr_total)
{
    uint32_t increment = thr_id;
    while (increment < SAMPLEWORK) {
        work_units[increment] += 1;
        increment = increment + thr_total;
    }
}

int main()
{
    uint32_t threads = 4;
    for (uint32_t i = 0; i < threads; i++) {
        worker_thread(i, threads);
        print_workunits();
    }

    return 1;
}
