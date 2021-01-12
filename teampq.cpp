/*****
 * Explaination for removeallk(n)
 * 
 * We do a linear search on the two priority queues and remove all the teams with k wins : O(n)
 * We then construct the priority queues again using fix-downs : O(n) (in course notes)
 * So total time is also of the order O(n)
 * 
 *
 *  ***/ 


#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

/* This is the Team class to store wins, losses and names of the teams */
class Team {
    private:
        int wins;
        int losses;
        string name;
    public:
    // constructor
    Team(int wins, int losses, string name) : wins{ wins }, losses{ losses }, name{ name } { }
    
    // setter and getter functions
    int getWins() { return wins; }
    int getLosses() { return losses; }
    string getName() { return name; }
    void setWins(int wins) { wins = wins; }
    void setLosses(int losses) { losses = losses; }
    void setName(string name) { name = name; }

    // function to print the name of the team to std::cout 
    void printName() {
        cout << name << endl;
    }   
};


/* Class defining a priority queue with two different priorities 

* We implement this using two priority queues to store the teams 
* with two different priorities.
* We maintain an index pair in the class to store the index of each 
* element of each priority queue so that we can easily obtain the index 
* of the priority element in the other queue while removal

*** Amortized times ***
* Insertion : (log n)
* Deletion  : (log n)
* Retrieval of highest priority item : O(1)
* Deletion of all items with specified key : O(n)
*/


class TeamPQ {
    private:
        vector<Team> maxWin;   // PQ with respect to max wins
        vector<Team> minLoss;  // PQ with respect to min losses
        int size;              // size of priority queue(number of teams)
        vector<int> arrWin;
        vector<int> arrLoss;

    // add fields/methods/constructors/destructor as necessary
    public:
    // constructor
    TeamPQ();
    
    int getSize();
    
    void fixupMax(vector<Team> &vec, int x);
    
    void fixupMin(vector<Team> &vec, int x);

    void fixdownMax(vector<Team> &vec, int size, int x);

    void fixdownMin(vector<Team> &vec, int size, int x);

    void heapifyMax(vector<Team> &vec, int size);

    void heapifyMin(vector<Team> &vec, int size);


    void insert(const Team &t);  // O(log n) time
    
    const Team &findMaxWins() const; // O(1) time

    const Team &findMinLosses() const;// O(1) time
    
    void removeMaxWins();     // O(log n) time
    
    void removeMinLosses();  // O(log n) time

    void removeAllK(int k) ;  // ??? time
    
    /* Helper Functions for printing */
    void print1() {
        for(int i = 0; i < size; i++) {
            cout << maxWin[i].getName() << endl;
        }
    }
    void print2() {
        for(int i = 0; i < size; i++) {
            cout << minLoss[i].getName() << endl;
        }
    }
    void print1arr() {
        for(int i = 0; i < size; i++) {
            cout << i << " " << arrWin[i] << endl;
        }
    }
    void print2arr() {
        for(int i = 0; i < size; i++) {
            cout << i << " " << arrLoss[i] << endl;
        }
    }
};


/* Helper Functions for insertion, deletion and remove all k */

/****** Begin *****/
int parent(int x) { return (x-1)/2; }  

int lchild(int x) { return (2*x + 1); }

int rchild(int x) { return (2*x + 2); }

void swap(Team *a, Team *b) {
    Team tmp = *a;
    *a = *b;
    *b = tmp;
}


// vec is maxWin and we update the index pair while swapping
void TeamPQ::fixupMax(vector<Team> &vec, int x) {
    while(x != 0 && vec[parent(x)].getWins() < vec[x].getWins()) {
        swap(&vec[x], &vec[parent(x)]);
        arrLoss[arrWin[x]] = parent(x);
        arrLoss[arrWin[parent(x)]] = x;
        int tmp = arrWin[x];
        arrWin[x] = arrWin[parent(x)];
        arrWin[parent(x)] = tmp;
        x = parent(x);
    }
}

// vec is minLoss and we update the index pair while swapping
void TeamPQ::fixupMin(vector<Team> &vec, int x) {
    while(x != 0 && vec[parent(x)].getLosses() > vec[x].getLosses()) {
        swap(&vec[x], &vec[parent(x)]);
        arrWin[arrLoss[x]] = parent(x);
        arrWin[arrLoss[parent(x)]] = x;
        int tmp = arrLoss[x];
        arrLoss[x] = arrLoss[parent(x)];
        arrLoss[parent(x)] = tmp;
        x = parent(x);
    }
}



// fixdown and we update the index pair while swapping
void TeamPQ::fixdownMax(vector<Team> &vec, int size, int x) {
    while (lchild(x) <= size - 1 || rchild(x) <= size - 1) {
        int j = lchild(x);
        if (j != (size - 1) && vec[j+1].getWins() > vec[j].getWins()) {
            j++;
        }
        if (vec[x].getWins() >= vec[j].getWins()) {
            break;
        }
        swap(&vec[j], &vec[x]);
        arrLoss[arrWin[x]] = parent(x);
        arrLoss[arrWin[parent(x)]] = x;
        int tmp = arrWin[x];
        arrWin[x] = arrWin[parent(x)];
        arrWin[parent(x)] = tmp;
        x = j;
    }
}


