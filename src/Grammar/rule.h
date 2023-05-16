#pragma once

#include <iostream>

using namespace std;

struct rule {
    string Left;
    string Right;

    string operator[](const int &index) {
        if (index == 0) return Left;
        else return Right;
    }

    rule(const string &left, const string &right) {
        Left = left;
        Right = right;
    }
};

ostream &operator<<(ostream &os, rule &rule) {
    os << rule[0] << " -> " << rule[1];
    return os;
}