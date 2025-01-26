#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

class SequenceGenerator {
        int len, start_pos;
        vector<int> seq;
        map<int, int> reg;
        int pos;
        bool d = false;

        void move_position(int &s) {
            // move one sequence position left and reset the current step
            seq[pos] = s;
            if(--pos < 0) {
                d = true;
                return;
            }
            s = seq[pos];
        }
        void re_order(const int &s) {
            // finish the swap of current numbers and sort all to the right (slow!?)
            seq[reg[seq[pos]]] = s;
            sort(seq.begin()+pos+1, seq.end()); // bad because happens all the time
            for(int i=pos; i<len; i++) {
                reg[seq[i]] = i;
            }
            pos = start_pos;
        }

    public:
        SequenceGenerator(const int &l) {
            // initialize with a monotonously increasing sequence and position map
            len = l;
            start_pos = len - 2;
            for(int i=0; i<len; i++) {
                seq.push_back(i);
                reg[i] = i;
            }
            pos = start_pos;
        }
        int get(const int &i) {
            // get sequence value at index
            return seq[i];
        }
        int get_length() {
            // get length of the sequence
            return len;
        }
        bool done() {
            // is 'true' when all sequences have been tested
            return d;
        }
        int next() {
            // increase the current position until it's either above the sequence
            // length or the value can be found on the right side of the current position
            if(d) return 0;
            int moved = 0;
            int s = seq[pos];
            while(true) {
                seq[pos]++;
                if(seq[pos] > len - 1) {
                    move_position(s);
                    moved++;
                    if(d) break;
                }
                else if(reg[seq[pos]] > pos) {
                    re_order(s);
                    break;
                }
            }
            return moved;
        }
        void skip(const int &i) {
            // skip the remaining sequences of the given position
            pos = i;
        }
        string print() {
            // return a string of comma the seperated values of the current sequence
            stringstream s;
            for(int i=0; i<len-1; i++) {
                s << seq[i] << ',';
            }
            s << seq[len-1];
            return s.str();
        }
        vector<int> get_sequence() {
            return seq;
        }
};

class Card {
    int sides[4] = {0, 0, 0, 0};
    bool umbrella = false;
    bool empty = false;
    bool base = false;
    int rotation = 0;
    public:
        Card(int left, int up, int right, int down, bool b=false) {
            // initialize card with side values
            sides[0] = left;
            sides[1] = up;
            sides[2] = right;
            sides[3] = down;
            base = b;
        }
        Card(vector<int> inputs, bool b=false) {
            // initialize card with side values
            sides[0] = inputs[0];
            sides[1] = inputs[1];
            sides[2] = inputs[2];
            sides[3] = inputs[3];
            base = b;
        }
        Card(bool u, bool b=false): umbrella(u), base(b) {
            // initialize card as ubrella (joker)
        }
        Card() {
            empty = true;
        }
        int get_side(const int &s) {
            // get side (0=left, 1=up, 2=right, 3=down)
            return sides[(s + rotation) % 4];
        }
        int get_rotation() {
            // get current rotation (0-3)
            return rotation;
        }
        void rotate() {
            // rotate the card left
            rotation = (rotation + 1) % 4;
        }
        void rotate(const int &r) {
            // rotate the card left
            rotation = (rotation + r) % 4;
        }
        string print() {
            // output the sides as a string (left, up, right, down)
            stringstream s;
            for(int i=0; i<4; i++) {
                s << sides[(i + rotation) % 4] << ',';
            }
            s << " umbrella: " << umbrella << ", empty: " << empty;
            s << ", free: " << (empty | umbrella);
            return s.str();
        }
        bool is_umbrella() {
            return umbrella;
        }
        bool is_empty() {
            return empty;
        }
        bool is_free() {
            return empty | umbrella;
        }
        bool is_base() {
            return base;
        }
        void set_empty() {
            empty = true;
        }
};

