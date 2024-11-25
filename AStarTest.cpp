#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <string>
#include <unordered_set>
#include <sstream>

using namespace std;

class AStarNode{
public:
    string state;
    AStarNode* parent;
    string action;
    int f, g, h;
    int depth;

    AStarNode(string _s, AStarNode* _p, string _a, int _g, int _h, int _d) {
        state = _s;
        parent = _p;
        action = _a;
        g = _g;
        h = _h;
        f = _g + _h;
        depth = _d;
    }

    vector<string> GetSolutionPath() const {
        vector<string> path;
        const AStarNode* current = this;
        while (current && !current->action.empty()) {
            path.push_back(current->action);
            current = current->parent;
        }
        reverse(path.begin(), path.end());
        return path;
    }
};

struct NodeCompare {
    bool operator()(const AStarNode* a, const AStarNode* b) {
        return a->f > b->f;
    }
};

class Frontier {
private:
    priority_queue<AStarNode*, vector<AStarNode*>, NodeCompare> pr_queue;
public:
    void MakeQueue(AStarNode* InitialNode) {
        pr_queue = priority_queue<AStarNode*, vector<AStarNode*>, NodeCompare>();
        pr_queue.push(InitialNode);
    }

    bool empty() const {
        return pr_queue.empty();
    }

    AStarNode* RemoveFront() {    
        if (empty()) return nullptr;
        auto front = pr_queue.top();       
        pr_queue.pop();                     
        return front;
    }

    void QueuingMany(const vector<AStarNode*>& nodes) {
        for (const auto& node : nodes) {
            pr_queue.push(node);
        }
    }
};

class PuzzleSolver {
private:
    int ExpandedNodes = 0;
    unordered_set<string> visited;

    bool CanMovePiece(const string& board, int PiecePos, int EmptyPos) const {
        int distance = abs(PiecePos - EmptyPos);
        return distance > 0 && distance <= 3;
    }


    int CalcHeuristic(const string& board) const {
    int hCost = 0;

    // Εύρεση της θέσης του πιο αριστερού 'Μ'
    int leftmostB = board.find('B');

    // Υπολογισμός ευριστικής: αποστάσεις των 'Λ' από τη θέση ακριβώς πριν από το πρώτο 'Μ'
    for (int i = 0; i < board.size(); ++i) {
        if (board[i] == 'W') {
            int targetPosition = leftmostB - 1; // Στόχος: πριν το πρώτο 'B' 
            hCost += abs(i - targetPosition);  // Απόσταση του 'Λ' από τη σωστή θέση
            //--leftmostB; // Ενημέρωση για την επόμενη θέση όπου πρέπει να πάει το επόμενο 'Λ'
        }
    }
    return hCost;
    }


    vector<AStarNode*> expand(AStarNode* node) {
        vector<AStarNode*> successors;
        int EmptyPos = node->state.find('E');

        for (int offset = -3; offset <= 3; offset++) {
            if (offset == 0) continue;

            int PiecePos = EmptyPos + offset;
            if (PiecePos >= 0 && PiecePos < node->state.length() && CanMovePiece(node->state, PiecePos, EmptyPos)) {
                string NewState = node->state;
                swap(NewState[PiecePos], NewState[EmptyPos]);

                int MoveCost = abs(offset);
                ostringstream MoveDesc;
                MoveDesc << "Move " << node->state[PiecePos]
                         << " from " << PiecePos
                         << " to " << EmptyPos
                         << " (cost: " << MoveCost << ")";
                
                int g = node->g + MoveCost;
                int h = CalcHeuristic(NewState);
                successors.push_back(new AStarNode(
                    NewState,
                    node,
                    MoveDesc.str(),
                    g,
                    h,
                    node->depth +1
                ));
            }
        }
        return successors;
    }

    bool IsGoalState(const string& state) const {
        bool foundB = false;
        for (int i=0; i<state.size(); i++) {
            char piece = state[i];
            if (piece == 'B') {
                foundB = true;
            }
            if (foundB && piece == 'W') {
                return false;
            }
        }
        return true;
    }

public: 
    AStarNode* solve(const string& InitialState) {
        Frontier frontier;
        visited.clear();
        ExpandedNodes = 0;

        int InitialHeuristic = CalcHeuristic(InitialState);
        auto InitialNode = new AStarNode(InitialState, nullptr, "", 0, InitialHeuristic, 0);
        frontier.MakeQueue(InitialNode);
        
        while (!frontier.empty()) {
            auto node = frontier.RemoveFront();
            ExpandedNodes++;

            if (!visited.insert(node->state).second) continue;
            if (IsGoalState(node->state)) return node;

            auto successors = expand(node);
            frontier.QueuingMany(successors);
        }
        return nullptr;
    }

    int GetNodesExpanded() const {
        return ExpandedNodes;
    }
};

int main() {
    string InitialBoard = "BBBWWWE";

    PuzzleSolver solver;

    cout << "Initial State: " << InitialBoard << endl;
    auto solution = solver.solve(InitialBoard);

    if (solution) {
        cout << "Solution found!" << endl; 
        cout << "Moves:" << endl;
        for (const string& move : solution->GetSolutionPath()) {
            cout << move << endl;
        }

        /*
        cout << "States in the solution path:" << endl;
        const AStarNode* current = solution;
        vector<string> states;
        while (current) {
            states.push_back(current->state);
            current = current->parent;
        }
        reverse(states.begin(), states.end());
        for (const string& state : states) {
        cout << state << endl;
        }*/

        cout << "Total Nodes Expanded: " << solver.GetNodesExpanded() << endl;
        cout << "Final State: " << solution->state << endl;
        cout << "Total Cost: " << solution->f << endl;
        cout << "Depth: " << solution->depth << endl;
        cout << "f=" << solution->f << endl;
        cout << "g=" << solution->g << endl;
        cout << "h=" << solution->h << endl;
    } else {
        cout << "No solution found.";
    }
    return 0;
}