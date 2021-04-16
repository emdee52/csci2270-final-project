#ifndef MINIGIT_H
#define MINIGIT_H
#include <iostream>
#include <string>
using namespace std;

struct doublyNode {
    int commitNumber;
    singlyNode * head;
    doublyNode * previous;
    doublyNode * next;
};

struct singlyNode {
    string fileName; // Name of local file
    string fileVersion; // Name of file in .minigit folder
    singlyNode * next;
};

class miniGit {
    private:
        doublyNode *head;

        doublyNode createNew();
    public:
        miniGit();
        ~miniGit();
        void init();
        void add(string filename);
        void remove(string filename);
        void commit();
        void checkout(int commitNumber);
        doublyNode search(string filename);
};

#endif