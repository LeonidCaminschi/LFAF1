#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>
#include "lexer.h"

class Parser {
private:
    std::vector<Token> tokens;
    size_t currentTokenIndex;

public:
    Parser(std::vector<Token> tokens) : tokens(tokens), currentTokenIndex(0) {}

    double parse() {
        return expression();
    }

private:
    double expression() {
        double result = term();

        while (currentTokenIndex < tokens.size() &&
               (tokens[currentTokenIndex].value == "+" || tokens[currentTokenIndex].value == "-")) {
            std::string op = tokens[currentTokenIndex++].value;
            if (op == "+") {
                result += term();
            } else {
                result -= term();
            }
        }

        return result;
    }

    double term() {
        double result = factor();

        while (currentTokenIndex < tokens.size() &&
               (tokens[currentTokenIndex].value == "*" || tokens[currentTokenIndex].value == "/")) {
            std::string op = tokens[currentTokenIndex++].value;
            if (op == "*") {
                result *= factor();
            } else {
                double divisor = factor();
                if (divisor != 0.0) {
                    result /= divisor;
                } else {
                    throw std::runtime_error("Error: Division by zero.");
                }
            }
        }

        return result;
    }

    double factor() {
        if (currentTokenIndex >= tokens.size()) {
            throw std::runtime_error("Error: Unexpected end of input.");
        }

        Token token = tokens[currentTokenIndex++];
        if (token.type == TokenType::NUMBER) {
            return std::stod(token.value);
        } else if (token.type == TokenType::LPAREN) {
            double result = expression();
            if (currentTokenIndex >= tokens.size() || tokens[currentTokenIndex++].type != TokenType::RPAREN) {
                throw std::runtime_error("Error: Missing closing parenthesis.");
            }
            return result;
        } else {
            throw std::runtime_error("Error: Invalid token encountered.");
        }
    }
};
