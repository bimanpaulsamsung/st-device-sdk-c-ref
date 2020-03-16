#include "mbed.h"

#define SAMPLE_FLAG1 (1UL << 0)
#define SAMPLE_FLAG2 (1UL << 9)
#define SAMPLE_FLAG3 (1UL << 5)
#define SAMPLE_FLAG4 (1UL << 12)
#define SAMPLE_FLAG5 (1UL << 15)

EventFlags event_flags;

void worker_thread_fun()
{
    printf("Waiting for All flag from 0x%08lx.\r\n", SAMPLE_FLAG1 | SAMPLE_FLAG2);
    uint32_t flags_read = 0;
    while (true) {
        flags_read = event_flags.wait_all(SAMPLE_FLAG3 | SAMPLE_FLAG4);
        printf("Got ALL: 0x%08lx\r\n", flags_read);
        flags_read = event_flags.get();
        printf("VALUE: 0x%08lx\r\n", flags_read);
    }
}

void worker_thread_fun2()
{
    printf("Waiting for any flag from 0x%08lx.\r\n", SAMPLE_FLAG1 | SAMPLE_FLAG2);
    uint32_t flags_read = 0;
    while (true) {
        flags_read = event_flags.wait_any(SAMPLE_FLAG1 | SAMPLE_FLAG2);
        printf("Got ANY: 0x%08lx\r\n", flags_read);
        flags_read = event_flags.get();
        printf("VALUE: 0x%08lx\r\n", flags_read);
    }
}

void worker_thread_fun3()
{
	ThisThread::sleep_for(2000);
    printf("worker_thread_fun3 Waiting for any flag from 0x%08lx.\r\n", SAMPLE_FLAG5);
    uint32_t flags_read = 0;
    while (true) {
        flags_read = event_flags.wait_any(SAMPLE_FLAG5);
        printf("worker_thread_fun3 Got ANY: 0x%08lx\r\n", flags_read);
        flags_read = event_flags.get();
        printf("worker_thread_fun3 VALUE: 0x%08lx\r\n", flags_read);
    }
}

int main()
{
    Thread worker_thread;
    Thread worker_thread2;
    Thread worker_thread3;
    worker_thread.start(mbed::callback(worker_thread_fun));
    worker_thread2.start(mbed::callback(worker_thread_fun2));
    worker_thread3.start(mbed::callback(worker_thread_fun3));
    while (true) {
        ThisThread::sleep_for(1000);
        event_flags.set(SAMPLE_FLAG5);
        ThisThread::sleep_for(1000);
        event_flags.set(SAMPLE_FLAG1);
        ThisThread::sleep_for(1000);
        event_flags.set(SAMPLE_FLAG2);
        ThisThread::sleep_for(1000);
        event_flags.set(SAMPLE_FLAG3);
//        ThisThread::sleep_for(1000);
//        event_flags.set(SAMPLE_FLAG4);
    }
}
