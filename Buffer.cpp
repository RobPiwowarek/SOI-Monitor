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

    class Node{
    public:
        char A;
        bool rA, rB, rC; //readA readB readC

        Node(){
            A = '%';
            rA = rB = rC = false;
        }
    };

    std::deque<Node> buffer;

public:
    Buffer():Monitor(){
    }

    void insertA(){
        enter();
        printf("PA\n");

        while (buffer.size() == N){
            wait(prodA);
        }

        std::cout << "ProducerA producing: " ;
        pushLetter();

        printBuffer();

        signal(consA);
        signal(consB);
        signal(consC);
        leave();
    }

    void insertB(){
        enter();
        printf("PB\n");

        while (buffer.size() > N-2){
            wait(prodB);
        }

        std::cout << "ProducerB producing: " << std::endl;
        pushLetter();
        pushLetter();

        printBuffer();

        signal(consA);
        signal(consB);
        signal(consC);

        leave();
    }

    void consumeA(){
        enter();
        printf("CA\n");

        while (buffer.size() == 0){
            wait(consA);
        }

        while (buffer.front().rA){
            wait(readA);
        }

        Node node = buffer.front();

        std::cout << "ConsumerA read: " << node.A << std::endl;

        node.rA = true;

        if (node.rB){
           std::cout << "ConsumerA consuming the flesh of front letter: " << node.A << std::endl;
            buffer.pop_front();

            if (buffer.size() <= N-2) signal(prodB);

            signal(prodA);

            signal(readB);

            if (node.rC) signal(readC);
        }

        printBuffer();
        leave();
    }

    void consumeB(){
        enter();
        printf("CB\n");

        while (buffer.size() == 0){
            wait(consB);
        }

        while (buffer.front().rB){
            wait(readB);
        }

        Node node = buffer.front();

        std::cout << "ConsumerB read: " << node.A << std::endl;

        node.rB = true;

        if (node.rA){
            std::cout << "ConsumerB gnawing the bones of letter: " << node.A << std::endl;

            buffer.pop_front();

            if (buffer.size() <= N-2) signal(prodB);

            signal(prodA);

            signal(readA);

            if (node.rC) signal(readC);
        }

        printBuffer();
        leave();
    }

    void consumeC(){
        enter();
        printf("CC\n");

        while (buffer.size() == 0){
            wait(consC);
        }

        while (buffer.front().rC){
            wait(readC);
        }

        Node node = buffer.front();

       std::cout << "ConsumerC read: " << node.A << std::endl;

        node.rC = true;

        if (!node.rA && !node.rB){
          std::cout << "ConsumerC : drinking the blood of letter: " << node.A << std::endl;
            buffer.pop_front();

            if (buffer.size() <= N-2) signal(prodB);
            signal(prodA);
        }

        printBuffer();
        leave();
    }

    void printBuffer(){
      std::cout << "BUFFER SIZE: " << buffer.size() << std::endl;

        for (auto it : buffer){
           std::cout << it.A << " ";
        }
        std::cout << std::endl;


    }

    void pushLetter(){
        Node node;
        node.A = generateRandomLetter();

        std::cout << "Pushing letter: " << node.A << std::endl;

        buffer.push_back(node);
    }

};
