#include "miniGit.hpp"
#include <iostream>
#include <string>
using namespace std;


void printMenu() {
    cout << "1. Initialize new repository" << endl;
    cout << "2. Add file" << endl;
    cout << "3. Remove file" << endl;
    cout << "4. Commit changes" << endl;
    cout << "5. Checkout" << endl;
    cout << "6. Quit" << endl;
}


int main() {
    miniGit g;
    int input = -1;
    string filename;
    string toRemove;
    int commitNumber;

    while (input != 6) {
        printMenu();
        cin >> input;

        switch (input) {
            case 1:
                g.init();
                break;
            case 2:
                cin >> filename;
                g.add(filename);
                break;
            case 3:
                cin >> toRemove;
                g.remove(toRemove);
                break;
            case 4:
                g.commit();
                break;
            case 5:
                cin >> commitNumber;
                g.checkout(commitNumber);
                break;
            default:
                cout << "Invalid Option" << endl;
        }
    }
}