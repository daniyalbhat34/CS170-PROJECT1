#include <iostream>
#include <vector>
#include <queue>
#include <string>

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
    int cost = 0;

    bool operator<(const State& compState) const {
        return cost > compState.cost;  
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

string makeString(State& stringState) {
    string reqString;

    for(unsigned int i = 0; i < 4; i++) {
        for(unsigned int j = 0; j < 4; j++) {
            reqString+(to_string(stringState.puzzle[i][j]));
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
    int validMoves[4][2] = {{-1,0}, 
                            {1,0}, 
                            {0,1}, 
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
            newState.cost = checkState.cost + 1;
            children.push_back(newState);
        }
        
    }

    return children;
}

int generalSearch(State currState) {
    priority_queue<State> allStates;
    allStates.push(currState);
    
    vector<string> visited;

    // We check if the queue is empty if not we continue to iterate through  it
    while(!allStates.empty()){
        State checkState = allStates.top();
        allStates.pop();

        string currString = makeString(checkState);
        if(alreadyVisited(visited, currString)) continue;

        visited.push_back(currString);

        if(isGoalState(checkState.puzzle)) return checkState.cost;

        vector<State> children = getChildren(checkState);
        
        for(unsigned int i = 0; i < children.size(); i++) {
            allStates.push(children.at(i));
            printPuzzle(children.at(i).puzzle);
        }
    }

    // Debug mode
    return 1;
}

int main() {
    // We get the puzzle as well as the position of empty tile

    State currState;
    int i, j;
    getPuzzle(currState.puzzle, i, j);
    currState.cost = 0;
    currState.tileRow = i;
    currState.tileCol = j;

    printPuzzle(currState.puzzle);

    cout << "The cost according to Uniform Cost Search is " << generalSearch(currState) << endl;


    return 0;
}