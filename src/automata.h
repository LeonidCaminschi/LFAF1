#pragma once

#include <map>
#include <sstream>
#include <queue>
#include "grammar.h"

struct Automata {
    std::map<std::pair<std::string, std::string>, std::string> transitions;
    std::vector<std::string> states;
    std::vector<std::string> symbols;
    std::string start_state;
    std::vector<std::string> final_states;

    Automata() {}
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

    Grammar AutomataToGrammar(const Automata& automata) {
        Grammar grammar;
        grammar.nonTerminalVariables = automata.states;
        grammar.terminalVariables = automata.symbols;
        grammar.startingCharacter = automata.start_state;

        for (const auto& [key, value] : automata.transitions) {
            string lhs = key.first;
            string rhs = value;
            grammar.productions.push_back({lhs, rhs});
        }

        return grammar;
    }

    bool isDeterministic() const {
        std::set<std::pair<std::string, std::string>> visited;
        for (const auto& [key, value] : transitions) {
            std::string state = key.first;
            std::string symbol = key.second;
            if (visited.count({state, symbol}))
                return false;
            visited.insert({state, symbol});
        }
        return true;
    }

    std::set<std::string> epsilonClosure(const std::set<std::string>& states) {
        std::set<std::string> closure = states;
        std::queue<std::string> frontier;
        for (const auto& state : states) {
            frontier.push(state);
        }
        while (!frontier.empty()) {
            std::string state = frontier.front();
            frontier.pop();
            for (const auto& [key, value] : transitions) {
                if (key.first == state && key.second == "") {
                    if (closure.count(value) == 0) {
                        closure.insert(value);
                        frontier.push(value);
                    }
                }
            }
        }
        return closure;
    }

    std::string stateSetToString(const std::set<std::string>& stateSet) {
        std::stringstream ss;
        ss << "{";
        bool first = true;
        for (const auto& state : stateSet) {
            if (!first) {
                ss << ",";
            }
            ss << state;
            first = false;
        }
        ss << "}";
        return ss.str();
    }

    bool hasIntersection(const std::set<std::string>& set1, const std::set<std::string>& set2) {
        for (const auto& element : set1) {
            if (set2.count(element) > 0) {
                return true;
            }
        }
        return false;
    }

    void convertNFAToDFA() {
        std::map<std::pair<std::string, std::string>, std::string> dfaTransitions;
        std::set<std::string> unprocessedDFAStates;
        std::map<std::string, std::set<std::string>> nfaStateSubsets;
        std::set<std::string> startNFAStateSubset = epsilonClosure({start_state});
        std::string startDFAState = stateSetToString(startNFAStateSubset);
        nfaStateSubsets[startDFAState] = startNFAStateSubset;
        unprocessedDFAStates.insert(startDFAState);

        while (!unprocessedDFAStates.empty()) {
            std::string dfaState = *unprocessedDFAStates.begin();
            unprocessedDFAStates.erase(dfaState);

            for (const auto& symbol : symbols) {
                std::set<std::string> nfaStateSubset = move(nfaStateSubsets[dfaState]);
                nfaStateSubset = epsilonClosure(nfaStateSubset);

                std::string nfaStateSubsetString = stateSetToString(nfaStateSubset);
                if (nfaStateSubsets.count(nfaStateSubsetString) == 0) {
                    nfaStateSubsets[nfaStateSubsetString] = nfaStateSubset;
                    unprocessedDFAStates.insert(nfaStateSubsetString);
                }

                std::string nextDFAState = nfaStateSubsetString;
                dfaTransitions[make_pair(dfaState, symbol)] = nextDFAState;
            }
        }

        transitions = dfaTransitions;

        states.clear();
        final_states.clear();
        for (const auto& [dfaState, nfaStateSubset] : nfaStateSubsets) {
            states.push_back(dfaState);
            if (hasIntersection(nfaStateSubset, std::set(final_states.begin(), final_states.end()))) {
                final_states.push_back(dfaState);
            }
        }
    }
};