// fixdown and we update the index pair while swapping
void TeamPQ::fixdownMin(vector<Team> &vec, int size, int x) {
    while (lchild(x) <= size - 1 || rchild(x) <= size - 1) {
        int j = lchild(x);
        if (j != (size - 1) && vec[j+1].getLosses() < vec[j].getLosses()) {
            j++;
        }
        if (vec[x].getLosses() <= vec[j].getLosses()) {
            break;
        }
        swap(&vec[j], &vec[x]);
        arrWin[arrLoss[x]] = parent(x);
        arrWin[arrLoss[parent(x)]] = x;
        int tmp = arrLoss[x];
        arrLoss[x] = arrLoss[parent(x)];
        arrLoss[parent(x)] = tmp; 
        x = j;
    }
}

/**** End of Helper Functions *****/

/**** Implementation of class Functions *****/

TeamPQ::TeamPQ() {
    size = 0;
}

int TeamPQ::getSize() {
    return size;
}

void TeamPQ::insert(const Team &t) { // O(log n) time
    maxWin.emplace_back(t);
    minLoss.emplace_back(t);
    size++;
    arrWin.emplace_back(size-1);
    arrLoss.emplace_back(size-1);
    fixupMax(maxWin, size - 1);
    fixupMin(minLoss, size - 1);
}  

const Team &TeamPQ::findMaxWins() const {
    return maxWin[0];
}  // O(1) time

const Team &TeamPQ::findMinLosses() const {
    return minLoss[0];
}  // O(1) time
   
   
void TeamPQ::removeMaxWins() {
    int idx = arrWin[0];
    swap(&maxWin[0], &maxWin[size-1]);
    maxWin.erase(maxWin.end());
    swap(&minLoss[idx], &minLoss[size-1]);
    minLoss.erase(minLoss.end());
    size--;
    // int idx = getIndex()
    fixdownMax(maxWin, size, 0);
    fixdownMin(minLoss, size, idx);
}     // O(log n) time


void TeamPQ::removeMinLosses() {
    int idx = arrLoss[0];
    swap(&minLoss[0], &minLoss[size-1]);
    minLoss.erase(minLoss.end());
    swap(&maxWin[idx], &maxWin[size-1]);
    maxWin.erase(maxWin.end());
    size--;
    fixdownMin(minLoss, size, 0);
    fixdownMax(maxWin, size, idx);
}  // O(log n) time

void TeamPQ::heapifyMax(vector<Team> &vec, int size) {
    for(int i = size-1; i>=0; i--) {
        fixdownMax(vec, size, i);
    }
} 

void TeamPQ::heapifyMin(vector<Team> &vec, int size) {
    for(int i = size-1; i>=0; i--) {
        fixdownMin(vec, size, i);
    }
} 

void TeamPQ::removeAllK(int k) { // O(n) time
    int count = 0;
    vector<Team> maxtmp;
    vector<Team> mintmp;
    for(int i = 0; i < size; i++) {
        if (maxWin[i].getWins() != k) {
            maxtmp.emplace_back(maxWin[i]);
            count++;
        }
        if (minLoss[i].getWins() != k) {
            mintmp.emplace_back(minLoss[i]);
        }
    }
    maxWin = maxtmp;
    minLoss = mintmp;
    size = count;
    heapifyMax(maxWin, size);
    heapifyMin(minLoss, size);
}   


/**** End of Class Functions ******/


/*** Input/Output Main Function *****/


int main() {
    string cmd;
    TeamPQ tpq;
    while(cin >> cmd) {
        if (cmd == "i") { // insert a team into the priority queue
            int wins, losses;
            string name;
            cin >> wins >> losses >> name;
            Team tmp = { wins, losses, name };
            tpq.insert(tmp);

        } else if (cmd == "pw") { // print the name of the team with most wins
            if (tpq.getSize() == 0) {
                continue;
            }
            Team tmp = tpq.findMaxWins();
            tmp.printName();

        } else if (cmd == "pl") { // print the name of the team with least losses
            if (tpq.getSize() == 0) {
                continue;
            }
            Team tmp = tpq.findMinLosses();
            tmp.printName();

        } else if (cmd == "rw") { // remove the team with the most wins
            tpq.removeMaxWins();

        } else if (cmd == "rl") {  // remove the team with the least losses
            tpq.removeMinLosses();

        } else if (cmd == "ra") { // removes all the teams with k wins 
            int k; cin >> k;
            tpq.removeAllK(k);

        } else if (cmd == "print1") {
            tpq.print1();

        } else if (cmd == "print2") {
            tpq.print2();

        } else if (cmd == "print1arr") {
            tpq.print1arr();

        }  else if (cmd == "print2arr") {
            tpq.print2arr();
        
        } else { // not a valid command
            cout << "Invalid command" << endl;
        }
    }
    return 0;
}