#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <string>
#include <unordered_set>
#include <sstream>

using namespace std;

class Node{
public:
    string state;   //κατάσταση σε μορφή string
    Node* parent;  //Πατρικός κόμβος
    string action;  //Ενέργεια
    int f, g, h;    //f συνολικό κόστος, g κόστος μονοπατιού, h ευριστική τιμή
    int depth;      //βάθος

    Node(string _s, Node* _p, string _a, int _g, int _h, int _d) {    //Constructor που αρχικοποιεί τα πεδία του κόμβου
        state = _s;
        parent = _p;
        action = _a;
        g = _g;
        h = _h;
        f = _g + _h;
        depth = _d;
    }

    vector<string> GetSolutionPath() const {
        vector<string> path;                            //Δημιουργεί άδειο vector τύπου strings 
        const Node* current = this;                     //Θέτει το τρέχον node στην μεταβλητη current 
        while (current && !current->action.empty()) {   //Όσο το current υπάρχει και το action του node δεν είναι empty
            path.push_back(current->action);            //Βάζει το action στο τέλος του path
            current = current->parent;                  //Αντικαταστεί το current node με το parent του
        }
        reverse(path.begin(), path.end());              //Αντιστροφή του path για καλύτερη ανάγνωση
        return path;
    }
};

struct NodeCompare {    //Σύγκριση των κόμβων σύμφωνα με το f κατά την προτεραιότητα
    bool operator()(const Node* a, const Node* b) {
        return a->f > b->f;     //Επιστρέφει true/false ανάλογα αν a>b ή όχι
    }
};

class Frontier {    //Διαχείριση της ουράς προτεραιότητας
private:
    priority_queue<Node*, vector<Node*>, NodeCompare> pr_queue;   //Κάνει κατάταξη σύμφωνα με την NodeCompare και τα κατατάσει σε πίνακα τύπου vector
public:
    void MakeQueue(Node* InitialNode) {    //Αρχικοποιεί την ουρά με τον αρχικό κόμβο
        pr_queue = priority_queue<Node*, vector<Node*>, NodeCompare>();
        pr_queue.push(InitialNode);
    }

    bool empty() const {                   //Επιστρέφει true/false αν η ουρά είναι άδεια
        return pr_queue.empty();
    }

    Node* RemoveFront() {                  //Διαγράφει το πρώτο node στην ουρά (priority queue)
        if (empty()) return nullptr;
        auto front = pr_queue.top();       //Επιστροφή
        pr_queue.pop();                    //Διαγραφή
        return front;
    }

    void QueuingMany(const vector<Node*>& nodes) {     //Βάζει τα nodes στην ουρά (priority queue)
        for (const auto& node : nodes) {
            pr_queue.push(node);
        }
    }
};

class PuzzleSolver {
private:
    int TotalNodesGenerated = 0;
    unordered_set<string> visited;  //Σύνολο καταστάσεων που έχουν επισκεφθεί

    bool CanMovePiece(const string& board, int PiecePos, int EmptyPos) const {  //Έλεγχος μετακίνησης
        int distance = abs(PiecePos - EmptyPos);                                //Υπολογίζει την απόσταση μεταξύ της επιλεγμένης θέσης και της κενής θέσης "Ε" 
        return distance > 0 && distance <= 3;                                   //true/false αν είναι στο επιτρεπόμενο όριο
    }

    int CalcHeuristic(const string& board) const {  //Συνάρτηση υπολογισμού ευριστικής h
        int hCost = 0;
        if (IsGoalState(board)) return 0;           //Aν είναι goal state, τότε h=0

        int FirstB = board.find('B');               //Βρίσκει τη θέση του πρώτου "B"  

        for (int i = 0; i < board.size(); i++) {    //Aναζήτηση των "W"
            if (board[i] == 'W') { 
                int TargetPos = FirstB - 1;         //Στόχος: πριν το πρώτο 'B' 
                hCost += abs(i - TargetPos);        //Απόσταση του "W" από τη θέση στόχου
            }
        }
        return hCost;
    }

