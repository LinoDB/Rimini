#ifndef CARD_H_
#define CARD_H_

#include <string>
#include <vector>

using namespace std;

class Card {
    int sides[4] = {0, 0, 0, 0};
    bool umbrella = false;
    bool empty = false;
    bool base = false;
    bool second_rotation = false;
    int rotation = 0;
    int card_id = 0;
    public:
        // initialize card with side values
        Card(int id, int left, int up, int right, int down, bool b=false, bool s_r=false);

        // initialize card with side values
        Card(int id, vector<int> inputs, bool b=false, bool s_r=false);

        // initialize card as ubrella (joker)
        Card(int id, bool u, bool b=false);

        // initialize empty card
        Card();

        // get side (0=left, 1=up, 2=right, 3=down)
        int get_side(const int &s);

        // get current rotation (0-3)
        int get_rotation();

        // rotate the card left
        void rotate();

        // rotate the card left
        void rotate(const int &r);

        // output the sides as a string (left, up, right, down)
        string print();

        bool is_umbrella();

        bool is_empty();

        bool is_free();

        bool is_base();

        void set_empty();

        bool needs_second_rotation();
};

#endif
