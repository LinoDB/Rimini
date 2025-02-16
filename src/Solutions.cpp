#include "Solutions.h"

using namespace std;

void Solutions::add_solution(vector<Field> solution, vector<int> sequence) {
    solutions.push_back(solution);
    sequences.push_back(sequence);
}

string Solutions::print() {
    stringstream s;
    if(solutions.size() == 0) {
        return "No solutions found!\n";
    }
    for(int i=0; i<solutions.size(); i++) {
        for(int k=0; k<sequences[i].size()-1; i++) {
            s << sequences[i][k] << ", ";
        }
        s << sequences[i][sequences[i].size()-1];
        s << " - " << solutions[i].size() << " Solutions\n";
    }
    return s.str();
}
