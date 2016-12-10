#include <iostream>
#include <stdlib.h>
#include "monitor.h"
#include "Buffer.cpp"

Buffer buffer;

void *producerA(void *){
    while(true){
        usleep(rand()%600);
        buffer.insertA();
    }
}

void *producerB(void *){
    while(true){
        usleep(rand()%400);
        buffer.insertB();
    }
}

void *consumerA(void *){
    while(true){
        usleep(rand()%100);
        buffer.consumeA();
    }
}

void *consumerB(void *){
    while(true){
        usleep(rand()%50);
        buffer.consumeB();
    }
}

void *consumerC(void *){
    while(true){
        usleep(rand()%150);
        buffer.consumeC();
    }
}


int main() {
    pthread_t pA, pB, cA, cB, cC;

    srand((unsigned) time(NULL));

    pthread_create(&pA, NULL, producerA, NULL);
    pthread_create(&pB, NULL, producerB, NULL);
    pthread_create(&cA, NULL, consumerA, NULL);
    pthread_create(&cB, NULL, consumerB, NULL);
    pthread_create(&cC, NULL, consumerC, NULL);

    pthread_join(pA, NULL);
    pthread_join(pB, NULL);
    pthread_join(cA, NULL);
    pthread_join(cB, NULL);
    pthread_join(cC, NULL);

    return 0;
}
