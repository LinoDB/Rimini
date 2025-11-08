#include "SequenceGenerator.h"

using namespace std;

void SequenceGenerator::move_position(int &s) {
    // move one sequence position left and reset the current step
    seq[pos] = s;
    if(--pos < 0) {
        d = true;
        return;
    }
    reg.insert(s);
    s = seq[pos];
}

void SequenceGenerator::re_order(const int &s) {
    // finish the swap of current numbers and sort all to the right (slow!?)
    reg.erase(seq[pos]);
    reg.insert(s);
    for(int i : reg) {
        seq[++pos] = i;
    }
    reg.clear();
    reg.insert(seq[len-1]);
    pos = start_pos;
}

SequenceGenerator::SequenceGenerator(const int l) {
    // initialize with a monotonously increasing sequence and position map
    len = l;
    start_pos = len - 2;
    for(int i=0; i<len; i++) {
        seq.push_back(i);
    }
    reg.insert(len-1);
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
        else if(reg.contains(seq[pos])) {
            re_order(s);
            break;
        }
    }
    return moved;
}

void SequenceGenerator::skip(const int &i) {
    // skip the remaining sequences of the given position
    for(int n = i+1; n<=pos; n++) {
        reg.insert(seq[n]);
    }
    for(int n = pos+1; n<=i; n++) {
        reg.erase(seq[n]);
    }
    pos = i;
}

string SequenceGenerator::print(const int &s_pos) {
    // return a string of comma the seperated values of the current sequence
    stringstream s;
    for(int i=0; i<len-1; i++) {
        if(i == s_pos) {
            s << "\033[31m" << seq[i] << "\033[0m,";
        }
        else {
            s << seq[i] << ',';
        }
    }
    s << seq[len-1];
    return s.str();
}

vector<int> SequenceGenerator::get_sequence() {
    // return the current sequence
    return seq;
}
