#include <iostream>
#include <vector>

using namespace std;

int getPuzzle(int puzzle[4][4]) {
    int k, l;
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
            if(puzzle[i][j] == 0) {
                k, l = i, j;
            }
        }
    }
    return k, l;
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

struct State{
    int puzzle[4][4];
    int tileRow;
    int tileCol;
    int cost;
};

int main() {
    // We get the puzzle as well as the position of empty tile

    State currState;
    int i, j = getPuzzle(currState.puzzle);
    currState.tileRow, currState.tileCol = i, j;

    printPuzzle(currState.puzzle);

    


    return 0;
}