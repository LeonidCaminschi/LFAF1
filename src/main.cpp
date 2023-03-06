#include <iostream>
#include <fstream>
#include "grammar.h"
#include "automata.h"

int main() {
    int counter = 0;
    fstream file("../var");
    if (!file) cout << "Can't open input file!";
    string variant = "";
    string line;
    while (getline(file, line)) {
        if (line != "") variant += line + "\n";
        if (line.find('}') != string::npos) counter++;
        if (counter % 3 == 0 && counter > 0) {
            if (variant != "\r\n" && variant.find("{") != string::npos) {
//                cout << variant << endl;
                Grammar grammar(variant);
                string s1 = grammar.generateWord();
                cout << "Word #1: " << s1 << endl;
                string s2 = grammar.generateWord();
                cout << "Word #2: " << s2 << endl;
                string s3 = grammar.generateWord();
                cout << "Word #3: " << s3 << endl;
                string s4 = grammar.generateWord();
                cout << "Word #4: " << s4 << endl;
                string s5 = grammar.generateWord();
                cout << "Word #5: " << s5 << endl;
                cout << endl;
                Automata automata(grammar);
                cout << "String 1 " << (automata.validate(s1) ? "true" : "false") << endl;
                cout << "String 2 " << (automata.validate(s2) ? "true" : "false") << endl;
                cout << "String 3 " << (automata.validate(s3) ? "true" : "false") << endl;
                cout << "String 4 " << (automata.validate(s4) ? "true" : "false") << endl;
                cout << "String 5 " << (automata.validate(s5) ? "true" : "false") << endl;
                cout << endl;
                variant = "";
//                break; // remove break for all 32 variants :)
            }
        }
    }
    file.close();
}
