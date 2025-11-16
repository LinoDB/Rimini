#ifndef UTILS_H_
#define UTILS_H_

#include "Field.h"
#include <string>

// declarations

void print_entire_solution(Field &solution);

void add_element(vector<int> &arr, const string &s);

vector<string> split_string(const string &s, const string &delim);

bool test_three_equals(const vector<int> &line);

vector<Card> import_cards(const string &file_name);

void find_solution(vector<Card> cards, Field field, const int &num, const float &total, bool *finished);

Field import_field(const string &file_name, const int &size);

#endif
