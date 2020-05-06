#include <iostream>
#include <map>
#include <string>
#include <iterator>
#include "mbed.h"
#include "cmsis_os2.h"

#define BLINKING_RATE_MS 1000

void thread1cb(void*)
{
	while (true) {
		thread_sleep_for(BLINKING_RATE_MS);
		printf("thread1\n");
	}
}

void thread2cb(void*)
{
	while (true) {
		thread_sleep_for(BLINKING_RATE_MS);
		printf("thread2\n");
	}
}

int main()
{
    std::map<osThreadId_t, Thread *> mapOfWords;

    Thread *thread = new Thread(osPriorityNormal, stack_size, nullptr, name);
    osStatus status = thread->start(callback(thread1cb, NULL));
    mapOfWords.insert(std::make_pair(thread->get_id(), thread));

    Thread *thread2 = new Thread(osPriorityNormal, stack_size, nullptr, name);
    osStatus status = thread2->start(callback(thread2cb, NULL));
    mapOfWords.insert(std::make_pair(thread->get_id(), thread));

    std::map<osThreadId_t, Thread *>::iterator it = mapOfWords.begin();
    while(it != mapOfWords.end())
    {
    	osThreadId_t tid = it->first;
        printf("it->first: %d", tid);
        it++;
    }

    // Searching element in std::map by key.
    if(mapOfWords.find("sun") != mapOfWords.end())
        std::cout<<"word 'sun' found"<<std::endl;
    if(mapOfWords.find("mars") == mapOfWords.end())
        std::cout<<"word 'mars' not found"<<std::endl;
    return 0;
}
