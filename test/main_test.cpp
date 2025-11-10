#include "card_test.h"
#include <iostream>

#define PASSED "\033[32mPASSED\033[0m\n"
#define FAILED "\033[31mFAILED\033[0m\n"

using namespace std;

int main() {
    {
        using namespace card;
        string passed = run_tests() ? FAILED : PASSED;
        cout << "Card tests " << passed << endl;
    }
}
