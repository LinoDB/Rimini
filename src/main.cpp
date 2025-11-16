#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include "SequenceGenerator.h"
#include "Card.h"
#include "Field.h"
#include "Game.h"
#include "Solutions.h"
#include "utils.h"

#define TOTAL_THREADS 1
#define ROUNDS 5
#define ROUND_CHECK 10000000

using namespace std;

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
