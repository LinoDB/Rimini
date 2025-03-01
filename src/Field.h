#ifndef FIELD_H_
#define FIELD_H_

#include <vector>
#include <iostream>
#include "Card.h"

using namespace std;

class Field {
    vector<vector<Card>> field;
    int pos_x = 0;
    int pos_y = 1;
    int len = 0;
    bool original = true;
    vector<int> created = {1, 0};

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

        bool is_valid();

        void mark_not_original();

        bool is_original();
};

#endif
