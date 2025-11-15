#include "card_test.h"
#include "sequence_generator_test.h"
#include <iostream>
#include <vector>
#include <functional>

#define PASSED "\033[32mPASSED\033[0m\n"
#define FAILED "\033[31mFAILED\033[0m\n"
#define DIV "-----------------------------------------"
#define TESTDIV "*****************************************\n"
#define GREYSTART "\033[38;5;246m"

using namespace std;

typedef function<bool (void)> func;

bool test(func f, const string& name) {
    cout << GREYSTART"Testing \033[33m" << name << "\033[0m" << endl;
    cout << DIV << endl;
    bool result = f();
    cout << DIV << endl;
    cout << GREYSTART << name << " tests " << (result ? PASSED: FAILED) << endl;
    cout << TESTDIV << endl;
    return result;
}

int main() {
    vector<int> overall = {0, 2};
    overall[0] += test(card::run_tests, "Card");
    overall[0] += test(sequence_generator::run_tests, "SequenceGenerator");

    cout << "\033[3" << (overall[0] == overall[1] ? "2" : "1")
        << "mOverall result [" << overall[0] << "/" << overall[1]
        << "]\033[0m" << endl;
}
