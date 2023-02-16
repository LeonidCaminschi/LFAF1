#pragma once

#include <map>
#include "grammar.h"

struct Automata {
    std::map<std::pair<std::string, std::string>, std::string> transitions;
    std::vector<std::string> states;
    std::vector<std::string> symbols;
    std::string start_state;
    std::vector<std::string> final_states;

    Automata(Grammar& gr) {
        for (Rule v : gr.productions) {
            transitions[make_pair(v[0], v[1].substr(0, 1))] = v[1].substr(1, 1);
            states.push_back(v[0]);
            symbols.push_back(v[1].substr(0, 1));
        }

        start_state = "S";
        final_states = gr.terminalVariables;
    }

    bool validate(std::string str) {
        std::string curr = start_state;
        for (auto&& c : str) {
            std::string symbol(1, c);
            if (std::count(symbols.begin(), symbols.end(), symbol) == 0) {
                return true;
            }

            curr = transitions[std::make_pair(curr, symbol)];
        }

        return std::count(final_states.begin(), final_states.end(), curr) > 0;
    }
};