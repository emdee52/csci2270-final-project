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
    int input = -1;
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
        while (input != 5) {
            printMenu();
            cin >> input;

            switch (input) {
                case 1:
                    if(g.getBlock())
                        printf("Cannot add to a previous version.\n");
                    else
                    {                        
                        do
                        {
                            cout << "Input file name" << endl;
                            cin >> filename;
                            filename = filename;
                            file.open(filename);
                            if(!file.is_open())
                                cout << "File doesn't exist, enter a valid filename" << endl;
                            else
                                exists = true;
                        } while(!exists);
                        g.add(filename);
                    }
                    break;
                case 2:
                    if(g.getBlock())
                        printf("Cannot remove files from a previous version.\n");
                    else
                    {
                        cin >> toRemove;
                        g.remove(toRemove);
                    }
                    break;
                case 3:
                    g.commit();
                    break;
                case 4:
                    if(g.getBlock())
                        printf("Cannot commit a previous version.\n");
                    else
                    {    
                        cin >> commitNumber;
                        g.checkout(commitNumber);
                    }
                    break;
                default:
                    cout << "Invalid Option" << endl;
            }
        }
    }
}