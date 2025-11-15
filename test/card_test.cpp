#include "card_test.h"
#include "../src/Card.cpp"
#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

vector<int> card::check_initialization() {
    cout << "Testing Card initialization" << endl;
    vector<int> success = {0, 4};
    Card empty = Card();
    Card umbrella = Card(1, true);
    Card vec_init = Card(2, vector<int> {1, -2, -1, 2}, false, true);
    Card full_init = Card(3, 1, -1, -2, 2, true, false);

    // empty initialization
    try {
        if(empty.is_empty() != true) throw string("empty");
        if(empty.is_free() != true) throw string("free");
        if(empty.is_umbrella() != false) throw string("umbrella");
        if(empty.is_base() != false) throw string("base");
        if(empty.get_rotation() != 0) throw string("rotation");
        if(empty.needs_second_rotation() != false) throw string("second rotation");
        if(empty.get_id() != 0) throw string("id");
        if(empty.get_side(0) != 0) throw string("side 0");
        if(empty.get_side(1) != 0) throw string("side 1");
        if(empty.get_side(2) != 0) throw string("side 2");
        if(empty.get_side(3) != 0) throw string("side 3");
        success[0]++;
    } catch(string e) {
        cout << "  -> Wrong value '" << e << "' for empty card" << endl;
    }

    // umbrella initialization
    try {
        if(umbrella.is_empty() != false) throw string("empty");
        if(umbrella.is_free() != true) throw string("free");
        if(umbrella.is_umbrella() != true) throw string("umbrella");
        if(umbrella.is_base() != false) throw string("base");
        if(umbrella.get_rotation() != 0) throw string("rotation");
        if(umbrella.needs_second_rotation() != false) throw string("second rotation");
        if(umbrella.get_id() != 1) throw string("id");
        if(umbrella.get_side(0) != 0) throw string("side 0");
        if(umbrella.get_side(1) != 0) throw string("side 1");
        if(umbrella.get_side(2) != 0) throw string("side 2");
        if(umbrella.get_side(3) != 0) throw string("side 3");
        success[0]++;
    } catch(string e) {
        cout << "  -> Wrong value '" << e << "' for umbrella card" << endl;
    }

    // vector initialization
    try {
        if(vec_init.is_empty() != false) throw string("empty");
        if(vec_init.is_free() != false) throw string("free");
        if(vec_init.is_umbrella() != false) throw string("umbrella");
        if(vec_init.is_base() != false) throw string("base");
        if(vec_init.get_rotation() != 0) throw string("rotation");
        if(vec_init.needs_second_rotation() != true) throw string("second rotation");
        if(vec_init.get_id() != 2) throw string("id");
        if(vec_init.get_side(0) != 1) throw string("side 0");
        if(vec_init.get_side(1) != -2) throw string("side 1");
        if(vec_init.get_side(2) != -1) throw string("side 2");
        if(vec_init.get_side(3) != 2) throw string("side 3");
        success[0]++;
    } catch(string e) {
        cout << "  -> Wrong value '" << e << "' for vector initialized card" << endl;
    }

    // full initialization
    try {
        if(full_init.is_empty() != false) throw string("empty");
        if(full_init.is_free() != false) throw string("free");
        if(full_init.is_umbrella() != false) throw string("umbrella");
        if(full_init.is_base() != true) throw string("base");
        if(full_init.get_rotation() != 0) throw string("rotation");
        if(full_init.needs_second_rotation() != false) throw string("second rotation");
        if(full_init.get_id() != 3) throw string("id");
        if(full_init.get_side(0) != 1) throw string("side 0");
        if(full_init.get_side(1) != -1) throw string("side 1");
        if(full_init.get_side(2) != -2) throw string("side 2");
        if(full_init.get_side(3) != 2) throw string("side 3");
        success[0]++;
    } catch(string e) {
        cout << "  -> Wrong value '" << e << "' for fully initialized card" << endl;
    }

    cout << "Card initialization finished [" << success[0] << "/" << success[1] << "]\n\n";
    return success;
}

vector<int> card::check_rotation() {
    cout << "Testing Card rotation" << endl;
    Card card = Card(1, 1, -1, -2, 2);
    vector<int> success = {0, 4};

    // before first rotation
    try {
        if(card.get_rotation() != 0) throw string("rotation before 1st rotation");
        if(card.get_side(0) != 1) throw string("value before 1st rotation");
        success[0]++;
    } catch(string e) {
        cout << "  -> Wrong '" << e << "' testing card rotation" << endl;
    }

    // after first rotation
    try {
        card.rotate();
        if(card.get_rotation() != 1) throw string("rotation after 1st rotation");
        if(card.get_side(0) != -1) throw string("value after 1st rotation");
        success[0]++;
    } catch(string e) {
        cout << "  -> Wrong '" << e << "' testing card rotation" << endl;
    }

    // after third rotation
    try {
        card.rotate(2);
        if(card.get_rotation() != 3) throw string("rotation after 3rd rotation");
        if(card.get_side(3) != -2) throw string("value after 3rd rotation");
        success[0]++;
    } catch(string e) {
        cout << "  -> Wrong '" << e << "' testing card rotation" << endl;
    }

    // after five rotations
    try {
        card.rotate(2);
        if(card.get_rotation() != 1) throw string("rotation after 5th rotation");
        if(card.get_side(1) != -2) throw string("value after 5th rotation");
        success[0]++;
    } catch(string e) {
        cout << "  -> Wrong '" << e << "' testing card rotation" << endl;
    }
    cout << "Card rotation finished [" << success[0] << "/" << success[1] << "]\n\n";
    return success;
}

bool card::run_tests() {
    vector<int> init = card::check_initialization();
    vector<int> rot = card::check_rotation();
    vector<int> all = {init[0]+rot[0], init[1]+rot[1]};
    cout << "Card tests finished [" << all[0] << "/" << all[1] << "]\n";
    return all[0] == all[1];
}
