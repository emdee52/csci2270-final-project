#ifndef MINIGIT_H
#define MINIGIT_H
#include <iostream>
#include <string>
using namespace std;

struct singlyNode;

struct doublyNode {
    int commitNumber;
    singlyNode * head;
    doublyNode * previous;
    doublyNode * next;
};

struct singlyNode {
    string fileName; // Name of local file
    string fileVersion; // Name of file in .minigit folder
    string versionNumber = 00;
    singlyNode * next;
};

class miniGit {
    private:
        doublyNode *doublyHead;
        bool checkoutBlock;
        doublyNode* createNew(doublyNode *node);
    public:
        miniGit();
        ~miniGit();
        void init();
        bool checkFilename(string filename);
        void add(string filename);
        bool remove(string filename);
        void commit();
        void checkout(int commitNumber);
        bool getBlock();
        //doublyNode* search();
        bool compareFiles(singlyNode *curr);
};

#endif