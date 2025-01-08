#include <iostream>
#include <sstream>
#include <map>
#include <algorithm>

using namespace std;

class SequenceGenerator {
        static const int len = 5;
        const int start_pos = len - 2;
        int seq[len];
        map<int, int> reg;
        int pos;
        bool d = false;

        void move_position(int &s) {
            // move one sequence position left and reset the current step
            seq[pos] = s;
            s = seq[--pos];
            if(pos < 0) d = true;
        }
        void re_order(const int &s) {
            // finish the swap of current numbers and sort all to the right (slow!?)
            seq[reg[seq[pos]]] = s;
            // reg[s] = reg[seq[pos]];
            // reg[seq[pos]] = pos;
            sort(&seq[pos + 1], &seq[len]); // bad because happens all the time
            for(int i=pos; i<len; i++) {
                reg[seq[i]] = i;
            }
            pos = start_pos;
        }

    public:
        SequenceGenerator() {
            // initialize with a monotonously increasing sequence and position map
            for(int i=0; i<len; i++) {
                seq[i] = i + 1;
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
        Card(bool u) {
            // initialize card as ubrella (joker)
            umbrella = u;
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

class Field {

};


int main() {
    SequenceGenerator s;
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
