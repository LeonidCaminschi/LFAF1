#include <iostream>
#include <fstream>
#include "grammar.h"
#include "automata.h"
#include "lexer.h"

int main() {
//    int counter = 0;
//    fstream file("../var");
//    string variant = "";
//    string line;
//
//    while (getline(file, line)) {
//        if (line != "") variant += line + "\n";
//        if (line.find('}') != string::npos) counter++;
//        if (counter % 3 == 0 && counter > 0) {
//            if (variant != "\r\n" && variant.find("{") != string::npos) {
//                Grammar grammar(variant);
//                Automata automata(grammar);
//                Automata::AutomataToGrammar(automata);
//                cout << grammar.clasify() << endl;
//                for (int i=1; i<6; i++) {
//                    string word = grammar.generateWord();
//                    cout << "Word #" << i << ": " << word << endl;
//                    cout << "String " << i << " " << (automata.validate(word) ? "true" : "false") << endl;
//                }
//                break; // remove break for all 32 variants :)
//                variant = "";
//            }
//        }
//    }
//    file.close();
//
//    Automata automata2;
//    automata2.final_states = {"q3"};
//    automata2.start_state = "q0";
//    automata2.symbols = {"q0", "q1", "q2", "q3"};
//    automata2.states = {"a", "b"};
//    automata2.transitions =
//            {
//                {{"q0","a"},"q1"},
//                {{"q1","b"},"q2"},
//                {{"q2","b"},"q3"},
//                {{"q3","a"},"q1"},
//                {{"q2","b"},"q2"},
//                {{"q1","a"},"q1"}
//            };
//
//    cout << "This automata is: " << (automata2.isDeterministic() ? "Deterministic" : "Non-Deterministic") << endl;
//    automata2.convertNFAToDFA();
//
//    string expression = "2*((3+4)/5)/13.451";
//    Lexer lexer(expression);
//    vector<Token> tokens = lexer.tokenize();
//    if (expression.length() > 0 && tokens.size() == 0) {
//        cout << "An error occured please revisit the initial expression" << endl;
//    } else {
//        for (Token token: tokens) {
//            cout << token << endl;
//        }
//    }


    Grammar test;
    test.terminalVariables.push_back("S");
    test.terminalVariables.push_back("A");
    test.terminalVariables.push_back("B");
    test.terminalVariables.push_back("C");
    test.terminalVariables.push_back("E");

    test.nonTerminalVariables.push_back("a");
    test.nonTerminalVariables.push_back("b");

    test.startingCharacter = "S";

    test.productions.push_back(Rule("S", "bA"));
    test.productions.push_back(Rule("S", "B"));
    test.productions.push_back(Rule("A", "a"));
    test.productions.push_back(Rule("A", "aS"));
    test.productions.push_back(Rule("A", "bAaAb"));
    test.productions.push_back(Rule("B", "AC"));
    test.productions.push_back(Rule("B", "bS"));
    test.productions.push_back(Rule("B", "aAa"));
    test.productions.push_back(Rule("C", "ε"));
    test.productions.push_back(Rule("C", "AB"));
    test.productions.push_back(Rule("E", "BA"));

    test.toCNF();

//    for (auto it : test.productions) {
//        cout << it << endl;
//    }
}
