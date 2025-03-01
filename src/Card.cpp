#include "Card.h"
#include <map>
#include <sstream>

using namespace std;

map<int, string> legend = {
    {0, " N"},
    {1, "FL"},
    {-1, "FU"},
    {2, "ML"},
    {-2, "MU"},
};

Card::Card(int id, int left, int up, int right, int down, bool b, bool s_r) {
    // initialize card with side values
    card_id = id;
    sides[0] = left;
    sides[1] = up;
    sides[2] = right;
    sides[3] = down;
    base = b;
    second_rotation = s_r;
}

Card::Card(int id, vector<int> inputs, bool b, bool s_r) {
    // initialize card with side values
    card_id = id;
    sides[0] = inputs[0];
    sides[1] = inputs[1];
    sides[2] = inputs[2];
    sides[3] = inputs[3];
    base = b;
    second_rotation = s_r;
}

Card::Card(int id, bool u, bool b): card_id(id), umbrella(u), base(b) {
    // initialize card as ubrella (joker)
}

Card::Card() {
    // initialize empty card
    empty = true;
}

int Card::get_side(const int &s) {
    // get side (0=left, 1=up, 2=right, 3=down)
    return sides[(s + rotation) % 4];
}

int Card::get_rotation() {
    // get current rotation (0-3)
    return rotation;
}

void Card::rotate() {
    // rotate the card left
    rotation = (rotation + 1) % 4;
}

void Card::rotate(const int &r) {
    // rotate the card left
    rotation = (rotation + r) % 4;
}

string Card::print() {
    // output the sides as a string (left, up, right, down)
    stringstream s;
    s << "Card " << ((0 <= card_id && card_id < 10) ? " " : "") << card_id << ": ";
    for(int i=0; i<4; i++) {
        s << legend.at(sides[(i + rotation) % 4]) << ", ";
    }
    s << "umbrella: " << umbrella << ", empty: " << empty;
    s << ", base: " << base << ", s_r: " << second_rotation;
    return s.str();
}

bool Card::is_umbrella() {
    return umbrella;
}

bool Card::is_empty() {
    return empty;
}

bool Card::is_free() {
    return empty | umbrella;
}

bool Card::is_base() {
    return base;
}

void Card::set_empty() {
    empty = true;
}

bool Card::needs_second_rotation() {
    return second_rotation;
}
