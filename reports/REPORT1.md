# The title of the work

### Course: Formal Languages & Finite Automata
### Author: Caminschi Leonid FAF-211 (Leonidas)

----

## Theory
Short summary of Grammar and Automata in the grammar we have terminal and
non terminal variables such as terminal [a, b, c, d ...] and non terminal
[A, B, C ...] and the set of rules or production which say what can be
exchanged into what S -> aB and so on and the starting character which 
is usualt just a regular "S".
Automata on the other hand stores these rules as deltas while we had 
S -> aB at the grammar the automatas would look something like this
delta[S, a] -> B or
delta[S, a] -> X (which indicates the end of the string)


## Objectives:

* Get accustomed to Grammar and Automata
* Learn how to transform from one to another
* Understand how it is aplicable to language creation
* Make the task harder even tho nobody asked for it xD

## Implementation description

I didn't read the conditions of the laboratory and ended up making a general
solution for every single variant through 1-32 and didn't do it for just 1 variant.
I ended up making a parser to parse through each variant and take valuable data out
so that i have all of the data at my disposition to create a grammar and generate random
words from its language.

* Code snippets from your files.

```
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
                variant = "";
                break; // remove break for all 32 variants :)
            }
        }
    }
    file.close();
}
```

## Conclusions / Screenshots / Results

Variant 1:
Word #1: bfeeeffeeeffffea
Word #2: ba
Word #3: acdefa
Word #4: bfffa
Word #5: ae

String 1 true
String 2 true
String 3 true
String 4 true
String 5 true

Variant 2:
Word #1: deeed
Word #2: dffd
Word #3: defed
Word #4: bbdd
Word #5: bdffeefd

String 1 true
String 2 true
String 3 true
String 4 true
String 5 true

Variant 3:
Word #1: bdbbbbbf
Word #2: bd
Word #3: ff
Word #4: bd
Word #5: afbbbbbbf

String 1 true
String 2 true
String 3 true
String 4 true
String 5 true

Variant 4:
Word #1: baceed
Word #2: ced
Word #3: de
Word #4: e
Word #5: aced

String 1 true
String 2 true
String 3 true
String 4 true
String 5 true

Variant 5:
Word #1: d
Word #2: aac
Word #3: bd
Word #4: bacaaaaac
Word #5: d

String 1 true
String 2 true
String 3 true
String 4 true
String 5 true

Variant 6:
Word #1: ce
Word #2: cfbccbnccfe
Word #3: ce
Word #4: ce
Word #5: cbccenm

String 1 true
String 2 true
String 3 true
String 4 true
String 5 true

Sadly couldnt upload photos but uploaded the output of the program execution.
for execution on own machine do the following commands:<br />
mkdir build<br />
cd src/build<br />
cmake ..<br />
make -j4<br />
./LFAF<br />

One thing to keep in mind it will only execute the first variant since it takes a bit of time
to execute all 32 variants due to me putting a sleep inside the code so that the randomized
seed isnt the same and all 5 generated strings are the same because  as the seed i choose time
which represent epoch timer.

In conclusion i can say that i have learned a bit more about gramars and Automatas and where and how
they are used and how they work and even how to program one to automate your work for you xD.

## References

Mr. Drumea's Laboratories<br />
Mrs. Cojuhari's Lectures
