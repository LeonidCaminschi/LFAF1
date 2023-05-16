# Laboratory 5 Parser & Building an abstract Tree

### Course: Formal Languages & Finite Automata
### Author: Caminschi Leonid FAF-211 (Leonidas)

----

## Theory
Short theory about parser and abstract syntax tree.
Parser is the next layer after lexer and tokenization was done, it is responsible for constructing the parser tree or Abstract syntax tree that represents the hierachical structure of the code at which point it should be executed by the next layer an interpreter


## Objectives:
1. Get familiar with parsing, what it is and how it can be programmed [1].
2. Get familiar with the concept of AST [2].
3. In addition to what has been done in the 3rd lab work do the following:
   1. In case you didn't have a type that denotes the possible types of tokens you need to:
      1. Have a type __*TokenType*__ (like an enum) that can be used in the lexical analysis to categorize the tokens. 
      2. Please use regular expressions to identify the type of the token.
   2. Implement the necessary data structures for an AST that could be used for the text you have processed in the 3rd lab work.
   3. Implement a simple parser program that could extract the syntactic information from the input text.

## Implementation description

On the implementation of the solution to this laboratory work i tried doing everything on paper and understand
everything thoroughly before trying to code the solution to this problem so that i have the basics in the head
before telling the computer to do it, it was harder than expected but in the end i think it ended up working well.
All was done.

* Code snippets from my files.

The following snippets of code and comments were done on a Top-down parser.

```
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
```

Parses a factor, which can be a number or a parenthesized expression. If a number is encountered, it converts and returns the numeric value. If a left parenthesis is encountered, it recursively calls expression to evaluate the subexpression within the parentheses. Throws errors for unexpected end of input, missing closing parenthesis, or encountering an invalid token.

```
double term() {
    double result = factor();

    while (currentTokenIndex < tokens.size() &&(tokens[currentTokenIndex].value == "*" || tokens[currentTokenIndex].value == "/")) 
    {
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
```

Parses a term, which consists of factors separated by multiplication or division operators. It recursively calls factor and performs multiplication or division based on the encountered operators. Handles division by zero exception.

```
double expression() {
    double result = term();

    while (currentTokenIndex < tokens.size() && (tokens[currentTokenIndex].value == "+" || tokens[currentTokenIndex].value == "-")) 
    {
        std::string op = tokens[currentTokenIndex++].value;
        if (op == "+") {
            result += term();
        } else {
            result -= term();
        }
    }
    return result;
}
```

Parses an expression, which consists of terms separated by addition or subtraction operators. It recursively calls term and performs addition or subtraction based on the encountered operators.

```
double parse() {
    return expression();
}
```

Entry point of the parsing process. It calls the expression function to parse and evaluate the expression and returns the final result.

## Conclusions / Screenshots / Results

Result after executing the code:

```
Result: 0.208163
```

Sadly couldnt upload photos but uploaded the output of the program execution.
for execution on own machine do the following commands:<br />

```
cd src
mkdir build
cd build
cmake ..
make
./LFAF
```

Reformated the repository removing the unused code from main leaving it as minimal as possible and because i had from my opinion to many files used in different places i made directories for each code representing a lab or a group of laboratories.

In conclusion i can say that i have learned a lot about CNF and reinforced the knowledge
given to me at the course.

## References

*Mr. Drumea's Laboratories*<br />
*Mrs. Cojuhari's Lectures*

## Online References

https://en.wikipedia.org/wiki/Parsing<br />
https://www.youtube.com/watch?v=4m7ubrdbWQU&t=45s<br />
https://www.geeksforgeeks.org/types-of-parsers-in-compiler-design/
