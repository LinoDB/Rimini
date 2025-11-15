#include "sequence_generator_test.h"
#include "../src/SequenceGenerator.cpp"
#include <iostream>
#include <vector>

using namespace std;

vector<int> sequence_generator::check_initialization() {
    cout << "Testing SequenceGenerator initialization" << endl;
    vector<int> success = {0, 1};

    // initialization
    try {
        int length = 5;
        vector<int> seq = {0, 1, 2, 3, 4};
        string print_str_0 = "\033[31m0\033[0m,1,2,3,4";
        string print_str_4 = "0,1,2,3,\033[31m4\033[0m";
        SequenceGenerator s_init = SequenceGenerator(length);
        if(s_init.get_length() != length) throw string("length");
        if(s_init.get_sequence() != seq) throw string("sequence");
        if(s_init.get(1) != 1) throw string("get 1");
        if(s_init.get(3) != 3) throw string("get 3");
        if(s_init.print(0) != print_str_0) throw string("print 0");
        if(s_init.print(4) != print_str_4) throw string("print 4");
        success[0]++;
    } catch(string e) {
        cout << "  -> Wrong value '" << e << "' for SequenceGenerator initialization" << endl;
    }

    cout << "SequenceGenerator initialization finished [" << success[0] << "/" << success[1] << "]\n\n";
    return success;
}

vector<int> sequence_generator::check_generation() {
    cout << "Testing SequenceGenerator sequence generation" << endl;
    vector<int> success = {0, 1};
    // initialization
    try {
        int length = 5;
        int moved = 0;
        SequenceGenerator s_gen = SequenceGenerator(length);
        moved = s_gen.next();
        if(s_gen.get_sequence() != vector<int> {0, 1, 2, 4, 3}) throw string("next 1");
        if(moved != 0) throw string("moved after next 1");
        if(s_gen.done()) throw string("done after next 1");
        moved = s_gen.next();
        if(moved != 1) throw string("moved after next 2");
        s_gen.next();
        if(s_gen.get_sequence() != vector<int> {0, 1, 3, 4, 2}) throw string("next 3");
        if(s_gen.done()) throw string("done after next 3");
        moved = s_gen.next();
        if(moved != 1) throw string("moved after next 4");
        s_gen.skip(0);
        s_gen.next();
        s_gen.next();
        s_gen.skip(1);
        s_gen.next();
        s_gen.next();
        moved = s_gen.next();
        if(s_gen.get_sequence() != vector<int> {1, 2, 3, 0, 4}) throw string("next 7 skip 2");
        if(moved != 1) throw string("next 7 skip 2");
        if(s_gen.done()) throw string("next 7 skip 2");
        while(s_gen.get_sequence() != vector<int> {3, 0, 1, 2, 4}) {
            moved = s_gen.next();
        }
        if(moved != 3) throw string("moved on 0 position switch");
        while(!s_gen.done()) moved = s_gen.next();
        if(s_gen.get_sequence() != vector<int> {4, 3, 2, 1, 0}) throw string("on done");
        if(moved != 4) throw string("moved on done");
        success[0]++;
    } catch(string e) {
        cout << "  -> Wrong value '" << e << "' for SequenceGenerator sequence generation" << endl;
    }
    cout << "SequenceGenerator sequence generation finished [" << success[0] << "/" << success[1] << "]\n\n";
    return success;
}

bool sequence_generator::run_tests() {
    vector<int> init = sequence_generator::check_initialization();
    vector<int> gen = sequence_generator::check_generation();
    vector<int> all = {init[0]+gen[0], init[1]+gen[1]};
    cout << "SequenceGenerator tests finished [" << all[0] << "/" << all[1] << "]\n";
    return all[0] == all[1];
}
