#include <iostream>
#include "miniGit.hpp"
#include <filesystem>
#include <fstream>
using namespace std;
namespace fs = filesystem;

miniGit::miniGit() { //constructor
    doublyHead = NULL;
    init(); //calls initializer func
}

miniGit::~miniGit() { //destructor
    doublyNode* crawler = doublyHead;
    doublyNode* temp;
    
    while (crawler != NULL) { //traverses through DLL and deletes
        temp = crawler;
        crawler = temp->next;
        
        singlyNode* singlyCrawler = temp->head;
        singlyNode* singlyTemp;
        while (singlyCrawler != NULL) { //nested loop traverses through SLL and deletes
            singlyTemp = singlyCrawler;
            singlyCrawler = singlyTemp->next;
            delete singlyTemp;
        }
        delete temp;
    }

    fs::remove_all(".minigit"); //removes .minigit directory and files inside it
}

void miniGit::init() {
    fs::create_directory(".minigit"); //creates .minigit directory
    doublyNode * temp = new doublyNode;
    temp->commitNumber = 0;
    temp->head = nullptr;
    temp->previous = nullptr;
    temp->next = nullptr;
    doublyHead = temp;
}

bool miniGit::checkFilename(string filename){ //checks if file exists in SLL
  singlyNode * temp = doublyHead->head;

  while(temp != nullptr)
  {
    if(temp->fileName == filename) //true if exists in current SLL node
      return true;
    else //else moves to next SLL node
      temp = temp->next;
  }
  return false;
}

doublyNode* search(doublyNode * temp) { //finds the last DLL node
  while(temp->next != nullptr){ //searches until EODLL
      temp = temp->next;
  }
  return temp;
}

void miniGit::add(string filename) {//adds files to commit
    bool exists = false;
    if(checkFilename(filename) == false){ //checks if file exists already
        singlyNode *temp = new singlyNode;
        doublyNode *commit = search(doublyHead); //getting latest commit
        temp->versionNumber = "00";
        string fileversion = filename + temp->versionNumber;
        temp->fileName = filename;
        temp->fileVersion = fileversion;
        temp->next = nullptr;
        
        if(doublyHead->head == nullptr)
            doublyHead->head = temp;
        else
        {
            singlyNode *crawler = doublyHead->head;
            while(crawler->next != nullptr)
                crawler = crawler->next;
            crawler->next = temp;
        }
    }
}

void miniGit::remove(string filename) { //removes file from SLL
    // Check the SLL for whether the file exists in the current version of the repository.
    // If found, delete the SLL node.
    doublyNode *current = search(doublyHead);

    if(current->head == NULL) {
        cout << "ERROR: Can't remove file, no files currently added."<< endl;
        return;
    }

    singlyNode *crawler = current->head;
    singlyNode *prev = NULL;
    while (crawler != NULL) {
        if(crawler->fileName == filename) {
            if(prev == NULL) {
                current->head = crawler->next;
                delete crawler;

            } else {
                prev->next = crawler->next;
                delete crawler;              
            }
            return;
        }
        prev = crawler;
        crawler = crawler->next;
    }
    
    if (crawler == NULL) {
        cout << "ERROR: file " << filename << " not found in minigit." << endl;
    }
}

bool miniGit::compareFiles(singlyNode *curr) { // returns true if there was a change in the file, false otherwise.
    ifstream directory(curr->fileName);
    ifstream git(".minigit/" + curr->fileName + curr->versionNumber);
    string dline;
    string gline;
    string d_full = "";
    string g_full = "";

    while (getline(directory, dline)) {
        d_full += dline;
    }
    while (getline(git, gline)) {
        g_full += gline;
    }

    return g_full != d_full; //returns 0 if not equal || returns 1 if equal
}

