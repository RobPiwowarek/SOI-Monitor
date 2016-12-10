 #include <deque>
#include <cstdlib>
#include <iostream>
#include "monitor.h"

using namespace std;

#define N 10

char generateRandomLetter() {
    return (char) (rand() % (90-65) + 65);
}

class Buffer: Monitor {

private:
    Condition prodA, prodB, consA, consB, consC, readA, readB, readC;
    int size;

    class Node{
    public:
        char A;
        bool rA, rB, rC; //readA readB readC

        Node(){
            A = '%';
            rA = rB = rC = false;
        }
    };

    std::deque<Node*> buffer;

public:
    Buffer():Monitor(){
        size = 0;
    }

    void insertA(){
        enter();
        printf("PA\n");

        if (size == N){
            wait(prodA);
        }

        std::cout << "ProducerA producing: " ;
        pushLetter();

        printBuffer();

        updateSize(1);
        leave();
    }

    void insertB(){
        enter();
        printf("PB\n");

        if (size < N-2){
            wait(prodB);
        }

        std::cout << "ProducerB producing: " << std::endl;
        pushLetter();
        pushLetter();

        printBuffer();

        updateSize(2);

        leave();
    }

    void consumeA(){
        enter();
        printf("CA\n");

        if (size == 0){
            wait(consA);
        }

        if (buffer.front()->rA){
            wait(readA);
        }

        // after signal(readA);
        if (size == 0){
            leave();
            return;
        }

        Node * node = buffer.front();

        std::cout << "ConsumerA read: " << node->A << std::endl;

        node->rA = true;

        if (node->rB){
            std::cout << "ConsumerA consuming the flesh of front letter: " << node->A << std::endl;
            buffer.pop_front();

            delete node;
            updateSize(-1);

            signal(readB);

            if (node->rC) signal(readC);
        }

        printBuffer();
        leave();
    }

    void consumeB(){
        enter();
        printf("CB\n");

        if (size == 0){
            wait(consB);
        }

        if (buffer.front()->rB){
            wait(readB);
        }

        // after signal(readB);
        if (size == 0){
            leave();
            return;
        }

        Node * node = buffer.front();

        std::cout << "ConsumerB read: " << node->A << std::endl;

        node->rB = true;

        if (node->rA){
            std::cout << "ConsumerB gnawing the bones of letter: " << node->A << std::endl;
            buffer.pop_front();

            delete node;
            updateSize(-1);

            signal(readA);

            if (node->rC) signal(readC);
        }

        printBuffer();
        leave();
    }

    void consumeC(){
        enter();
        printf("CC\n");

        if (size == 0){
            wait(consC);
        }

        if (buffer.front()->rC){
            wait(readC);
        }

        // after signal(readC);
        if (size == 0){
            leave();
            return;
        }

        Node * node = buffer.front();

        std::cout << "ConsumerC read: " << node->A << std::endl;

        node->rC = true;

        if (!node->rA && !node->rB){
            std::cout << "ConsumerC : drinking the blood of letter: " << node->A << std::endl;
            buffer.pop_front();

            delete node;
            updateSize(-1);
        }

        printBuffer();
        leave();
    }

    void printBuffer(){
        std::cout << "BUFFER SIZE: " << buffer.size() << std::endl;

        for (auto it : buffer){
            std::cout << it->A << " ";
        }
        std::cout << std::endl;


    }

    void pushLetter(){
        Node * node = new Node();
        node->A = generateRandomLetter();

        std::cout << "Pushing letter: " << node->A << std::endl;

        buffer.push_back(node);
    }

    void updateSize(int diff){
        if (size+diff > 0){
            signal(consA);
            signal(consB);
            signal(consC);
        }

        if (size == N && size+diff < N-1){
            signal(prodB);
        }

        if (size == N && size+diff < N){
            signal(prodA);
        }


        size = (int)buffer.size();
    }

};