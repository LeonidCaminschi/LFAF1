# Laboratory 3 Lexer and Tokenizer

### Course: Formal Languages & Finite Automata
### Author: Caminschi Leonid FAF-211 (Leonidas)

----

## Theory
Short theory for creating a lexer and tokenizer we need to make generalized regex expressions
to check with our input data and them to check the given text with the given regex expression
and to iterate through the elements 1 by 1 until we have tokenized each part of the given input
and give specific situation to result in error messages such as non equal number of parantheses
and multiple operators one after another.


## Objectives:

* Get accustomed to Lexers and Tokenizers
* Learn how to create a Tokenizer and Lexer
* Understand how it is aplicable to language creation
* Learn differences of Tokenizer and Lexer

## Implementation description

On the implementation of the solution to this laboratory work i tried doing everything on paper and understand
everything thoroughly before trying to code the solution to this problem so that i have the basics in the head
before telling the computer to do it, it was harder than expected but in the end i think it ended up working well.

* Code snippets from your files.

```

enum class TokenType {
    NUMBER, OPERATOR, LPAREN, RPAREN
};

```

Enumerate all the parts of the text as different components

```

class Token {
public:
    TokenType type;
    string value;

    Token(TokenType type, string value) {
        this->type = type;
        this->value = value;
    }
}

```

I have each token as its type and the value itself so we can divide the text into components
or tokens

```

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
}

```

I have a class lexer which will take the text itself and set the position to 0 and a vector of
tokens where I am going to save all of the components of the given text

```

        regex pattern("(\\d+\\.?\\d*)|([+*/\\-()])");
        smatch match;
        int lpar = 0, rpar = 0;
        int continuity = -1;

```

so here we have the general regex with which I am going to use to determine the text each
component separately to break it down it will look for a *digit* or *multiple digits* and if the
*number* has a *. (dot)* and *digits* afterwards so the following numbers are allowed 
*13* *48.3* ...
*OR* it searches for an operator such as *+ \* / - ()* if any of the conditions match then the 
string is matched

```

 while (regex_search(text, match, pattern)) {
//            std::cout << text << endl; // debug purposes
            for (size_t i = 1; i < match.size(); i++) {
```

So my loop works until theres at least 1 match in the given text input and then iterates through the matched string size.

```

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

```

and inside the loop itself is a if where it searches what did it match with and after finding
gives it a type and gets stored inside the token vectors. but if there are different numbers
of left parantheses and right parantheses it will give an warning message and leave the loop or
if there are 2 operators one after another or there are 2 numbers 1 after another.


## Conclusions / Screenshots / Results

Result after executing the code:

```
Token(0, "2")
Token(1, "*")
Token(2, "(")
Token(2, "(")
Token(0, "3")
Token(1, "+") 
Token(0, "4")
Token(3, ")")
Token(1, "/")
Token(0, "5")
Token(3, ")")
Token(1, "/")
Token(0, "13.451")  
```

Sadly couldnt upload photos but uploaded the output of the program execution.
for execution on own machine do the following commands:<br />
cd src<br />
mkdir build<br />
cd build<br />
cmake ..<br />
make<br />
./LFAF<br />

Updated from last laboratory everything unused was commented in case of need to use them in
later laboratories otherwise everything was left as is

In conclusion i can say that i have learned a lot about Tokenizers and Lexers and their
properties and how to differentiate a Tokenizer to a Lexer.

## References

*Mr. Drumea's Laboratories*<br />
*Mrs. Cojuhari's Lectures*
