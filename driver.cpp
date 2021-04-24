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
    miniGit g;
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
    
    if(stoi(init) == 1)
    {
        while (input != 5) {
            printMenu();
            cin >> input;

            switch (input) {
                case 1:
                    do
                    {
                        cout << "Input file name" << endl;
                        cin >> filename;
                        filename = filename + ".txt";
                        file.open(".minigit/" + filename);
                        if(!file.is_open())
                            cout << "File doesn't exist, enter a valid filename" << endl;
                        else
                            exists = true;
                    } while(!exists);
                    g.add(filename);
                    break;
                case 2:
                    cin >> toRemove;
                    g.remove(toRemove);
                    break;
                case 3:
                    g.commit();
                    break;
                case 4:
                    cin >> commitNumber;
                    g.checkout(commitNumber);
                    break;
                default:
                    cout << "Invalid Option" << endl;
            }
        }
    }
}