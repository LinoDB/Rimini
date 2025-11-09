#include "Game.h"

using namespace std;

void Game::reset_count() {
    // reset the map keys for current fields
    vector<int> keys;
    for(auto pair: fields) {
        keys.push_back(pair.first);
    }
    field_count = 0;
    for(int key: keys) {
        // use this to check if copying works properly!!! - and do research!
        Field field = fields[key];
        fields.erase(key);
        fields[field_count++] = field;
    }
}

Game::Game(Field f) {
    fields[0] = f;
    field_count++;
}

bool Game::add_card(Card card) {
    // add a card to all current fields
    bool check_valid = false;
    if(field_count > 100000) {
        reset_count();
    }
    vector<int> keys;
    for(auto pair : fields) {
        keys.push_back(pair.first);
    }
    for(int i : keys) {
        int check = fields[i].add_card(card);
        if(check < 4) {
            check_valid = true;
        }
        if(!card.is_umbrella() && card.needs_second_rotation()) {
            while(check < 3) {
                int current_size = field_count++;
                fields[current_size] = fields[i];
                fields[current_size].undo_add();
                fields[current_size].update_created();
                fields[current_size].mark_not_original();
                check = fields[current_size].add_card(
                    card, check + 1
                );
            }
        }
    }
    if(check_valid) {
        vector<int> to_erase;
        vector<int> not_to_erase;
        for(auto pair : fields) {
            if(pair.second.get_current().is_empty()) {
                to_erase.push_back(pair.first);
            }
            else {
                not_to_erase.push_back(pair.first);
            }
        }
        for(int i : to_erase) {
            if(fields[i].is_original()) {
                fields[not_to_erase[0]].make_original();
            }
            fields.erase(i);
        }
    }
    return check_valid;
}

void Game::remove_cards(const int &n) {
    // remove a set number of cards from the fields
    if(n > 0) {
        for(auto pair : fields) {
            for(int i=0; i<n; i++) {
                pair.second.undo_add();
            }
        }
    }
    if(n > 1) {
        vector<int> to_erase;
        for(auto pair : fields) {
            if(!pair.second.is_valid()) {
                to_erase.push_back(pair.first);
            }
        }
        for(int i : to_erase) {
            fields.erase(i);
        }
    }
    if(fields.empty()) {
        cout << "Reached 0 fields :(" << endl;
    }
}

vector<Field> Game::get_solutions() {
    // get all fields
    vector<Field> solutions;
    for(auto pair : fields) {
        solutions.push_back(pair.second);
    }
    return solutions;
}

int Game::get_size() {
    return fields.size();
}

int Game::get_number_of_fields() {
    return field_count;
}
