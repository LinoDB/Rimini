#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include "SequenceGenerator.h"
#include "Card.h"
#include "Field.h"
#include "Game.h"
#include "Solutions.h"

#define TOTAL_THREADS 12
#define ROUNDS 5
#define ROUND_CHECK 10000000

using namespace std;

void print_entire_solution(Field &solution);

void add_element(vector<int> &arr, const string &s);

vector<string> split_string(const string &s, const string &delim);

bool test_three_equals(const vector<int> &line);

vector<Card> import_cards(const string &file_name);

void find_solution(vector<Card> cards, Field field, const int &num, const float &total, bool *finished);

Field import_field(const string &file_name, const int &size);

int main() {
    vector<Card> cards = import_cards("resources/cards.csv");
    Field field = import_field("resources/field.csv", 11);
    cout << "Card count: " << cards.size() << endl << endl;
    cout << "Thread count: " << TOTAL_THREADS << endl << endl;
    if(cards.size() < TOTAL_THREADS) {
        cerr << "ERROR: Use less threads than card count" << endl;
        throw 1;
    }
    bool finished = false;
    vector<thread*> threads;
    for(int i = 0; i < TOTAL_THREADS; i++) {
        threads.push_back(new thread(find_solution, cards, field, i, TOTAL_THREADS, &finished));
        this_thread::sleep_for(chrono::milliseconds(990 / TOTAL_THREADS));
    }
    for(thread* t : threads) {
        t->join();
    }
}

void find_solution(vector<Card> cards, Field field, const int &num, const float &total, bool *finished) {
    Solutions solutions;
    SequenceGenerator s(cards.size());
    const int starting = cards.size() * (num / total);
    int ending = cards.size() * ((num + 1) / total);
    ending = ending == cards.size() ? ending + 1: ending;
    cout << "Running thread " << num << " from cards " << starting << " to " << ending << endl;
    for(int i = 0; i < starting; i++) {
        s.skip(0);
        s.next();
    }
    int count = 0;
    Game game(field);
    int seq = 0;
    int first_val = s.get(0);
    int first_count = 0;
    while(!s.done() && s.get(0) < ending && !*finished) {
        if((count % ROUND_CHECK) == 0) {
            string skipped = "";
            if(first_val != s.get(0)) {
                first_count = 0;
                first_val = s.get(0);
            }
            if(first_count++ == ROUNDS) {
                first_count = 0;
                skipped = ", Skipping start number: " + to_string(first_val);
                s.skip(0);
                s.next();
                first_val = s.get(0);
            }
            cout << "Thread: " << num << ", Rounds: " << count <<
                ", Fields: " << game.get_size() << endl;
            cout << s.print(seq) << skipped << endl << endl;
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
            cout << "Solution found: " << s.print(-1) << endl;
            *finished = true;
            int moved = s.next();
            seq -= moved + 1;
            game.remove_cards(moved + 1);
        }
    }

    // cout << "DONE" << endl;
    // cout << s.print(-1) << endl;
    // cout << solutions.print();
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
            cout << "(" << y << "," << x << ") " << 
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
    // -> this is only true if at least 2 sides have to match
    return true;
    // int count = 0;
    // for(int i=2; i<5; i++) {
    //     count += line[1] != line[i];
    // }
    // if(count == 1) return true;
    // else if(count == 3) {
    //     count = 0;
    //     for(int i=3; i<5; i++) {
    //         count += line[2] != line[i];
    //     }
    //     if(count == 0) return true;
    // }
    // return false;
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