class Field {
    vector<vector<Card>> field;
    // bool valid = false;
    int pos_x = 0;
    int pos_y = 1;
    int len = 0;
    bool original = true;
    vector<int> created = {1, 0};
    public:
        Field(vector<vector<Card>> f): field(f), len(f[0].size()) {} //, valid(true)
        Field() {}
        // void make_unvalid() {
        //     valid = false;
        // }
        bool check_left(Card &card) {
            return (
                field[pos_y][pos_x-1].is_free() ||
                (card.get_side(0) + field[pos_y][pos_x-1].get_side(2) == 0)
            );
        }
        bool check_up(Card &card) {
            return (
                field[pos_y-1][pos_x].is_free() ||
                (card.get_side(1) + field[pos_y-1][pos_x].get_side(3) == 0)
            );
        }
        bool check_right(Card &card) {
            return (
                field[pos_y][pos_x+1].is_free() ||
                (card.get_side(2) + field[pos_y][pos_x+1].get_side(0) == 0)
            );
        }
        bool check_down(Card &card) {
            return (
                field[pos_y+1][pos_x].is_free() ||
                (card.get_side(3) + field[pos_y+1][pos_x].get_side(1) == 0)
            );
        }
        int add_card(Card card, const int &start_rotation=0) {
            // if(!valid) return 4;
            card.rotate(start_rotation);
            do {
                if(++pos_x == len + 1) {
                    pos_x = 1;
                    if(++pos_y == len + 1) {
                        cout << "ERROR: Too many cards added to field" << endl;
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
            // valid = false;
            return 4;
        }
        void undo_add() {
            // DON'T ALWAYS MAKE TRUE
            // valid = true;
            field[pos_y][pos_x].set_empty();
            do {
                if(((--pos_x == 0) && !(pos_y == 1)) || (pos_x == -1)) {
                    pos_x = len;
                    if(--pos_y == 0) {
                        cout << "ERROR: Tried to remove card from an empty field" << endl;
                        throw 1;
                    }
                }
            } while(field[pos_y][pos_x].is_base() && !(pos_x == 0));
        }
        string print_card(const int &y, const int &x) {
            return field[y][x].print();
        }
        Card get_current() {
            return field[pos_y][pos_x];
        }
        void update_created() {
            created[0] = pos_y;
            created[1] = pos_x;
        }
        bool is_valid() {
            return (((pos_y > created[0]) || ((pos_y == created[0]) && (pos_x > created[1]))) || original);
        }
        void mark_not_original() {
            original = false;
        }
        bool is_original() {
            return original;
        }
};

class Game {
    // actually make an extra field class and this a vector of field (to loop over)
    // the field class has the 'valid' attribute so there'll be no need to rearrange vectors
    map<int, Field> fields;
    // bool valid = true;
    int field_count = 0;
    public:
        Game(Field f) {
            fields[0] = f;
            field_count++;
        }
        bool add_card(Card card) {
            bool check_valid = false;
            // vector<int> moves_made;
            vector<int> keys;
            for(auto pair : fields) {
                keys.push_back(pair.first);
            }
            for(int i : keys) {
                // if(fields[i].is_valid()) {
                int check = fields[i].add_card(card);
                if(check < 4) {
                    check_valid = true;
                }
                // else {
                //     moves_made.push_back(i);
                // }
                if(!card.is_umbrella()) {
                    while(check < 3) {
                        // make it exclude symmetrical cards
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
                // }
            }
            if(check_valid) {
                vector<int> to_erase;
                for(auto pair : fields) {
                    if(pair.second.get_current().is_empty()) {
                        to_erase.push_back(pair.first);
                    }
                }
                for(int i : to_erase) {
                    if(!fields[i].is_original()) {
                        fields.erase(i);
                    }
                }
            }
            // // NOT NECESSARY, REMOVE moves_made
            // if(!check_valid) {
            //     for(int move : moves_made) {
            //         fields[move].undo_add();
            //     }
            // }
            return check_valid;
        }
        void remove_cards(const int &n) {
            if(n > 0) {
                for(auto pair : fields) {
                    // if(pair.second.is_valid()) {
                    for(int i=0; i<n; i++) {
                        fields[pair.first].undo_add();
                    }
                    // }
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
        }
        vector<Field> get_solutions() {
            vector<Field> solutions;
            for(auto pair : fields) {
                solutions.push_back(fields[pair.first]);
            }
            return solutions;
        }
        int get_size() {
            return fields.size();
        }
        int get_number_of_fields() {
            return field_count;
        }
};

void add_element(vector<int> &arr, const string &s) {
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

Field import_field(const string &file_name, const int &size) {
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
        // make test with linebreak
        cout << "ERROR: Input file for field must be at least 6 columns long "
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
            field[stoi(split[0])][stoi(split[1])] = Card(true, true);
        }
        else {
            field[stoi(split[0])][stoi(split[1])] = Card(
                line_arr[1],
                line_arr[2],
                line_arr[3],
                line_arr[4],
                true
            );
        }
    }

    field_csv.close(); 
    return Field(field);
};

vector<Card> import_cards(const string &file_name) {
    vector<Card> cards;
    string line;
    string header;
    ifstream card_csv(file_name);

    getline(card_csv, header);
    int len = split_string(header, ",").size();
    if(len < 5) {
        // make test with linebreak
        cout << "ERROR: Input file for cards must be at least 5 columns long "
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
            cards.push_back(Card(true));
        }
        else {
            cards.push_back(
                Card(
                    line_arr[1],
                    line_arr[2],
                    line_arr[3],
                    line_arr[4]
                )
            );
        }
    }

    card_csv.close(); 
    return cards;
};

class Solutions {
    vector<vector<Field>> solutions;
    vector<vector<int>> sequences;
    public:
        void add_solution(vector<Field> solution, vector<int> sequence) {
            solutions.push_back(solution);
            sequences.push_back(sequence);
        }
        string print() {
            stringstream s;
            if(solutions.size() == 0) {
                return "No solutions found!\n";
            }
            for(int i=0; i<solutions.size(); i++) {
                for(int k=0; k<sequences[i].size()-1; i++) {
                    s << sequences[i][k] << ", ";
                }
                s << sequences[i][sequences[i].size()-1];
                s << " - " << solutions[i].size() << " Solutions\n";
            }
            return s.str();
        }
};

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
    // int count = 0;
    Game game(field);
    int seq = 0;
    cout << "Size debug: " << cards.size() << endl;
    while(!s.done()) {
        // if((count % 100000) == 0) {
        //     cout << "Rounds: " << count << endl;
        //     cout << "Seq: " << seq << endl;
        //     cout << "Current number of fields: " << game.get_size() << endl;
        //     cout << "Total number of fields: " << game.get_number_of_fields() << endl;
        //     cout << "Current sequence: " << s.print() << endl;
        // }
        // count++;
        if(!game.add_card(cards[s.get(seq)])) {
            s.skip(seq);
            // cout << "Current level: " << seq << endl;
            // if(seq == 0) {
            //     cout << ++count_0 << "/71 - 0/71" << endl;
            //     count_1 == 0;
            // }
            // else if(seq == 1) {
            //     cout << count_0 << "/71 - " << ++count_1 << "/71" << endl;
            // }
            int moved = s.next();
            seq -= moved;
            game.remove_cards(moved + 1);
        }
        else if(++seq == s.get_length() - 1) {
            if(game.add_card(cards[s.get(seq)])) {
                solutions.add_solution(game.get_solutions(), s.get_sequence());
                cout << "Solution found: " << s.print() << endl;
                // s.next();
                // game = Game(field);
            }
            int moved = s.next();
            seq -= moved + 1;
            game.remove_cards(moved + 1);
        }
    }


    //  while(true) {    
    //     while(!game.add_card(cards[s.get(seq)])) {
    //         s.skip(seq);
    //         // cout << "Current level: " << seq << endl;
    //         // if(seq == 0) {
    //         //     cout << ++count_0 << "/72 - 0/72" << endl;
    //         //     count_1 == 0;
    //         // }
    //         // else if(seq == 1) {
    //         //     cout << count_0 << "/72 - " << ++count_1 << "/72" << endl;
    //         // }
    //         int moved = s.next();
    //         seq -= moved;
    //         game.remove_cards(moved + 1);
    //         if(s.done()) break;
    //     }
    //     if(s.done()) break;
    //     if(++seq == s.get_length()) {
    //         solutions.add_solution(game.get_solutions(), s.get_sequence());
    //         cout << "Solution found: " << s.print() << endl;
    //         // int moved = s.next();
    //         // seq -= moved;
    //         // game.remove_cards(moved);
    //         if(s.done()) break;
    //     }
    // }


        // vector<int> sequence = s.get_sequence();
        // for(int seq=0; seq<sequence.size(); seq++) {
        //     if(!game.add_card(cards[sequence[seq]])) {
        //         s.skip(seq);
        //         if(seq == 0) {
        //             cout << ++count_0 << "/72 - 0/72" << endl;
        //             count_1 == 0;
        //         }
        //         else if(seq == 1) {
        //             cout << count_0 << "/72 - " << ++count_1 << "/72" << endl;
        //         }
        //         break;
        //     }
        //     if(seq == (sequence.size() - 1)) {
        //         solutions.add_solution(game.get_solutions(), sequence);
        //         cout << "Solution found: " << s.print() << endl;
        //     }
        // }
        // s.next();
    cout << "DONE" << endl;
    cout << s.print() << endl;
    cout << solutions.print();
}

/*
    - Only sort if going right (if possible?) -> minimize sorting
    - Minimize rotations by defining card types
     (no need to rotate after finding a solution for 4 equals or symmetric
     cards -> there is always at least a card left and up anyways)
    - Re-assign indices of the map from time to time
    - Optimize where possible
    - Make it run parallel
*/
