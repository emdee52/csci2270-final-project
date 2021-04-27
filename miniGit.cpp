#include <iostream>
#include "miniGit.hpp"
#include <filesystem>
#include <fstream>
using namespace std;
namespace fs = filesystem;



miniGit::miniGit() {
    doublyHead = NULL;
    init();
}

miniGit::~miniGit() {
    doublyNode* crawler = doublyHead;
    doublyNode* temp;
    
    while (crawler != NULL) {
        temp = crawler;
        crawler = temp->next;
        
        singlyNode* singlyCrawler = temp->head;
        singlyNode* singlyTemp;
        while (singlyCrawler != NULL) {
            singlyTemp = singlyCrawler;
            singlyCrawler = singlyTemp->next;
            delete singlyTemp;
        }
        delete temp;
    }

    fs::remove_all(".minigit");
}

void miniGit::init() {
    fs::create_directory(".minigit");
    doublyNode * temp = new doublyNode;
    temp->commitNumber = 0;
    temp->head = nullptr;
    temp->previous = nullptr;
    temp->next = nullptr;
    doublyHead = temp;
    cout << "init called" << endl;
}

bool miniGit::checkFilename(string filename){
  cout << "checked filename" << endl;
  singlyNode * temp = doublyHead->head;

  while(temp != nullptr)
  {
    if(temp->fileName == filename)
      return true;
    else
      temp = temp->next;
  }
  return false;
}

doublyNode* search(doublyNode * temp) {
    cout << "search called" << endl;
  while(temp->next != nullptr){
      temp = temp->next;
      cout << "searching" << endl;
  }
  cout << "search end" << endl;
  return temp;
}

void miniGit::add(string filename) {
  bool exists = false;
  cout << "add called" << endl;
  if(checkFilename(filename) == false){
    cout << "checked file" << endl;
    singlyNode *temp = new singlyNode;
    doublyNode *commit = search(doublyHead);
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
  cout << "add func ended" << endl;
}

bool miniGit::remove(string filename) {
    // Check the SLL for whether the file exists in the current version of the repository.
    // If found, delete the SLL node.

    doublyNode *current = doublyHead;
    while (current->next != NULL) {
        current = current->next;
    }

    singlyNode *crawler = current->head;
    singlyNode *prev = NULL;
    while (crawler->fileName != filename && crawler != NULL) {
        prev = crawler;
        crawler = crawler->next;
    }
    
    if (crawler == NULL) {
        cout << "ERROR: file " << filename << " not found in minigit." << endl;
        return false;

    } else {
        if (prev == NULL) {
            current->head = crawler->next;
            delete crawler;
        } else {
            prev->next = crawler->next;
            delete crawler;
        }

        return true;
    }
}

bool miniGit::compareFiles(singlyNode *curr){
    cout << "entered compare func" << endl;
    string filename = curr->fileName;
    ifstream currFile;
    currFile.open(filename);
    ifstream repoFile;
    repoFile.open(".minigit/" + curr->fileVersion);
    string currFileLine;
    string repoFileLine;
    while(true){
        cout << "entered compare while loop" << endl;
        if(!getline(currFile, currFileLine) || !getline(repoFile, repoFileLine)){
            return true;
        }
        if(currFileLine != repoFileLine){
            return true;
        }
        if(currFile.eof() && repoFile.eof()){
            return false;
        }
    }
    cout << "exited while loop" << endl;
    return false;
}

void miniGit::commit() {
    doublyNode *currDLL = doublyHead;
    while (currDLL->next != NULL) {
        currDLL = currDLL->next;
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

        } else { // file exists
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

    doublyNode *newDLL = new doublyNode;
    newDLL->commitNumber = currDLL->commitNumber + 1;
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

void miniGit::checkout(int commitNumber) {
    doublyNode * current = search(doublyHead);
    doublyNode * temp = doublyHead;
    if(commitNumber != current->commitNumber)
        checkoutBlock = true;

    while(temp->next->commitNumber != commitNumber)
        temp = temp->next;

    ifstream currFile;
    ifstream commitFile;
    ofstream output;
    string l1, l2;
    while(temp->head != nullptr){
        currFile.open(temp->head->fileName);
        commitFile.open(".minigit/" + temp->head->fileVersion);
        output.open(temp->head->fileName);
        while(getline(commitFile, l2)){
            getline(currFile, l1);
            output << l2;
        }
    }
    currFile.close(), commitFile.close(), output.close();
    cout << "Check out your requested commit file in your current directory!" << endl;
}

bool miniGit::getBlock() {
    return checkoutBlock;
}
