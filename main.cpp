#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <chrono>
#include <cmath>

using namespace std;

void getPuzzle(int (&puzzle)[3][3], int &k, int& l) {
    // Get the 8 puzzle from the user
    cout << "Enter the puzzle. \n";

    for(unsigned int i = 0; i < 3; i++) {
        for(unsigned int j = 0; j < 3; j++) {
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
    
void printPuzzle(const int (&puzzle)[3][3]) {
    cout << "The puzzle is below. \n";

    for(unsigned int i = 0; i < 3; i++) {
        for(unsigned int j = 0; j < 3; j++) {
            cout << puzzle[i][j];
            (j == 2) ? cout << ",\n":cout << ", ";
        }
    }
}

// We implement the struct state
struct State{
    int puzzle[3][3];
    int tileRow = 0;
    int tileCol = 0;
    int gCost = 0;
    int hCost = 0;

    int getfCost() const {
        return gCost + hCost;
    }

    // Comparison operator to make sure items are added to th equeue based on priority
    bool operator<(const State& compState) const {
        return hCost + gCost > compState.hCost + compState.gCost;  
    }
};

// Finds the number of tiles whichh are not at the exact location
int misplacedTiles(const State& currState) {
    int goalState[3][3] = { {1, 2, 3},
                            {4, 5, 6},
                            {7, 8, 0},
                        };
    int count = 0;
    for(unsigned int i = 0; i < 3; i++) {
        for(unsigned int j = 0; j < 3; j++) {
            if (goalState[i][j] != currState.puzzle[i][j] && currState.puzzle[i][j] != 0) count++;
        }
    }
    return count;
}

// Helper function to check if the state is our required goal state
bool isGoalState(const int (&currState)[3][3]) {
    int goalState[3][3] = { {1, 2, 3},
                            {4, 5, 6},
                            {7, 8, 0}
                        };
    for(unsigned int i = 0; i < 3; i++) {
        for(unsigned int j = 0; j < 3; j++) {
            if (goalState[i][j] != currState[i][j]) return false;
        }
    }

    return true;
}

// Helper function to convert 2D array into a string
string makeString(const State& stringState) {
    string reqString = "";

    for(unsigned int i = 0; i < 3; i++) {
        for(unsigned int j = 0; j < 3; j++) {
            reqString += (to_string(stringState.puzzle[i][j]));
        }
    }

    return reqString;
}

// Helper function to check if we already visited a state before
bool alreadyVisited(const vector<string>& visited, const string& currString) {
    for(unsigned int i = 0; i < visited.size(); i++) {
        if(visited.at(i) == currString) return true;
    }
    return false;
}

// Finds the Manhattan distance using the required formula
int manhattanDist(const State& currState) {
    int goalState[3][3] = { {1, 2, 3},
                            {4, 5, 6},
                            {7, 8, 0}
                        };
    int cost = 0;

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(currState.puzzle[i][j] != 0) {
                for(int k = 0; k < 3; k++) {
                    for(int l = 0; l < 3; l++) {
                        if(goalState[k][l] == currState.puzzle[i][j]) {
                            cost += fabs(i-k) + fabs(j-l);
                        }
                    }
                }
            }
        }
    }

    return cost;
}

vector <State>getChildren(const State &checkState, const int option) {
    // Legal moves are left, right, up and down so it's based on adding and subtracting coordinates
    int validMoves[4][2] = {{0,1},
                            {-1,0}, 
                            {1,0},  
                            {0,-1}};
    vector<State> children;
    /*We find the new row and column and see if it's within the range, only then we make it a state*/
    for(int i = 0; i < 4; i++) {
        int newBlankRow = checkState.tileRow + validMoves[i][0];
        int newBlankCol = checkState.tileCol + validMoves[i][1];
        if(newBlankRow < 3 && newBlankRow >= 0 && newBlankCol >= 0 && newBlankCol < 3) {
            State newState;
            for(unsigned int i = 0; i < 3; i++) {
                for(unsigned int j = 0; j < 3; j++) {
                    newState.puzzle[i][j] = checkState.puzzle[i][j];
                }
            }
            // Here we swap the two blocks
            int swapTile = checkState.puzzle[checkState.tileRow][checkState.tileCol];
            newState.puzzle[checkState.tileRow][checkState.tileCol] = newState.puzzle[newBlankRow][newBlankCol];
            newState.puzzle[newBlankRow][newBlankCol] = swapTile;

            newState.tileRow = newBlankRow;
            newState.tileCol = newBlankCol;
            // If we select the herustic algorithms, then we add the herustic cost
            newState.gCost = checkState.gCost + 1;
            if(option == 1) newState.hCost = misplacedTiles(newState);
            else if(option == 2) newState.hCost = manhattanDist(newState);
            children.push_back(newState);
        }
        
    }

    return children;
}

int generalSearch(State& currState, const int option, int& nodes) {
    priority_queue<State> allStates;
    allStates.push(currState);
    vector<string> visited;
    int costReq = 0;

    // We check if the queue is empty if not we continue to iterate through  it
    while(!allStates.empty()){
        /* We pop the topmost state and see if we already visited it, if no only then we continue */
        State checkState = allStates.top();
        allStates.pop();
        
        string currString = makeString(checkState);
        if(!alreadyVisited(visited, currString)) {
            visited.push_back(currString);
            cout << "The best state to expand with g(n) = " << checkState.gCost << " and h(n) = "
                << checkState.hCost <<  " is\n";
            printPuzzle(checkState.puzzle);
            if(isGoalState(checkState.puzzle)) {
                costReq = checkState.gCost;
                break;
            }
            // We get the children and push them into the priority queue
            vector<State> children = getChildren(checkState, option);

            for(unsigned int i = 0; i < children.size(); i++) {
                allStates.push(children.at(i));
                nodes++;
            }
        }

    }
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
    int nodes = 0;
    int finalCost = 0;

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

    // We also print the time required by each algorithm
    if(option == 1) {
        currState.hCost = misplacedTiles(currState);
    } else if (option == 2) {
        currState.hCost = manhattanDist(currState);
    }

    finalCost = generalSearch(currState, option, nodes);

    if(option == 0) {
        auto start = std::chrono::high_resolution_clock::now();
        cout << "The cost according to Uniform Cost Search is " << finalCost << endl;
        cout << "Nodes expanded = " << nodes << endl;
        auto stop = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        cout << "Time taken is " << (duration.count() / 1000000.0)  << " seconds" << endl;
    } else if(option == 1) {
        auto start = std::chrono::high_resolution_clock::now();
        cout << "The cost according to Misplaced tile Search is " << finalCost << endl;
        cout << "Nodes expanded = " << nodes << endl;
        auto stop = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        cout << "Time taken is " << (duration.count() / 1000000.0)  << " seconds" << endl;
    } else {
        auto start = std::chrono::high_resolution_clock::now();
        cout << "The cost according to Manhattan Search is " << finalCost << endl;
        cout << "Nodes expanded = " << nodes << endl;
        auto stop = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

        cout << "Time taken is " << (duration.count() / 1000000.0) << " seconds" << endl;
    }
    
    return 0;
}