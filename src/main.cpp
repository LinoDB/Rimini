#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include "SequenceGenerator.h"
#include "Card.h"
#include "Field.h"
#include "Game.h"
#include "Solutions.h"

using namespace std;

void print_entire_solution(Field &solution);

void add_element(vector<int> &arr, const string &s);

vector<string> split_string(const string &s, const string &delim);

bool test_three_equals(const vector<int> &line);

vector<Card> import_cards(const string &file_name);

Field import_field(const string &file_name, const int &size);


int main() {
    // The sequence interaction is wrong (moves further left than seq)
    // Update add_cart (see Field)

    vector<Card> cards = import_cards("resources/cards.csv");
    Field field = import_field("resources/field.csv", 11);
    Solutions solutions;
    SequenceGenerator s(cards.size());
    // int count_0 = 0;
    // int count_1 = 0;
    // cout << "0/71 - 0/71" << endl;
    int count = 0;
    Game game(field);
    int seq = 0;
    // s.next();
    // s.skip(0);
    // s.next();
    // s.skip(0);
    // s.next();
    // s.skip(0);
    // s.next();
    // s.skip(0);
    // s.next();
    // s.skip(0);
    // s.next();
    // s.skip(0);
    // s.next();
    // s.skip(0);
    // s.next();
    // s.skip(0);
    // s.next();
    cout << "Size debug: " << cards.size() << endl << endl;
    while(!s.done()) {
        if((count % 10000000) == 0) {
            cout << "Rounds: " << count << endl;
            // cout << "Seq: " << seq << endl;
            // cout << "Current number of fields: " << game.get_size() << endl;
            // cout << "Total number of fields: " << game.get_number_of_fields() << endl;
            cout << s.print() << endl << endl;
        }
        count++;
        if(!game.add_card(cards[s.get(seq)])) {
            s.skip(seq);
            int moved = s.next();
            seq -= moved;
            game.remove_cards(moved + 1);
        }
        else if(++seq == s.get_length()) {
            solutions.add_solution(game.get_solutions(), s.get_sequence());
            print_entire_solution(game.get_solutions()[0]);
            cout << "Solution found: " << s.print() << endl;
            int moved = s.next();
            seq -= moved + 1;
            game.remove_cards(moved + 1);
        }
    }

    cout << "DONE" << endl;
    cout << s.print() << endl;
    cout << solutions.print();
}

/*
    - UNIT TESTING TO BE SURE THERE ARE NO BUGS
      (there are empty cards that aren't empty in some places e.g.)
    - Run down a fixed order that causes more conflicts (left to right is minum
      conflicts -> cause 4-way fits)
    - Refine sequence generator (?)
    - Make it run parallel
*/

void print_entire_solution(Field &solution) {
    vector<int> created = solution.get_created();
    cout << "Original field: " << solution.is_original() << ", Created: ("
        << created[0] << ", " << created[1] << ')' << endl;
    for(int y=1; y<10; y++) {
        for(int x=1; x<10; x++) {
            cout << "Row " << y << ", Col " << x << ": " << 
                solution.print_card(y, x) << endl;
        }
    }
}

void add_element(vector<int> &arr, const string &s) {
    // convert card side types to ints
    if(s[0] == 'N') {
        arr[0] = 1;
    }
    else if(s[0] == 'F') {
        if(s[1] == 'L') {
            arr.push_back(1);
            return;
        }
        else if(s[1] == 'U') {
            arr.push_back(-1);
            return;
        }
    }
    else if(s[0] == 'M') {
        if(s[1] == 'L') {
            arr.push_back(2);
            return;
        }
        else if(s[1] == 'U') {
            arr.push_back(-2);
            return;
        }
    }
    arr.push_back(0);
}

vector<string> split_string(const string &s, const string &delim) {
    // split a string by a given delimiter
    vector<string> split;
    int s_pos = 0;
    int e_pos;
    while((e_pos = s.find(delim, s_pos)) != string::npos) {
        split.push_back(s.substr(s_pos, e_pos - s_pos));
        s_pos = e_pos + delim.size();
    }
    if(s_pos < s.size()) {
        split.push_back(s.substr(s_pos, s.size()));
    }
    return split;
}

bool test_three_equals(const vector<int> &line) {
    // check if a card has 3 equal sides (in this case multiple fitting rotations matter)
    int count = 0;
    for(int i=2; i<5; i++) {
        count += line[1] != line[i];
    }
    if(count == 1) {
        return true;
    }
    return false;
}

vector<Card> import_cards(const string &file_name) {
    // import cards from a csv file
    vector<Card> cards;
    string line;
    string header;
    ifstream card_csv(file_name);

    getline(card_csv, header);
    int len = split_string(header, ",").size();
    if(len < 5) {
        // make test with linebreak
        cerr << "ERROR: Input file for cards must be at least 5 columns long "
        "(valid column indices: 1 - 4)" << endl;
        throw 1;
    }
    while(getline(card_csv, line)) {
        auto split = split_string(line, ",");
        vector<int> line_arr = {0};
        for(int i=1; i<split.size(); i++) {
            add_element(line_arr, split[i]);
        }
        while(line_arr.size() < 5) {
            line_arr.push_back(0);
        }
        if(line_arr[0]) {
            cards.push_back(Card(stoi(split[0]), true));
        }
        else {
            cards.push_back(
                Card(
                    stoi(split[0]),
                    line_arr[1],
                    line_arr[2],
                    line_arr[3],
                    line_arr[4],
                    false,
                    test_three_equals(line_arr)
                )
            );
        }
    }

    card_csv.close(); 
    return cards;
};

Field import_field(const string &file_name, const int &size) {
    // import the base field from a csv file
    vector<Card> empty_row;
    for(int i=0; i<size; i++) {
        empty_row.push_back(Card());
    }
    vector<vector<Card>> field;
    for(int i=0; i<size; i++) {
        field.push_back(empty_row);
    }

    string line;
    string header;
    ifstream field_csv(file_name);
    getline(field_csv, header);
    int len = split_string(header, ",").size();
    if(len < 6) {
        cerr << "ERROR: Input file for field must be at least 6 columns long "
        "(valid column indices: 0 - 5)" << endl;
        throw 1;
    }
    while(getline(field_csv, line)) {
        auto split = split_string(line, ",");
        vector<int> line_arr = {0};
        for(int i=2; i<split.size(); i++) {
            add_element(line_arr, split[i]);
        }
        while(line_arr.size() < 5) {
            line_arr.push_back(0);
        }
        if(line_arr[0]) {
            field[stoi(split[0])][stoi(split[1])] = Card(-1, true, true);
        }
        else {
            field[stoi(split[0])][stoi(split[1])] = Card(
                -1,
                line_arr[1],
                line_arr[2],
                line_arr[3],
                line_arr[4],
                true,
                false
            );
        }
    }

    field_csv.close(); 
    return Field(field);
};
