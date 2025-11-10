#include "card_test.h"
#include "../src/Card.cpp"
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

vector<int> card::check_initialization() {
    Card empty = Card();
    Card umbrella = Card(1, true, false);
    Card noval_base = Card(2, false, true);
    Card vec_init = Card(3, vector<int> {1, -2, -1, 2}, false, true);
    Card full_init = Card(4, 1, -1, -2, 2, true, false);
    cout << "Testing Card initialization" << endl;
    vector<int> successful = {0, 5};
    try {
        if(empty.is_empty() != true) throw string("empty");
        if(empty.is_free() != true) throw string("free");
        if(empty.is_umbrella() != false) throw string("umbrella");
        if(empty.is_base() != false) throw string("base");
        if(empty.needs_second_rotation() != false) throw string("second rotation");
        if(empty.get_id() != 0) throw string("id");
        if(empty.get_side(0) != 0) throw string("side 0");
        if(empty.get_side(1) != 0) throw string("side 1");
        if(empty.get_side(2) != 0) throw string("side 2");
        if(empty.get_side(3) != 0) throw string("side 3");
        successful[0]++;
    } catch(string e) {
        cout << "  -> Wrong value '" << e << "' for empty card" << endl;
    }
    return successful;
}
bool card::run_tests() {
    vector<int> init = card::check_initialization();
    vector<int> all = {init[0], init[1]};
    cout << "Card tests finished [" << init[0] << "/" << init[1] << "]\n";
    return all[0] != all[1];
}
