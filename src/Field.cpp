#include "Field.h"

using namespace std;

Field::Field(vector<vector<Card>> f): field(f) {}

Field::Field() {}

bool Field::check_left(Card &card) {
    // return bool incidating whether the left side of the card fits the current field
    return (
        field[field_order[pos][0]][field_order[pos][1]-1].is_free() ||
        (card.get_side(0) + field[field_order[pos][0]][field_order[pos][1]-1].get_side(2) == 0)
    );
}

bool Field::check_up(Card &card) {
    // return bool incidating whether the upper side of the card fits the current field
    return (
        field[field_order[pos][0]-1][field_order[pos][1]].is_free() ||
        (card.get_side(1) + field[field_order[pos][0]-1][field_order[pos][1]].get_side(3) == 0)
    );
}

bool Field::check_right(Card &card) {
    // return bool incidating whether the right side of the card fits the current field
    return (
        field[field_order[pos][0]][field_order[pos][1]+1].is_free() ||
        (card.get_side(2) + field[field_order[pos][0]][field_order[pos][1]+1].get_side(0) == 0)
    );
}

bool Field::check_down(Card &card) {
    // return bool incidating whether the downwards facing side of the card fits the current field
    return (
        field[field_order[pos][0]+1][field_order[pos][1]].is_free() ||
        (card.get_side(3) + field[field_order[pos][0]+1][field_order[pos][1]].get_side(1) == 0)
    );
}

int Field::add_card(Card card, const int &start_rotation) {
    // add a card to the next position of the field
    card.rotate(start_rotation);
    pos++;
    if(pos >= field_order.size()) {
        cerr << "ERROR: Too many cards added to field" << endl;
        throw 1;
    }
    if(card.is_umbrella()) {
        field[field_order[pos][0]][field_order[pos][1]] = card;
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
            field[field_order[pos][0]][field_order[pos][1]] = card;
            return card.get_rotation();
        }
        card.rotate();
    } while(card.get_rotation() != 0);
    return 4;
}

void Field::undo_add() {
    // remove the last added card (turn it empty and move the field back)
    if(pos == 0) {
        cerr << "ERROR: There are no cards on the field" << endl;
        throw 1;
    }
    field[field_order[pos][0]][field_order[pos][1]].set_empty();
    pos--;
}

string Field::print_card(const int &y, const int &x) {
    return field[y][x].print();
}

Card Field::get_current() {
    return field[field_order[pos][0]][field_order[pos][1]];
}

void Field::update_created() {
    // track when this card was created
    created[0] = field_order[pos][0];
    created[1] = field_order[pos][1];
}

vector<int> Field::get_created() {
    return created;
}

bool Field::is_valid() {
    return (((field_order[pos][0] > created[0]) || ((field_order[pos][0] == created[0]) && (field_order[pos][1] > created[1]))) || original);
}

void Field::mark_not_original() {
    original = false;
}

void Field::make_original() {
    original = true;
}

bool Field::is_original() {
    return original;
}
