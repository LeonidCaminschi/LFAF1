#include <iostream>
#include <fstream>
#include "Grammar/grammar.h"
#include "Automata/automata.h"
#include "DSL/lexer.h"
#include "DSL/parser.h"

int main() {
    string expression = "2*((3+4)/5)/13.451";
    Lexer lexer(expression);
    vector<Token> tokens = lexer.tokenize();
    Parser parser(tokens);
    try {
        double result = parser.parse();
        std::cout << "Result: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
