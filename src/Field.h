#ifndef FIELD_H_
#define FIELD_H_

#include <vector>
#include <iostream>
#include "Card.h"
#include <set>

using namespace std;

class Field {
    vector<vector<Card>> field;
    int pos = -1;
    bool original = true;
    set<int> card_ids;
    vector<int> created = {1, 0};
    vector<vector<int>> field_order = {
        {2, 1},
        {1, 2},
        {1, 1},
        {2, 3},
        {1, 4},
        {1, 3},
        {1, 6},
        {1, 5},
        {1, 8},
        {2, 7},
    
        {1, 7},
        {2, 9},
        {1, 9},
        {3, 2},
        {2, 2},
        {3, 4},
        {2, 4},
        {3, 6},
        {2, 6},
        {3, 8},
    
        {2, 8},
        {4, 1},
        {3, 1},
        {4, 3},
        {4, 2},
        {4, 5},
        {3, 5},
        {4, 7},
        {4, 9},
        {4, 8},
    
        {3, 9},
        {6, 1},
        {5, 1},
        {5, 4},
        {4, 4},
        {6, 3},
        {5, 3},
        {5, 6},
        {4, 6},
        {6, 7},
    
        {5, 7},
        {6, 9},
        {5, 9},
        {7, 2},
        {6, 2},
        {6, 5},
        {7, 4},
        {6, 4},
        {7, 6},
        {6, 6},
    
        {7, 5},
        {7, 8},
        {6, 8},
        {8, 1},
        {7, 1},
        {9, 2},
        {9, 1},
        {8, 3},
        {8, 2},
        {9, 4},
    
        {8, 4},
        {9, 3},
        {9, 6},
        {9, 5},
        {8, 7},
        {8, 6},
        {9, 8},
        {9, 7},
        {8, 9},
        {7, 9},
    
        {8, 8},
        {9, 9},
    };

    public:
        Field(vector<vector<Card>> f);
        Field();

        // return bool incidating whether the left side of the card fits the current field
        bool check_left(Card &card);

        // return bool incidating whether the upper side of the card fits the current field
        bool check_up(Card &card);

        // return bool incidating whether the right side of the card fits the current field
        bool check_right(Card &card);

        // return bool incidating whether the downwards facing side of the card fits the current field
        bool check_down(Card &card);

        // add a card to the next position of the field
        int add_card(Card card, const int &start_rotation=0);

        // remove the last added card (turn it empty and move the field back)
        void undo_add();

        string print_card(const int &y, const int &x);

        Card get_current();

        // track when this card was created
        void update_created();

        vector<int> get_created();

        void check_cards_valid(const bool &removed=false);

        bool is_valid();

        void mark_not_original();

        void make_original();

        bool is_original();
};

#endif
