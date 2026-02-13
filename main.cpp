#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <chrono>

using namespace std;

void getPuzzle(int (&puzzle)[4][4], int &k, int& l) {
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
                k= i;
                l = j;
            }
        }
    }
}
    
void printPuzzle(const int (&puzzle)[4][4]) {
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
    int tileRow = 0;
    int tileCol = 0;
    int gCost = 0;
    int hCost = 0;
    int fCost = 0;

    bool operator<(const State& compState) const {
        return gCost > compState.gCost;  
    }
};

bool isGoalState(const int (&currState)[4][4]) {
    int goalState[4][4] = { {1, 2, 3, 4},
                            {5, 6, 7, 8},
                            {9, 10, 11, 12},
                            {13, 14, 15, 0}
                        };
    for(unsigned int i = 0; i < 4; i++) {
        for(unsigned int j = 0; j < 4; j++) {
            if (goalState[i][j] != currState[i][j]) return false;
        }
    }

    return true;
}

string makeString(const State& stringState) {
    string reqString = "";

    for(unsigned int i = 0; i < 4; i++) {
        for(unsigned int j = 0; j < 4; j++) {
            reqString += (to_string(stringState.puzzle[i][j]));
        }
    }

    return reqString;
}

bool alreadyVisited(const vector<string>& visited, const string& currString) {
    for(unsigned int i = 0; i < visited.size(); i++) {
        if(visited.at(i) == currString) return true;
    }
    return false;
}

vector <State>getChildren(const State &checkState) {
    // Legal moves are left, right, up and down so it's based on adding and subtracting coordinates
    int validMoves[4][2] = {{0,1},
                            {-1,0}, 
                            {1,0},  
                            {0,-1}};
    vector<State> children;

    for(int i = 0; i < 4; i++) {
        int newBlankRow = checkState.tileRow + validMoves[i][0];
        int newBlankCol = checkState.tileCol + validMoves[i][1];
        if(newBlankRow < 4 && newBlankRow >= 0 && newBlankCol >= 0 && newBlankCol < 4) {
            State newState;
            for(unsigned int i = 0; i < 4; i++) {
                for(unsigned int j = 0; j < 4; j++) {
                    newState.puzzle[i][j] = checkState.puzzle[i][j];
                }
            }
            int swapTile = checkState.puzzle[checkState.tileRow][checkState.tileCol];
            newState.puzzle[checkState.tileRow][checkState.tileCol] = newState.puzzle[newBlankRow][newBlankCol];
            newState.puzzle[newBlankRow][newBlankCol] = swapTile;

            newState.tileRow = newBlankRow;
            newState.tileCol = newBlankCol;
            newState.gCost = checkState.gCost + 1;
            children.push_back(newState);
        }
        
    }

    return children;
}

int misplacedTies(const State& currState) {
    int goalState[4][4] = { {1, 2, 3, 4},
                            {5, 6, 7, 8},
                            {9, 10, 11, 12},
                            {13, 14, 15, 0}
                        };
    int count = 0;
    for(unsigned int i = 0; i < 4; i++) {
        for(unsigned int j = 0; j < 4; j++) {
            if (goalState[i][j] != currState.puzzle[i][j] && currState.puzzle[i][j] != 0) count++;
        }
    }
    return count;
}

int generalSearch(const State& currState, const int option) {
    priority_queue<State> allStates;
    allStates.push(currState);
    
    vector<string> visited;
    int costReq = 0;

    // We check if the queue is empty if not we continue to iterate through  it
    while(!allStates.empty()){
        State checkState = allStates.top();
        allStates.pop();
        

        string currString = makeString(checkState);
        if(!alreadyVisited(visited, currString)) {
            visited.push_back(currString);
            printPuzzle(checkState.puzzle);

            if(isGoalState(checkState.puzzle)) {
                costReq = checkState.gCost;
                break;
            }

            vector<State> children = getChildren(checkState);

            for(unsigned int i = 0; i < children.size(); i++) {
                allStates.push(children.at(i));
            }
        }

        
    }

    // Debug mode
    return costReq;
}

int main() {
    // We get the puzzle as well as the position of empty tile

    State currState;
    int i, j;
    getPuzzle(currState.puzzle, i, j);
    currState.tileRow = i;
    currState.tileCol = j;
    int option;

    printPuzzle(currState.puzzle);

    cout << "Chose the algorithm to solve the puzzle\n"
        << "Enter 0 for Uniform Cost Seatch\n"
        << "Enter 1 for Misplaced tile Seatch\n"
        << "Enter 2 for Manhattan tile Seatch\n";

    cin >> option;
    if(cin.fail()) {
        cout << "System entered in error state\n";
        exit(EXIT_FAILURE);
    }

    while(option > 2 || option < 0) {
        cout << "Chose the algorithm to solve the puzzle\n"
        << "Enter 0 for Uniform Cost Seatch\n"
        << "Enter 1 for Misplaced tile Seatch\n"
        << "Enter 2 for Manhattan tile Seatch\n";

        cin >> option;
        if(cin.fail()) {
            cout << "System entered in error state\n";
            exit(EXIT_FAILURE);
        }
    }

    if(option == 0) {
        auto start = std::chrono::high_resolution_clock::now();
        cout << "The cost according to Uniform Cost Search is " << generalSearch(currState, 0) << endl;
        auto stop = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        cout << "Time taken is " << duration.count() << " microseconds" << endl;
    } else if(option == 1) {
        auto start = std::chrono::high_resolution_clock::now();
        cout << "The cost according to Misplaced tile Search is " << generalSearch(currState, 1) << endl;
        auto stop = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        cout << "Time taken is " << duration.count() << " microseconds" << endl;
    } else{
        auto start = std::chrono::high_resolution_clock::now();
        cout << "The cost according to Manhattan Search is " << generalSearch(currState, 2) << endl;
        auto stop = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        cout << "Time taken is " << duration.count() << " microseconds" << endl;
    }
    
    return 0;
}