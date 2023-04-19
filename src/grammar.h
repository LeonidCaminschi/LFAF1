#pragma once

#include <vector>
#include <random>
#include <cstdlib>
#include <csignal>
#include <set>
#include <unordered_set>
#include <map>

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

    void toCNF() {
        eliminateEpsilonProductions();
        eliminateRenaming();
        eliminateInaccessibleSymbols();
        eliminateNonproductiveSymbols();
        convertToChomskyNormalForm();

        // Sort the rules vector
        std::sort(productions.begin(), productions.end(), [](const Rule& a, const Rule& b) {
            if (a.Left == b.Left) {
                return a.Right < b.Right;
            }
            return a.Left < b.Left;
        });

        // Use std::unique to remove duplicates
        auto it = std::unique(productions.begin(), productions.end(), [](const Rule& a, const Rule& b) {
            return a.Left == b.Left && a.Right == b.Right;
        });

        // Erase the duplicates from the vector
        productions.erase(it, productions.end());

        // Print the rules vector to verify the duplicates are removed
        for (const auto& r : productions) {
            std::cout << r.Left << " -> " << r.Right << std::endl;
        }
    }

private:
    void eliminateEpsilonProductions() {
        unordered_set<string> epsilonVars;

        // Find variables that can derive ε
        for (const auto& r : productions) {
            if (r.Right == "ε") {
                epsilonVars.insert(r.Left);
            }
        }

        // Eliminate ε productions
        while (!epsilonVars.empty()) {
            string var = *epsilonVars.begin();
            epsilonVars.erase(epsilonVars.begin());

            // Remove ε productions with var on the right-hand side
            for (auto it = productions.begin(); it != productions.end(); ) {
                if (it->Right == "ε") {
                    it = productions.erase(it);
                } else if (it->Right.find(var) != string::npos) {
                    // Generate all possible combinations of the right-hand side without var
                    vector<string> combinations = {""};
                    for (const auto& c : it->Right) {
                        if (string(1, c) == var) {
                            size_t n = combinations.size();
                            for (size_t i = 0; i < n; i++) {
                                combinations.push_back(combinations[i]);
                                combinations.back().push_back(c);
                                combinations[i].push_back(' ');
                            }
                        } else {
                            for (auto& s : combinations) {
                                s.push_back(c);
                            }
                        }
                    }

                    // Add the new productions
                    for (const auto& s : combinations) {
                        if (s != it->Right) {
                            productions.push_back({it->Left, s});
                        }
                    }

                    // Remove the original production
                    it = productions.erase(it);
                } else {
                    ++it;
                }
            }

            // Remove ε productions with var on the left-hand side
            for (auto it = productions.begin(); it != productions.end(); ) {
                if (it->Left == var && it->Right == "ε") {
                    it = productions.erase(it);
                } else if (it->Right.find(var) != string::npos) {
                    productions.push_back({it->Left, it->Right});
                    it = productions.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }

    void eliminateRenaming() {
        // Find all variables that can be directly replaced by another variable
        map<string, vector<vector<string>>> replacements;
        for (const auto& r : productions) {
            if (r.Right.size() == 1 && isupper(r.Right[0])) {
                replacements.emplace(r.Left, vector<vector<string>>{{r.Right}});
            } else {
                vector<vector<string>> right_parts;
                vector<string> current_part;
                for (char c : r.Right) {
                    if (isupper(c)) {
                        if (!current_part.empty()) {
                            right_parts.push_back(current_part);
                            current_part.clear();
                        }
                        current_part.push_back(string(1, c));
                    } else {
                        current_part.push_back(string(1, c));
                    }
                }
                if (!current_part.empty()) {
                    right_parts.push_back(current_part);
                }
                if (replacements.count(r.Left) == 0) {
                    replacements.emplace(r.Left, vector<vector<string>>{right_parts[0]});
                } else {
                    replacements[r.Left].push_back(right_parts[0]);
                }
                for (size_t i = 1; i < right_parts.size(); i++) {
                    replacements[r.Left].push_back(right_parts[i]);
                }
            }
        }

        // Replace each nonterminal with its expansions
        vector<Rule> new_productions;
        for (const auto& r : productions) {
            if (replacements.count(r.Left) == 0) {
                new_productions.push_back(r);
            } else {
                vector<vector<string>> replacement_parts = replacements[r.Left];
                for (const auto& p : replacement_parts) {
                    string new_right;
                    for (const auto& s : p) {
                        new_right += s;
                    }
                    new_productions.push_back({r.Left, new_right});
                }
            }
        }

        // Update the grammar with the new productions
        productions = new_productions;
    }

    void eliminateInaccessibleSymbols() {
        // Start with the starting symbol
        vector<string> reachable_symbols{startingCharacter};

        // Repeat until no new reachable symbols are found
        bool found_new_symbol = true;
        while (found_new_symbol) {
            found_new_symbol = false;
            for (const auto& r : productions) {
                // If the left-hand side of the production is reachable,
                // add its right-hand side to the set of reachable symbols
                if (find(reachable_symbols.begin(), reachable_symbols.end(), r.Left) != reachable_symbols.end()) {
                    for (char c : r.Right) {
                        if (isupper(c)) {
                            string var(1, c);
                            if (find(reachable_symbols.begin(), reachable_symbols.end(), var) == reachable_symbols.end()) {
                                reachable_symbols.push_back(var);
                                found_new_symbol = true;
                            }
                        }
                    }
                }
            }
        }

        // Remove all productions that contain unreachable symbols
        vector<Rule> new_productions;
        for (const auto& r : productions) {
            bool is_reachable = true;
            for (char c : r.Right) {
                if (isupper(c)) {
                    string var(1, c);
                    if (find(reachable_symbols.begin(), reachable_symbols.end(), var) == reachable_symbols.end()) {
                        is_reachable = false;
                        break;
                    }
                }
            }
            if (is_reachable) {
                new_productions.push_back(r);
            }
        }

        // Update the grammar with the new productions
        productions = new_productions;
    }

    void eliminateNonproductiveSymbols() {
        // Start with all non-terminal variables
        vector<string> productive_symbols{nonTerminalVariables};

        // Repeat until no new productive symbols are found
        bool found_new_symbol = true;
        while (found_new_symbol) {
            found_new_symbol = false;
            for (const auto& r : productions) {
                // If the left-hand side of the production is productive,
                // add its right-hand side to the set of productive symbols
                if (find(productive_symbols.begin(), productive_symbols.end(), r.Left) != productive_symbols.end()) {
                    bool is_productive = true;
                    for (char c : r.Right) {
                        if (isupper(c)) {
                            string var(1, c);
                            if (find(productive_symbols.begin(), productive_symbols.end(), var) == productive_symbols.end()) {
                                is_productive = false;
                                break;
                            }
                        }
                    }
                    if (is_productive) {
                        string var = string() + r.Left;
                        if (find(productive_symbols.begin(), productive_symbols.end(), var) == productive_symbols.end()) {
                            productive_symbols.push_back(var);
                            found_new_symbol = true;
                        }
                    }
                }
            }
        }

        // Remove all non-productive symbols
        vector<string> new_non_terminal_variables;
        vector<Rule> new_productions;
        for (const auto& var : productive_symbols) {
            if (find(nonTerminalVariables.begin(), nonTerminalVariables.end(), var) != nonTerminalVariables.end()) {
                new_non_terminal_variables.push_back(var);
            }
            for (const auto& r : productions) {
                if (r.Left == var || find(r.Right.begin(), r.Right.end(), var[0]) != r.Right.end()) {
                    new_productions.push_back(r);
                }
            }
        }

        // Update the grammar with the new non-terminal variables and productions
        nonTerminalVariables = new_non_terminal_variables;
        productions = new_productions;
    }

    void convertToChomskyNormalForm() {
        map<string, int> indices;
        int index = 0;
        for (const string& var : nonTerminalVariables) {
            indices[var] = index++;
        }

        for (Rule& r : productions) {
            if (r.Right.size() > 2) {
                string newVar = "X" + to_string(index++);
                int length = r.Right.length();
                for (int i = 0; i < length - 2; i++) {
                    string newRuleVar = (i == 0) ? r.Left : newVar;
                    string newRuleRight = r.Right.substr(i, 2);
                    productions.push_back({ newRuleVar, newRuleRight });
                    r.Right.replace(i, 2, newRuleVar);
                }
                productions.push_back({ newVar, r.Right.substr(length - 2, 2) });
                r.Right.replace(length - 2, 2, newVar);
            }
        }
    }
};