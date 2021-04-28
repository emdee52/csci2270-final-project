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
    string versionNumber = "00";
    singlyNode * next;
};

class miniGit {
    private:
        doublyNode *doublyHead;
        bool checkoutBlock;
        doublyNode* createNew(doublyNode *node);
        bool compareFiles(singlyNode *curr);
        bool checkFilename(string filename);
    public:
        miniGit();
        ~miniGit();
        void init();
        void add(string filename);
        void remove(string filename);
        void commit();
        void checkout(int commitNumber);
        bool getBlock();
        void printList();
        int latestCommitNum();
};

#endif