    vector<Node*> expand(Node* node) {    //Δημιουργεί τους πιθανούς διαδόχους ενός κόμβου
        vector<Node*> successors;              //Δημιουργεί κενό vector για τους διαδόχους
        int EmptyPos = node->state.find('E');       //Βρίσκει την κενή θέση "Ε" στο τρέχον state

        for (int offset = -3; offset <= 3; offset++) {  //Εξετάζει τις πιθανές κινήσεις 
            if (offset == 0) continue;                  

            int PiecePos = EmptyPos + offset;           //Υπολογίζει τη νέα θέση που θα μετακινηθεί
            if (PiecePos >= 0 && PiecePos < node->state.length() && CanMovePiece(node->state, PiecePos, EmptyPos)) {    //Αν είναι εντός των ορίων του πίνακα και αν είναι έγκυρη σύμφωνα με την CanMovePiece
                string NewState = node->state;          //Δημιουργία νέου State
                swap(NewState[PiecePos], NewState[EmptyPos]);

                int MoveCost = abs(offset);             //Υπολογίζει το κόστος της κίνησης
                ostringstream MoveDesc;
                MoveDesc << "Move " << node->state[PiecePos]
                         << " from " << PiecePos
                         << " to " << EmptyPos
                         << " (cost: " << MoveCost << ")";
                
                int g = node->g + MoveCost;             //Υπολογίζει το κόστος μονοπατιού προσθέτωντας το κόστος κίνησης
                int h = CalcHeuristic(NewState);        //Υπολογίζει το h για το νέο state
                successors.push_back(new Node(     //Δημιουργεί νέο κόμβο σύμφωνα με τα παρακάτω
                    NewState,
                    node,                               //Ο τρέχων κόμβος ως πατρικός του νέου
                    MoveDesc.str(),
                    g,
                    h,
                    node->depth +1
                ));
            }
        }
        return successors;                              //προσθέτει το νέο κόμβο στη λίστα διαδόχων
    }

    bool IsGoalState(const string& state) const {       //Ελέγχει αν το τρέχον state είναι στόχος
        bool foundB = false;                            //Αρχικοποιεί τη foundB ως false
        for (int i=0; i<state.size(); i++) {            //Αναζήτηση στο state 
            char piece = state[i];                      //Ελέγχει την κάθε θέση του state
            if (piece == 'B') {                         
                foundB = true;                          
            }
            if (foundB && piece == 'W') {               //Δεν είναι στόχος αν βρεθεί "B" και μετά ένα "W" 
                return false;
            }
        }
        return true;
    }

public: 
    Node* solve(const string& InitialState) {      //Υλοποιεί τον A*
        Frontier frontier;                         //Δημιουργεί ένα αντικείμενο Frontier για την αποθήκευση κόμβων που πρέπει να εξεταστούν
        visited.clear();
        TotalNodesGenerated = 1;                   //Αρχικοποίηση με τον αρχικό κόμβο

        int InitialHeuristic = CalcHeuristic(InitialState);     //Υπολογίζει το h για το αρχικό state
        auto InitialNode = new Node(InitialState, nullptr, "", 0, InitialHeuristic, 0);    //Δημιουργεί τον αρχικό κόμβο με αρχική κατάσταση
        frontier.MakeQueue(InitialNode);                        //Προσθέτει τον κόμβο στην ουρά
        
        while (!frontier.empty()) {                             //Όσο η ουρά δεν είναι κενή
            auto node = frontier.RemoveFront();                 //Αφαιρεί τον κόμβο με το χαμηλότερο f

            if (!visited.insert(node->state).second) continue;  //Προσθέτει το state του κόμβου στο σύνολο  visited εκτός αν έχει ήδη επισκεφθεί
            if (IsGoalState(node->state)) return node;          //Αν το state είναι στόχος, τότε επιστρέφει τον κόμβο ως λύση

            auto successors = expand(node);                     //Δημιουργεί τους διαδόχους του τρέχοντος κόμβου
            TotalNodesGenerated += successors.size();           //Προσθήκη των successors στο σύνολο των κόμβων
            frontier.QueuingMany(successors);                   //Τους προσθέτει στην ουρά
        }
        return nullptr;
    }

    int GetNodesGenerated() const {                              //Επιστροφή των κόμβων που δημιουργήθηκαν
        return TotalNodesGenerated;
    }
};

int main() {
    string InitialBoard = "BBBWWWE";                                            //Ορισμός αρχικού state τύπου string

    PuzzleSolver solver;                                                        //Επίλυση 

    cout << "Initial State: " << InitialBoard << endl;                          //Εκτύπωση αρχικού state
    auto solution = solver.solve(InitialBoard);                                 //Κλήση της solve της κλάσης PuzzleSolver για να βρούμε λύση και αποθήκευση στη solution
    if (solution) {
        cout << "Solution found!" << endl;                                      //Αν βρεθεί solution, εκτύπωση: μήνυμα επιτυχίας, κινήσεις
        cout << "Moves:" << endl;
        for (const string& move : solution->GetSolutionPath()) {
            cout << move << endl;
        }

        cout << "Total Nodes Generated: " << solver.GetNodesGenerated() << endl;
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