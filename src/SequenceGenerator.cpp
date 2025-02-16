#include "SequenceGenerator.h"

using namespace std;

void SequenceGenerator::move_position(int &s) {
    // move one sequence position left and reset the current step
    seq[pos] = s;
    if(--pos < 0) {
        d = true;
        return;
    }
    s = seq[pos];
}

void SequenceGenerator::re_order(const int &s) {
    // finish the swap of current numbers and sort all to the right (slow!?)
    seq[reg[seq[pos]]] = s;
    sort(seq.begin()+pos+1, seq.end()); // bad because happens all the time
    for(int i=pos; i<len; i++) {
        reg[seq[i]] = i;
    }
    pos = start_pos;
}

SequenceGenerator::SequenceGenerator(const int l) {
    // initialize with a monotonously increasing sequence and position map
    len = l;
    start_pos = len - 2;
    for(int i=0; i<len; i++) {
        seq.push_back(i);
        reg[i] = i;
    }
    pos = start_pos;
}

int SequenceGenerator::get(const int &i) {
    // get sequence value at index
    return seq[i];
}

int SequenceGenerator::get_length() {
    // get length of the sequence
    return len;
}

bool SequenceGenerator::done() {
    // is 'true' when all sequences have been tested
    return d;
}

int SequenceGenerator::next() {
    // increase the current position until it's either above the sequence
    // length or the value can be found on the right side of the current position
    if(d) return 0;
    int moved = 0;
    int s = seq[pos];
    while(true) {
        seq[pos]++;
        if(seq[pos] > len - 1) {
            move_position(s);
            moved++;
            if(d) break;
        }
        else if(reg[seq[pos]] > pos) {
            re_order(s);
            break;
        }
    }
    return moved;
}

void SequenceGenerator::skip(const int &i) {
    // skip the remaining sequences of the given position
    pos = i;
}

string SequenceGenerator::print() {
    // return a string of comma the seperated values of the current sequence
    stringstream s;
    for(int i=0; i<len-1; i++) {
        s << seq[i] << ',';
    }
    s << seq[len-1];
    return s.str();
}

vector<int> SequenceGenerator::get_sequence() {
    // return the current sequence
    return seq;
}
