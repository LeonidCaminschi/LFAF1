#pragma once

#include <iostream>
#include <string>
#include <regex>
#include <vector>

using namespace std;

enum class TokenType {
    NUMBER, OPERATOR, LPAREN, RPAREN
};

// Define Token class
class Token {
public:
    TokenType type;
    string value;

    Token(TokenType type, string value) {
        this->type = type;
        this->value = value;
    }

    friend ostream& operator<<(ostream& os, const Token& token) {
        os << "Token(" << static_cast<int>(token.type) << ", \"" << token.value << "\")";
        return os;
    }
};

class Lexer {
private:
    vector<Token> tokens;
    string text;
    int position;

public:
    Lexer(string text) {
        this->text = text;
        position = 0;
    }

    vector<Token> tokenize() {
        regex pattern("(\\d+\\.?\\d*)|([+*/\\-()])");
        smatch match;
        int lpar = 0, rpar = 0;
        int continuity = -1;
        while (regex_search(text, match, pattern)) {
//            std::cout << text << endl; // debug purposes
            for (size_t i = 1; i < match.size(); i++) {
                if (!match[i].str().empty()) {
                    TokenType type;
                    string value = match[i].str();
                    if (i == 1) {
                        type = TokenType::NUMBER;
                        if (continuity == 0) {
                            tokens.clear();
                            return tokens;
                        }
                        continuity = 0;
                    } else if (value == "+") {
                        type = TokenType::OPERATOR;
                        if (continuity == 1) {
                            tokens.clear();
                            return tokens;
                        }
                        continuity = 1;
                    } else if (value == "-") {
                        type = TokenType::OPERATOR;
                        if (continuity == 1) {
                            tokens.clear();
                            return tokens;
                        }
                        continuity = 1;
                    } else if (value == "*") {
                        type = TokenType::OPERATOR;
                        if (continuity == 1) {
                            tokens.clear();
                            return tokens;
                        }
                        continuity = 1;
                    } else if (value == "/") {
                        type = TokenType::OPERATOR;
                        if (continuity == 1) {
                            tokens.clear();
                            return tokens;
                        }
                        continuity = 1;
                    } else if (value == "(") {
                        type = TokenType::LPAREN;
                        lpar++;
                    } else if (value == ")") {
                        type = TokenType::RPAREN;
                        rpar++;
                    }
                    Token token(type, value);
                    tokens.push_back(token);
                    position += match[i].length();
                    break;
                }
            }
            text = match.suffix().str();
        }
        if (lpar == rpar) {
            return tokens;
        } else {
            tokens.clear();
            return tokens;
        }
    }
};