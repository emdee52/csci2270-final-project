#include "miniGit.hpp"
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
using namespace std;


void printMenu() {
    cout << "-----------------" << endl;
    cout << "1. Add file" << endl;
    cout << "2. Remove file" << endl;
    cout << "3. Commit changes" << endl;
    cout << "4. Checkout" << endl;
    cout << "5. Quit" << endl;
}

int main() {
    string input = "";
    string checkout_input = "";
    string filename;
    string toRemove;
    int commitNumber;
    bool exists = false;
    string init;
    ifstream file;
    cout << "Do you want to initialize new repository?" << endl;
    cout << "1 = YES | 2 = NO" << endl;
    cin >> init;
    
    if(stoi(init) == 1) {
        cout << "Repository initialized." << endl;
        miniGit g;

        while (input != "5") {
            printMenu();
            cin >> input;
            cout << "-----------------" << endl;

            if (input != "1" && input != "2" && input != "3" && input != "4" && input != "5") {
                cout << "Invalid option, please enter a number between 1 and 5." << endl;

            } else if (input == "print") { //buddy
                    cout << "(DEBUG) printing..." << endl;
                    g.printList();

            } else {
                switch (stoi(input)) {
                    case 1:
                        if(g.getBlock())
                            printf("Cannot add files while checking out a different commit.\n");
                        else {                        
                            do
                            {
                                cout << "Input the name of the file you want to add:" << endl;
                                cin >> filename;
                                filename = filename;
                                file.open(filename);
                                if(!file.is_open())
                                    cout << "File doesn't exist, please enter a valid file name:" << endl;
                                else
                                    exists = true;
                            } while(!exists);
                            g.add(filename);
                        }
                        break;
                    case 2:
                        if(g.getBlock())
                            printf("Cannot remove files while checking out a different commit.\n");
                        else
                        {
                            cout << "Input the name of the file you want to remove:" << endl;
                            cin >> toRemove;
                            g.remove(toRemove);
                        }
                        break;
                    case 3:
                        if(g.getBlock())
                            printf("Cannot commit while checking out a different commit.\n");
                        else{
                            cout << "Committing... (Commit number " << g.latestCommitNum() + 1 << ")" << endl;
                            g.commit();
                        }
                        break;
                    case 4:
                        cout << "Note that while checking out a different commit, you cannot use add, remove, or commit functionality." << endl;
                        cout << "Additionally, checking out a different commit will override your current files in your working directory." << endl;
                        cout << "Do you still want to check out a different commit? Enter \"1\" for yes and anything else for no." << endl;
                        cin >> checkout_input;

                        if (checkout_input == "1") {
                            cout << "Please enter commit number you would like to check out: (0 = first commit, " << g.latestCommitNum() << " = latest commit)" << endl;
                            cin >> commitNumber;
                            g.checkout(commitNumber);
                        }
                        break;
                    case 5:
                        cout << "Goodbye! Your repository has been deleted." << endl;
                        break;
                    default:
                        cout << "Invalid option, please enter a number between 1 and 5." << endl;
                }
            }
        }
    }
}