#include <iostream>
#include <fstream>
#include "grammar.h"
#include "automata.h"
#include "lexer.h"
#include "parser.h"

int main() {
    string expression = "2*((3+4)/5)/13.451";
    Lexer lexer(expression);
    vector<Token> tokens = lexer.tokenize();

    if (expression.length() > 0 && tokens.size() == 0) {
        cout << "An error occured please revisit the initial expression" << endl;
    } else {
        for (Token token: tokens) {
            cout << token << endl;
        }
    }

    Parser parser(tokens);
    try {
        double result = parser.parse();
        std::cout << "Result: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
