#ifndef GAME_H_
#define GAME_H_

#include <map>
#include "Field.h"

using namespace std;

class Game {
    map<int, Field> fields;
    int field_count = 0;

    // reset the map keys for current fields
    void reset_count();

    public:
        Game(Field f);

        // add a card to all current fields
        bool add_card(Card card);

        // remove a set number of cards from the fields
        void remove_cards(const int &n);

        // get all fields
        vector<Field> get_solutions();

        int get_size();

        int get_number_of_fields();
};

#endif