void miniGit::commit() { // commits added files to .minigit repository
    doublyNode *currDLL = search(doublyHead);

    if(currDLL->head == NULL)
    {
        cout << "ERROR: Can't commit file, no files currently added."<< endl;
        return;
    }

    singlyNode *currSLL = currDLL->head;
    ifstream gitfile;
    while (currSLL != NULL) {
        gitfile.open(".minigit/" + currSLL->fileName + currSLL->versionNumber);

        if (!gitfile.is_open()) { // file doesn't exist
            ofstream out_file(".minigit/" + currSLL->fileName + currSLL->versionNumber);
            ifstream in_file(currSLL->fileName);
            string line;
            while(getline(in_file, line)) {
                out_file << line << endl;
            }
            out_file.close();
            in_file.close();

        } else { // new file version exists
            if (compareFiles(currSLL)) {
                int newVer = stoi(currSLL->versionNumber) + 1;

                if (newVer < 10) {
                    currSLL->versionNumber = "0" + to_string(newVer);
                } else {
                    currSLL->versionNumber = to_string(newVer);
                }
                currSLL->fileVersion = currSLL->fileName + currSLL->versionNumber;
                //cout << currSLL->versionNumber << endl;

                ofstream out_file(".minigit/" + currSLL->fileName + currSLL->versionNumber);
                ifstream in_file(currSLL->fileName);
                string line;
                while(getline(in_file, line)) {
                    out_file << line << endl;
                }
                out_file.close();
                in_file.close();
            }
        }
        gitfile.close();
        currSLL = currSLL->next;
    }
    // creates new DLL node and sets pointers accordingly
    doublyNode *newDLL = new doublyNode;
    int prevCommit = currDLL->commitNumber;
    newDLL->commitNumber = prevCommit + 1;
    newDLL->previous = currDLL;
    newDLL->next = NULL;
    currDLL->next = newDLL;

    currSLL = currDLL->head;
    singlyNode *prev = NULL;
    while (currSLL != NULL) {
        singlyNode *newSLL = new singlyNode;
        newSLL->fileName = currSLL->fileName;
        newSLL->versionNumber = currSLL->versionNumber;
        newSLL->fileName = currSLL->fileName;
        newSLL->fileVersion = currSLL->fileVersion;
        newSLL->next = NULL;

        if (prev == NULL) {
            newDLL->head = newSLL;
        } else {
            prev->next = newSLL;
        }
        prev = newSLL;
        currSLL = currSLL->next;
    }

}

void miniGit::checkout(int commitNumber) { // check out a different commit
    doublyNode * current = search(doublyHead);
    doublyNode * temp = doublyHead;
    
    if(current && commitNumber != current->commitNumber)
        checkoutBlock = true;

    if(current && commitNumber + 1 == current->commitNumber)
        checkoutBlock = false;

    while(temp && temp->next && temp->commitNumber != commitNumber)
        temp = temp->next;
        
    if(commitNumber + 1 > current->commitNumber || commitNumber < 0)
    {
        cout << "ERROR: Commit does not exist! Please try again." << endl;
        return;
    }

    ifstream currFile;
    ifstream commitFile;
    ofstream output;
    string l1, l2;

    singlyNode* sll = temp->head;
    while(sll != nullptr){
        currFile.open(sll->fileName);
        commitFile.open(".minigit/" + sll->fileVersion);
        output.open((sll->fileName), ofstream::trunc);
        while(getline(commitFile, l2)){
            getline(currFile, l1);
            output << l2 << endl;
        }
        currFile.close(), commitFile.close(), output.close();
        sll = sll->next;
    }
    cout << "Check out your requested commit file in your current directory!" << endl;
}

bool miniGit::getBlock() {//gets block for restricting add,remove,commit when checking out
    return checkoutBlock;
}

void miniGit::printList() { // used for debugging and printing singlyLinkedList
    doublyNode *dcrawler = doublyHead;
    singlyNode *scrawler = NULL;

    while (dcrawler != NULL) {
        cout << "|" << dcrawler->commitNumber << "|";
        scrawler = dcrawler->head;

        while (scrawler != NULL) {
            cout << " -> " << scrawler->fileVersion;
            scrawler = scrawler->next;
        }

        dcrawler = dcrawler->next;
        cout << endl;
    }
}

int miniGit::latestCommitNum() { // returns latest commit number minus one because of our naming choice
    doublyNode *crawler = doublyHead;

    if (crawler == NULL)
        return -1;

    while (crawler->next != NULL) {
        crawler = crawler->next;
    }

    return crawler->commitNumber - 1;
}