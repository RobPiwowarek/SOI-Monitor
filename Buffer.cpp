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
    Condition prodA, prodB, consA, consB, consC;

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
            std::cout << "PA STOI" << std::endl;
            wait(prodA);
        }

        std::cout << "ProducerA producing: " ;
        pushLetter();

        printBuffer();

        if (buffer.size() > 0) {
            std::cout << "CA RUSZA" << std::endl;
            signal(consA);
        }

        if (buffer.size() > 0) {
            std::cout << "CB RUSZA" << std::endl;
            signal(consB);
        }

        if (buffer.size() > 0) {
            std::cout << "CC RUSZA" << std::endl;
            signal(consC);
        }

        leave();
    }

    void insertB(){
        enter();
        printf("PB\n");

        while (buffer.size() > N-2){
            std::cout << "PB STOI" << std::endl;
            wait(prodB);
        }

        std::cout << "ProducerB producing: " << std::endl;
        pushLetter();
        pushLetter();

        printBuffer();

        if (buffer.size() > 0) {
            std::cout << "CA RUSZA" << std::endl;
            signal(consA);
        }

        if (buffer.size() > 0) {
            std::cout << "CB RUSZA" << std::endl;
            signal(consB);
        }

        if (buffer.size() > 0) {
            std::cout << "CC RUSZA" << std::endl;
            signal(consC);
        }

        leave();
    }

    void consumeA(){
        enter();
        printf("CA\n");

        while (buffer.size() == 0 || buffer.front().rA){
            std::cout << "CA STOI" << std::endl;
            wait(consA);
        }

        Node node = buffer.front();

        std::cout << "ConsumerA read: " << node.A << std::endl;

        buffer.front().rA = true;

        if (node.rB){
            std::cout << "ConsumerA consuming the flesh of front letter: " << node.A << std::endl;
            buffer.pop_front();

            if (buffer.size() <= N-2) {
                std::cout << "PB RUSZA" << std::endl;
                signal(prodB);
            }

            if (buffer.size() <= N-1) {
                std::cout << "PA RUSZA" << std::endl;
                signal(prodA);
            }

        }

        printBuffer();
        leave();
    }

    void consumeB(){
        enter();
        printf("CB\n");

        while (buffer.size() == 0 || buffer.front().rB){
            std::cout << "CB STOI" << std::endl;
            wait(consB);
        }

        Node node = buffer.front();

        std::cout << "ConsumerB read: " << node.A << std::endl;

        buffer.front().rB = true;

        if (node.rA){
            std::cout << "ConsumerB gnawing the bones of letter: " << node.A << std::endl;

            buffer.pop_front();

            if (buffer.size() <= N-2) {
                std::cout << "PB RUSZA" << std::endl;
                signal(prodB);
            }

            if (buffer.size() <= N-1) {
                std::cout << "PA RUSZA" << std::endl;
                signal(prodA);
            }

        }

        printBuffer();
        leave();
    }

    void consumeC(){
        enter();
        printf("CC\n");

        while (buffer.size() == 0 || buffer.front().rC){
            std::cout << "CC STOI" << std::endl;
            wait(consC);
        }

        Node node = buffer.front();

        std::cout << "ConsumerC read: " << node.A << std::endl;

        buffer.front().rC = true;

        if (!node.rA && !node.rB){
            std::cout << "ConsumerC : drinking the blood of letter: " << node.A << std::endl;
            buffer.pop_front();

            if (buffer.size() <= N-2) {
                std::cout << "PB RUSZA" << std::endl;
                signal(prodB);
            }

            if (buffer.size() <= N-1) {
                std::cout << "PA RUSZA" << std::endl;
                signal(prodA);
            }
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
