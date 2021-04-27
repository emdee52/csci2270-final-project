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
    string filename = curr->fileName;
    ifstream currFile;
    currFile.open(filename);
    ifstream repoFile;
    repoFile.open(".minigit/" + curr->fileVersion);
    string currFileLine;
    string repoFileLine;
    while(getline(currFile, currFileLine) && getline(repoFile, repoFileLine)){
        if(currFileLine != repoFileLine){
            return true;
        }
    }
    return false;
}

void miniGit::commit() {
    /*
    1.  The current SLL should be traversed in its entirety, and for every node
        (a)  Check whether the corresponding fileVersion file exists in .minigit directory.
            If thefileVersionfiledoes notexist, copy the file from the current directory into the.minigit directory.  
            The newly copied file should get the name from the node’s fileVersion member.   
            (Note:  this  will  only  be  the  case  when  a  file  is added to the repository for the first time.)
        (b)  If the fileVersion file does exist in .minigit, check whether the current directory file has been changed 
            (i.e.  has it been changed by the user?)  with respect to the fileVersion file.  
            (To do the comparison, you can read in the file from the current directory into one string and read in the file from the .minigit directory into another string, and check for equality.)  
        Based on the comparison result, do the following: 
            File is unchanged:  do nothing.
            File  is  changed:  copy  the  file  from  the  current  directory  to  the .minigit directory,  and  give  it  a  name  with  the  incremented  version  number.   Also,update the SLL node memberfileVersionto the incremented name.2.  Once all the files have been scanned,  the final step of the commit will create a newDoubly Linked List node of the repository.  An exact (deep) copy of the SLL from theprevious node shall be copied into the new DLL node.  The commit number of the newDLL node will be the previous nodes commit number incremented by one.
    */
   doublyNode *currDLL = search(doublyHead);
   singlyNode *currSLL = currDLL->head;
   ifstream infile;
   while(currSLL != nullptr){
       //Part (a)
       string fileversion = currSLL->fileVersion;
       infile.open(".minigit/" + fileversion);
       if(!infile.is_open()){
           infile.close();
           infile.open(currSLL->fileName);
           ofstream output(".minigit/" + fileversion);
           string line;
           while(getline(infile, line)){
               output << line << '\n';
           }
           infile.close();
           output.close();
       }
       //Part (b)
       else{
           if(compareFiles(currSLL) == true){
                infile.close();
                infile.open(currSLL->fileName);
                int newVersionNumber = stoi(currSLL->versionNumber) + 1;
                currSLL->versionNumber = to_string(newVersionNumber);
                currSLL->fileVersion = currSLL->fileVersion + currSLL->versionNumber;
                ofstream output;
                output.open((".minigit/" + currSLL->fileVersion));
                string line;
                while(getline(infile, line)){
                    output << line << '\n';
                }
                infile.close();
                output.close();
            }
       }       
       currSLL = currSLL->next;
    }
    doublyNode* newDoubly = new doublyNode;
    newDoubly->commitNumber = currDLL->commitNumber++;
    newDoubly->previous = currDLL;
    newDoubly->next = nullptr;
    currDLL->next = newDoubly;
    while(currDLL->head != nullptr){
        newDoubly->head = new singlyNode;
        newDoubly->head = currDLL->head;
        newDoubly->head->fileName = currDLL->head->fileName;
        newDoubly->head->fileVersion = currDLL->head->fileVersion;
        newDoubly->head->versionNumber = currDLL->head->versionNumber;
        currDLL->head = currDLL->head->next;
        newDoubly->head = newDoubly->head->next;
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
