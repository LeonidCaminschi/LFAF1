#include <iostream>
#include <fstream>
#include "grammar.h"
#include "automata.h"

int main() {
    int counter = 0;
    fstream file("../var");
    string variant = "";
    string line;

    while (getline(file, line)) {
        if (line != "") variant += line + "\n";
        if (line.find('}') != string::npos) counter++;
        if (counter % 3 == 0 && counter > 0) {
            if (variant != "\r\n" && variant.find("{") != string::npos) {
                Grammar grammar(variant);
                Automata automata(grammar);
                for (int i=1; i<6; i++) {
                    string word = grammar.generateWord();
                    cout << "Word #" << i << ": " << word << endl;
                    cout << "String " << i << " " << (automata.validate(word) ? "true" : "false") << endl;
                }
                break; // remove break for all 32 variants :)
            }
        }
    }
    file.close();
}
