#ifndef SEQUENCE_GENERATOR_H_
#define SEQUENCE_GENERATOR_H_

#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

class SequenceGenerator {
    int len;
    int pos;
    int start_pos;
    vector<int> seq;
    map<int, int> reg;
    bool d = false;

    // move one sequence position left and reset the current step
    void move_position(int &s);

    // finish the swap of current numbers and sort all to the right (slow!)
    void re_order(const int &s);

public:
    // initialize with a monotonously increasing sequence and position map
    SequenceGenerator(const int l);

    // get sequence value at index
    int get(const int &i);

    // get length of the sequence
    int get_length();

    // is 'true' when all sequences have been tested
    bool done();

    // increase the current position until it's either above the sequence
    // length or the value can be found on the right side of the current position
    int next();

    // skip the remaining sequences of the given position
    void skip(const int &i);

    // return a string of comma the seperated values of the current sequence
    string print();

    // return the current sequence
    vector<int> get_sequence();
};

#endif