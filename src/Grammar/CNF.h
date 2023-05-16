#pragma once

#include <iostream>
#include "grammar.h"

class CNF : public Grammar  {

    CNF() : Grammar() {}
    CNF() : Grammar(Grammar A) {};

    void toCNF() {
        eliminateEpsilonProductions();
        eliminateRenaming();
        eliminateInaccessibleSymbols();
        eliminateNonproductiveSymbols();
        convertToChomskyNormalForm();

        std::sort(productions.begin(), productions.end(), [](const rule &a, const rule &b) {
            if (a.Left == b.Left) {
                return a.Right < b.Right;
            }
            return a.Left < b.Left;
        });
        auto it = std::unique(productions.begin(), productions.end(), [](const rule &a, const rule &b) {
            return a.Left == b.Left && a.Right == b.Right;
        });
        productions.erase(it, productions.end());
        for (const auto &r: productions) {
            std::cout << r.Left << " -> " << r.Right << std::endl;
        }
    }

private:
    void eliminateEpsilonProductions() {
        unordered_set <string> epsilonVars;

        for (const auto &r: productions) {
            if (r.Right == "ε") {
                epsilonVars.insert(r.Left);
            }
        }
        while (!epsilonVars.empty()) {
            string var = *epsilonVars.begin();
            epsilonVars.erase(epsilonVars.begin());
            for (auto it = productions.begin(); it != productions.end();) {
                if (it->Right == "ε") {
                    it = productions.erase(it);
                } else if (it->Right.find(var) != string::npos) {
                    vector <string> combinations = {""};
                    for (const auto &c: it->Right) {
                        if (string(1, c) == var) {
                            size_t n = combinations.size();
                            for (size_t i = 0; i < n; i++) {
                                combinations.push_back(combinations[i]);
                                combinations.back().push_back(c);
                                combinations[i].push_back(' ');
                            }
                        } else {
                            for (auto &s: combinations) {
                                s.push_back(c);
                            }
                        }
                    }
                    for (const auto &s: combinations) {
                        if (s != it->Right) {
                            productions.push_back({it->Left, s});
                        }
                    }

                    it = productions.erase(it);
                } else {
                    ++it;
                }
            }
            for (auto it = productions.begin(); it != productions.end();) {
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
        map < string, vector < vector < string>>> replacements;
        for (const auto &r: productions) {
            if (r.Right.size() == 1 && isupper(r.Right[0])) {
                replacements.emplace(r.Left, vector < vector < string >> {{r.Right}});
            } else {
                vector <vector<string>> right_parts;
                vector <string> current_part;
                for (char c: r.Right) {
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
                    replacements.emplace(r.Left, vector < vector < string >> {right_parts[0]});
                } else {
                    replacements[r.Left].push_back(right_parts[0]);
                }
                for (size_t i = 1; i < right_parts.size(); i++) {
                    replacements[r.Left].push_back(right_parts[i]);
                }
            }
        }

        vector <rule> new_productions;
        for (const auto &r: productions) {
            if (replacements.count(r.Left) == 0) {
                new_productions.push_back(r);
            } else {
                vector <vector<string>> replacement_parts = replacements[r.Left];
                for (const auto &p: replacement_parts) {
                    string new_right;
                    for (const auto &s: p) {
                        new_right += s;
                    }
                    new_productions.push_back({r.Left, new_right});
                }
            }
        }
        productions = new_productions;
    }

    void eliminateInaccessibleSymbols() {
        vector <string> reachable_symbols{startingCharacter};
        bool found_new_symbol = true;
        while (found_new_symbol) {
            found_new_symbol = false;
            for (const auto &r: productions) {
                if (find(reachable_symbols.begin(), reachable_symbols.end(), r.Left) != reachable_symbols.end()) {
                    for (char c: r.Right) {
                        if (isupper(c)) {
                            string var(1, c);
                            if (find(reachable_symbols.begin(), reachable_symbols.end(), var) ==
                                reachable_symbols.end()) {
                                reachable_symbols.push_back(var);
                                found_new_symbol = true;
                            }
                        }
                    }
                }
            }
        }

        vector <rule> new_productions;
        for (const auto &r: productions) {
            bool is_reachable = true;
            for (char c: r.Right) {
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
        productions = new_productions;
    }

    void eliminateNonproductiveSymbols() {
        vector <string> productive_symbols{nonTerminalVariables};
        bool found_new_symbol = true;
        while (found_new_symbol) {
            found_new_symbol = false;
            for (const auto &r: productions) {
                if (find(productive_symbols.begin(), productive_symbols.end(), r.Left) != productive_symbols.end()) {
                    bool is_productive = true;
                    for (char c: r.Right) {
                        if (isupper(c)) {
                            string var(1, c);
                            if (find(productive_symbols.begin(), productive_symbols.end(), var) ==
                                productive_symbols.end()) {
                                is_productive = false;
                                break;
                            }
                        }
                    }
                    if (is_productive) {
                        string var = string() + r.Left;
                        if (find(productive_symbols.begin(), productive_symbols.end(), var) ==
                            productive_symbols.end()) {
                            productive_symbols.push_back(var);
                            found_new_symbol = true;
                        }
                    }
                }
            }
        }
        vector <string> new_non_terminal_variables;
        vector <rule> new_productions;
        for (const auto &var: productive_symbols) {
            if (find(nonTerminalVariables.begin(), nonTerminalVariables.end(), var) != nonTerminalVariables.end()) {
                new_non_terminal_variables.push_back(var);
            }
            for (const auto &r: productions) {
                if (r.Left == var || find(r.Right.begin(), r.Right.end(), var[0]) != r.Right.end()) {
                    new_productions.push_back(r);
                }
            }
        }
        nonTerminalVariables = new_non_terminal_variables;
        productions = new_productions;
    }

    void convertToChomskyNormalForm() {
        map<string, int> indices;
        int index = 0;
        for (const string &var: nonTerminalVariables) {
            indices[var] = index++;
        }
        for (rule &r: productions) {
            if (r.Right.size() > 2) {
                string newVar = "X" + to_string(index++);
                int length = r.Right.length();
                for (int i = 0; i < length - 2; i++) {
                    string newRuleVar = (i == 0) ? r.Left : newVar;
                    string newRuleRight = r.Right.substr(i, 2);
                    productions.push_back({newRuleVar, newRuleRight});
                    r.Right.replace(i, 2, newRuleVar);
                }
                productions.push_back({newVar, r.Right.substr(length - 2, 2)});
                r.Right.replace(length - 2, 2, newVar);
            }
        }
    }
};