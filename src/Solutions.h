#ifndef SOLUTIONS_H_
#define SOLUTIONS_H_

#include <vector>
#include <sstream>
#include "Field.h"

using namespace std;

class Solutions {
    vector<vector<Field>> solutions;
    vector<vector<int>> sequences;

    public:
        void add_solution(vector<Field> solution, vector<int> sequence);

        string print();
};

#endif