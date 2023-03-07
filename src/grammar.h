#pragma once

#include <vector>
#include <random>
#include <cstdlib>
#include <csignal>
#include <set>

using namespace std;

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

struct Rule {
    string Left;
    string Right;

    string operator[] (const int &index) {
        if (index == 0) return Left;
        else return Right;
    }

    Rule (const string& left, const string& right) {
        Left = left;
        Right = right;
    }
};

ostream& operator<<(ostream& os, Rule& rule)
{
    os << rule[0] << " -> " << rule[1];
    return os;
}

struct Grammar {
    vector<string> nonTerminalVariables;
    vector<string> terminalVariables;
    vector<Rule> productions;
    string startingCharacter;

    string generateWord () {
        int count = 1;
        vector<Rule> toBeChanged;
        string word = startingCharacter;

        random_device rnd_device;
        mt19937 mersenne_engine {rnd_device()};

        while (count != 0) {
            count = 0;
            toBeChanged = {};
            int index = 0;
            for (Rule rule : productions) {
                if (word.find(rule.Left) != string::npos) {
                    count++;
                    toBeChanged.emplace_back(rule);
                }
            }

            if (count != 0) {
                index = mersenne_engine() % count;
                replace(word, toBeChanged[index][0], toBeChanged[index][1]);
            }
        }
        return word;
    }

    bool isType3Grammar(const vector<Rule>& P) {
        for (const Rule& prod : P) {
            // A production is of type 3 if it is of the form A -> aB or A -> a
            if (prod.Right.length() > 1 || !isupper(prod.Left[0]) || !islower(prod.Right[0])) {
                return false;
            }
        }
        return true;
    }

// Function to check if a grammar is of type 2
    bool isType2Grammar(const vector<Rule>& P) {
        for (const Rule& prod : P) {
            // A production is of type 2 if it is of the form A -> α
            if (prod.Right.empty() || !isupper(prod.Left[0])) {
                return false;
            }
        }
        return true;
    }

// Function to check if a grammar is of type 1
    bool isType1Grammar(const vector<Rule>& P) {
        for (const Rule& prod : P) {
            // A production is of type 1 if it is of the form αAβ -> αγβ
            if (prod.Right.length() < 2 || !isupper(prod.Left[0]) || prod.Right[0] == prod.Left[0]) {
                return false;
            }
            bool foundNonterminal = false;
            for (char c : prod.Right) {
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

    string clasify() {
        if (isType3Grammar(productions)) {
            return "Type 3";
        } else if (isType2Grammar(productions)) {
            return "Type 2";
        } else if (isType1Grammar(productions)) {
            return "Type 1";
        } else {
            return "Type 0";
        }
    }

    Grammar() {}
    Grammar(const string& variant) {
        string variant_copy = variant;

        auto x1 = variant_copy.find("{");
        auto x2 = variant_copy.find("}");
        auto VT = variant_copy.substr(x1+1, x2-x1-1);
        while(VT.find(",") != string::npos) replace(VT, ", ", "");
        for(int i=0; i<VT.size(); i++) nonTerminalVariables.emplace_back(to_string(VT[i]));
        variant_copy.replace(0, variant_copy.find("},")+4, "");

        x1 = variant_copy.find("{");
        x2 = variant_copy.find("}");
        auto VN = variant_copy.substr(x1+1, x2-x1-1);
        while(VN.find(",") != string::npos) replace(VN, ", ", "");
        for(int i=0; i<VN.size(); i++) terminalVariables.emplace_back(to_string(VN[i]));

        variant_copy.replace(0, variant_copy.find("},")+4, "");
        variant_copy.replace(0, variant_copy.find("{")+4, "");
        variant_copy.replace(variant_copy.size()-3, 1, "");
        while(variant_copy.find(" ") != string::npos) replace(variant_copy, " ", "");
//        cout << variant_copy << endl;

        string tmp;
        while(variant_copy.find("\n") != string::npos) {
            productions.emplace_back(Rule(variant_copy.substr(0, 1), variant_copy.substr(4, variant_copy.find("\n")-4)));
            replace(productions[productions.size()-1].Right, "\r", "");
            variant_copy.replace(0, variant_copy.find("\n")+1, "");
//            cout << productions[productions.size()-1] << endl;
            if(variant_copy.size() < 3) break;
        }
//        cout << endl;

        startingCharacter = "S";
    }
};