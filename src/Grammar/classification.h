#pragma once

#include "grammar.h"

namespace Grammar_functions {
    bool isType3Grammar(const vector<rule> &P) {
        for (const rule &prod: P) {

            if (prod.Right.length() > 1 || !isupper(prod.Left[0]) || !islower(prod.Right[0])) {
                return false;
            }
        }
        return true;
    }


    bool isType2Grammar(const vector<rule> &P) {
        for (const rule &prod: P) {

            if (prod.Right.empty() || !isupper(prod.Left[0])) {
                return false;
            }
        }
        return true;
    }


    bool isType1Grammar(const vector<rule> &P) {
        for (const rule &prod: P) {

            if (prod.Right.length() < 2 || !isupper(prod.Left[0]) || prod.Right[0] == prod.Left[0]) {
                return false;
            }
            bool foundNonterminal = false;
            for (char c: prod.Right) {
                if (isupper(c)) {
                    if (foundNonterminal) {
                        return false;
                    }
                    foundNonterminal = true;
                } else {
                    if (!foundNonterminal) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    string clasify(const vector<rule> P) {
        if (isType3Grammar(P)) {
            return "Type 3";
        } else if (isType2Grammar(P)) {
            return "Type 2";
        } else if (isType1Grammar(P)) {
            return "Type 1";
        } else {
            return "Type 0";
        }
    }
}