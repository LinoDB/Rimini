#include "card_test.h"
#include <iostream>

#define PASSED "\033[32mPASSED\033[0m\n\n"
#define FAILED "\033[31mFAILED\033[0m\n\n"
#define DIV "-----------------------------------------"
#define GREYSTART "\033[38;5;246m"

using namespace std;

int main() {
    {
        using namespace card;
        cout << GREYSTART"Testing \033[33mCard\033[0m" << endl;
        cout << DIV << endl;
        string passed = run_tests() ? FAILED : PASSED;
        cout << DIV << endl;
        cout << GREYSTART"Card tests " << passed << endl;
    }
}
