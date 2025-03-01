#include "Field.h"

using namespace std;

Field::Field(vector<vector<Card>> f): field(f), len(f[0].size()) {}

Field::Field() {}

bool Field::check_left(Card &card) {
    // return bool incidating whether the left side of the card fits the current field
    return (
        field[pos_y][pos_x-1].is_free() ||
        (card.get_side(0) + field[pos_y][pos_x-1].get_side(2) == 0)
    );
}

bool Field::check_up(Card &card) {
    // return bool incidating whether the upper side of the card fits the current field
    return (
        field[pos_y-1][pos_x].is_free() ||
        (card.get_side(1) + field[pos_y-1][pos_x].get_side(3) == 0)
    );
}

bool Field::check_right(Card &card) {
    // return bool incidating whether the right side of the card fits the current field
    return (
        field[pos_y][pos_x+1].is_free() ||
        (card.get_side(2) + field[pos_y][pos_x+1].get_side(0) == 0)
    );
}

bool Field::check_down(Card &card) {
    // return bool incidating whether the downwards facing side of the card fits the current field
    return (
        field[pos_y+1][pos_x].is_free() ||
        (card.get_side(3) + field[pos_y+1][pos_x].get_side(1) == 0)
    );
}

int Field::add_card(Card card, const int &start_rotation) {
    // add a card to the next position of the field
    card.rotate(start_rotation);
    do {
        if(++pos_x == len - 1) {
            pos_x = 0;
            if(++pos_y == len - 1) {
                cerr << "ERROR: Too many cards added to field" << endl;
                throw 1;
            }
        }
    } while(field[pos_y][pos_x].is_base());
    if(card.is_umbrella()) {
        field[pos_y][pos_x] = card;
        return card.get_rotation();
    }
    do {
        if(
            (
                check_left(card) &&
                check_up(card) &&
                check_right(card) &&
                check_down(card) 
            )
        ) {
            field[pos_y][pos_x] = card;
            return card.get_rotation();
        }
        card.rotate();
    } while(card.get_rotation() != 0);
    return 4;
}

void Field::undo_add() {
    // remove the last added card (turn it empty and move the field back)
    if(field[pos_y][pos_x].is_base()) {
        cerr << "ERROR: Tried to remove a base card from the field" << endl;
        throw 1;
    }
    field[pos_y][pos_x].set_empty();
    do {
        if((--pos_x == -1)) {
            pos_x = len - 2;
            if(--pos_y == 0) {
                cerr << "ERROR: Tried to remove card from an empty field" << endl;
                throw 1;
            }
        }
    } while(field[pos_y][pos_x].is_base() && !(pos_x == 0 && pos_y == 1));
}

string Field::print_card(const int &y, const int &x) {
    return field[y][x].print();
}

Card Field::get_current() {
    return field[pos_y][pos_x];
}

void Field::update_created() {
    // track when this card was created
    created[0] = pos_y;
    created[1] = pos_x;
}

vector<int> Field::get_created() {
    return created;
}

bool Field::is_valid() {
    return (((pos_y > created[0]) || ((pos_y == created[0]) && (pos_x > created[1]))) || original);
}

void Field::mark_not_original() {
    original = false;
}

bool Field::is_original() {
    return original;
}
