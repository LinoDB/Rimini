#include "SequenceGenerator.h"
#include <sstream>

using namespace std;

void SequenceGenerator::re_order(const int &s) {
    // finish the swap of current number 's' with the new number
    // sort all to the right and update their position in req (slow!)
    // reset to the starting position
    seq[reg[seq[pos]]] = s;
    sort(seq.begin()+pos+1, seq.end()); // bad because happens all the time
    for(int i=pos; i<len; i++) {
        reg[seq[i]] = i;
    }
    pos = start_pos;
}

SequenceGenerator::SequenceGenerator(const int l) {
    // initialize with a monotonously increasing sequence and position map
    // from 0 to l - 1
    len = l;
    start_pos = len - 2;
    for(int i=0; i<len; i++) {
        seq.push_back(i);
        reg.push_back(i);
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
    // return the positions that were moved to the right during the process
    if(d) return 0;
    int moved = 0;
    int s = seq[pos];
    while(true) {
        seq[pos]++; // increase current value
        if(seq[pos] > len - 1) {
            // end of sequence reached, there is no bigger number on the right-hand side
            // move to the left until the new start value 'seq[pos]' is smaller
            // than the old start value's'
            seq[pos] = s;
            while(true) {
                moved++;
                if(--pos < 0) {
                    d = true;
                    return moved;
                }
                if(seq[pos] < s) break;
                s = seq[pos];
            }
            s = seq[pos];
        }
        else if(reg[seq[pos]] > pos) {
            // if the current number is found on the right side, switch it with
            // the previous value s and reset the internal position 'pos'
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

string SequenceGenerator::print(const int &s_pos) {
    // return a string of comma the seperated values of the current sequence with
    // the given position 's_pos' colored red
    stringstream s;
    for(int i=0; i<len-1; i++) {
        if(i == s_pos) {
            s << "\033[31m" << seq[i] << "\033[0m,";
        }
        else {
            s << seq[i] << ',';
        }
    }
    if(len-1 == s_pos) {
        s << "\033[31m" << seq[len-1] << "\033[0m";
    }
    else {
        s << seq[len-1];
    }
    return s.str();
}

vector<int> SequenceGenerator::get_sequence() {
    // return the current sequence
    return seq;
}
