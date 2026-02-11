#include <iostream>
#include <vector>

using namespace std;

void getPuzzle(int puzzle[4][4]) {
    // Get the 8 puzzle from the user
    cout << "Enter the puzzle. \n";

    for(unsigned int i = 0; i < 4; i++) {
        for(unsigned int j = 0; j < 4; j++) {
            cout << "Enter the value. Enter 0 for space ";
            cin >> puzzle[i][j];
            if(cin.fail()) {
                cout << "The system ran into an error state \n";
                exit(EXIT_FAILURE);
            }
        }
    }
}
    
void printPuzzle(int puzzle[4][4]) {
    cout << "The puzzle is below. \n";

    for(unsigned int i = 0; i < 4; i++) {
        for(unsigned int j = 0; j < 4; j++) {
            cout << puzzle[i][j];
            (j == 3) ? cout << ",\n":cout << ", ";
        }
    }
}

int main() {

    int puzzle[4][4];

    getPuzzle(puzzle);
    printPuzzle(puzzle);

    return 0;
}