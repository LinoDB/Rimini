#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

class SequenceGenerator {
        int len, start_pos;
        vector<int> seq;
        map<int, int> reg;
        int pos;
        bool d = false;

        void move_position(int &s) {
            // move one sequence position left and reset the current step
            seq[pos] = s;
            if(--pos < 0) {
                d = true;
                return;
            }
            s = seq[pos];
        }
        void re_order(const int &s) {
            // finish the swap of current numbers and sort all to the right (slow!?)
            seq[reg[seq[pos]]] = s;
            sort(seq.begin()+pos+1, seq.end()); // bad because happens all the time
            for(int i=pos; i<len; i++) {
                reg[seq[i]] = i;
            }
            pos = start_pos;
        }

    public:
        SequenceGenerator(const int &l) {
            // initialize with a monotonously increasing sequence and position map
            len = l;
            start_pos = len - 2;
            for(int i=0; i<len; i++) {
                seq.push_back(i + 1);
                reg[i + 1] = i;
            }
            pos = start_pos;
        }
        int get(const int &i) {
            // get sequence value at index
            return seq[i];
        }
        int get_length() {
            // get length of the sequence
            return len;
        }
        bool done() {
            // is 'true' when all sequences have been tested
            return d;
        }
        void next() {
            // increase the current position until it's either above the sequence
            // length or the value can be found on the right side of the current position
            if(d) return;
            int s = seq[pos];
            while(true) {
                seq[pos]++;
                if(seq[pos] > len) {
                    move_position(s);
                    if(d) break;
                }
                else if(reg[seq[pos]] > pos) {
                    re_order(s);
                    break;
                }
            }
        }
        void skip(const int &i) {
            // skip the remaining sequences of the given position
            pos = i;
        }
        string print() {
            // return a string of comma the seperated values of the current sequence
            stringstream s;
            for(int i=0; i<len-1; i++) {
                s << seq[i] << ',';
            }
            s << seq[len-1];
            return s.str();
        }
};

class Card {
    int sides[4] = {0, 0, 0, 0};
    bool umbrella;
    int rotation = 0;
    public:
        Card(int left, int up, int right, int down) {
            // initialize card with side values
            sides[0] = left;
            sides[1] = up;
            sides[2] = right;
            sides[3] = down;
            umbrella = false;
        }
        Card(vector<int> inputs) {
            // initialize card with side values
            sides[0] = inputs[0];
            sides[1] = inputs[1];
            sides[2] = inputs[2];
            sides[3] = inputs[3];
            umbrella = false;
        }
        Card(bool u) : umbrella(u) {
            // initialize card as ubrella (joker)
        }
        int get_side(const int &s) {
            // get side (0=left, 1=up, 2=right, 3=down)
            return sides[(s + rotation) % 4];
        }
        int get_rotation() {
            // get current rotation (0-3)
            return rotation;
        }
        void rotate() {
            // rotate the card left
            rotation = (rotation + 1) % 4;
        }
        string print() {
            // output the sides as a string (left, up, right, down)
            stringstream s;
            for(int i=0; i<3; i++) {
                s << sides[(i + rotation) % 4] << ',';
            }
            s << sides[(3 + rotation) % 4];
            return s.str();
        }
};

// class Field {
//     // with deep copy constructor
// };

// Field ImportField(const string &file_name) {
//     // generate card array and base field from CSVs
// };

void add_element(vector<int> &arr, const string &s) {
    if(s[0] == 'N') {
        arr[0] = 1;
    }
    else if(s[0] == 'F') {
        if(s[1] == 'L') {
            arr.push_back(1);
            return;
        }
        else if(s[1] == 'U') {
            arr.push_back(-1);
            return;
        }
    }
    else if(s[0] == 'M') {
        if(s[1] == 'L') {
            arr.push_back(2);
            return;
        }
        else if(s[1] == 'U') {
            arr.push_back(-2);
            return;
        }
    }
    arr.push_back(0);
}

vector<int> split_string(const string &s, const string &delim, const int &start=0, const int &len=0) {
    vector<int> split = {0};
    int s_pos = 0;
    int e_pos;
    while((e_pos = s.find(delim, s_pos)) != string::npos) {
        if(s_pos > 0) {
            add_element(split, s.substr(s_pos, e_pos));
        }
        s_pos = e_pos + delim.size();
    }
    if(s_pos < s.size()) {
        add_element(split, s.substr(s_pos, s.size()));
    }

    while(len > split.size()) {
        split.push_back(0);
    }
    return split;
}

vector<Card> import_cards(const string &file_name) {
    vector<Card> cards;
    string line;
    string header;
    ifstream card_csv(file_name);

    getline(card_csv, header);
    int len = split_string(header, ",").size();
    if(len < 5) {
        // make test with linebreak
        cout << "ERROR: Input file for cards must be at least 5 columns long "
        "(valid column indices: 1 - 4)" << endl;
        throw 404;
    }
    while(getline (card_csv, line)) {
        auto line_arr = split_string(line, ",", 1, len);
        if(line_arr[0]) {
            cards.push_back(Card(true));
        }
        else {
            cards.push_back(
                Card(
                    line_arr[1],
                    line_arr[2],
                    line_arr[3],
                    line_arr[4]
                )
            );
        }
    }

    card_csv.close(); 
    return cards;
};


int main() {
    vector<Card> cards = import_cards("resources/cards.csv");
    SequenceGenerator s(5);
    int count = 0;
    while(!s.done()) {
        count++;
        cout << s.print() << endl;
        if(s.get(0) == 3) {
            s.skip(0);
        }
        else if (s.get(2) == 2)
        {
            s.skip(2);
        }
        
        s.next();
    }
    cout << endl << count << " total sequences" << endl;
    Card card1(true);
    Card card2(1, 2, 3, 4);
    cout << "Card 1: " << card1.print() << endl;
    cout << "Card 2: " << card2.print() << endl;
    for(int i=0; i<6; i++) {
        cout << "After " << i << " rotations: " << card2.get_rotation();
        cout << ", sides: " << card2.print();
        cout << ", up: " << card2.get_side(1) << endl;
        card2.rotate();
    }
}
