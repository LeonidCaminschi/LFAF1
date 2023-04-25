# Laboratory 4 CNF (Chomsky Normal Form)

### Course: Formal Languages & Finite Automata
### Author: Caminschi Leonid FAF-211 (Leonidas)

----

## Theory
Short theory for obtaining chomsky normal form we have to go through 5 stages.
1) remove epsilon enclosures and replace them at a higher level with terminal characters.
2) removing NonTerminal - NonTerminal productions and generating their equivalent.
3) removing the innacesible symbols if there are any inaccesible symbols after the first 2 steps.
4) removing non productive productions which means removing any loop in our productions.
5) We modify the final productions according to rules:
    a) Are allowed Terminal -> NonTerminal.
    b) Are allowed Terminal -> Terminal + Terminal.
    c) in case of Terminal -> Terminal + NonTerminal we create a new production storing the nonterminal value.


## Objectives:

* Get accustomed to CNF and step by step transformation.
* Learn how to create a program to automatically transform into CNF.
* Understand how it is aplicable.

## Implementation description

On the implementation of the solution to this laboratory work i tried doing everything on paper and understand
everything thoroughly before trying to code the solution to this problem so that i have the basics in the head
before telling the computer to do it, it was harder than expected but in the end i think it ended up working well.
All was done.

* Code snippets from my files.

We have a centralized method which calls each step of CNF separately for simplicity sake.

```

void toCNF() {
        eliminateEpsilonProductions();
        eliminateRenaming();
        eliminateInaccessibleSymbols();
        eliminateNonproductiveSymbols();
        convertToChomskyNormalForm();
}

```

We eliminate each production with epsilon and then check for Terminal character eliminated 
so we update each other 
example S -> Ca , C -> ε
we remove C -> ε and change S -> Ca to S -> a.

In this particular snippet we add each rule we encounter with epsilon into a map.

```

            if (r.Right == "ε") {
                epsilonVars.insert(r.Left);
            }
            
```

And here we process each marked rule according to the example above.

```
                    for (const auto &c: it->Right) {
                        if (string(1, c) == var) {
                            size_t n = combinations.size();
                            for (size_t i = 0; i < n; i++) {
                                combinations.push_back(combinations[i]);
                                combinations.back().push_back(c);
                                combinations[i].push_back(' ');
                            }
                        } else {
                            for (auto &s: combinations) {
                                s.push_back(c);
                            }
                        }
                    }

```

We eliminate renaming and the replace with their equvalent parts
A -> B would lets assume be changed to A -> C , C -> B.

I itterate through all of the production searching for NonTerminal -> NonTerminal and
process through it as to eliminate all renamings.

```

        for (const auto &r: productions) {
            if (r.Right.size() == 1 && isupper(r.Right[0])) {
                replacements.emplace(r.Left, vector<vector<string>>{{r.Right}});
            } else {
                vector<vector<string>> right_parts;
                vector<string> current_part;
                for (char c: r.Right) {
                    if (isupper(c)) {
                        if (!current_part.empty()) {
                            right_parts.push_back(current_part);
                            current_part.clear();
                        }
                        current_part.push_back(string(1, c));
                    } else {
                        current_part.push_back(string(1, c));
                    }
                }
                if (!current_part.empty()) {
                    right_parts.push_back(current_part);
                }
                if (replacements.count(r.Left) == 0) {
                    replacements.emplace(r.Left, vector<vector<string>>{right_parts[0]});
                } else {
                    replacements[r.Left].push_back(right_parts[0]);
                }
                for (size_t i = 1; i < right_parts.size(); i++) {
                    replacements[r.Left].push_back(right_parts[i]);
                }
            }
        }

```

Now we eliminate innaccesible symbols example:
S -> A , A -> B , D -> B
in this case D -> B is going to get removed.

```

            for (const auto &r: productions) {
                if (find(reachable_symbols.begin(), reachable_symbols.end(), r.Left) != reachable_symbols.end()) {
                    for (char c: r.Right) {
                        if (isupper(c)) {
                            string var(1, c);
                            if (find(reachable_symbols.begin(), reachable_symbols.end(), var) ==
                                reachable_symbols.end()) {
                                reachable_symbols.push_back(var);
                                found_new_symbol = true;
                            }
                        }
                    }
                }
            }

```

now we eliminate non productive symbols
example S -> A , A -> S.

Search for rule loops and remove them while updating the old ones and repeat it 
until no more loops are found.

```

            for (const auto &r: productions) {
                if (find(productive_symbols.begin(), productive_symbols.end(), r.Left) != productive_symbols.end()) {
                    bool is_productive = true;
                    for (char c: r.Right) {
                        if (isupper(c)) {
                            string var(1, c);
                            if (find(productive_symbols.begin(), productive_symbols.end(), var) ==
                                productive_symbols.end()) {
                                is_productive = false;
                                break;
                            }
                        }
                    }
                    if (is_productive) {
                        string var = string() + r.Left;
                        if (find(productive_symbols.begin(), productive_symbols.end(), var) ==
                            productive_symbols.end()) {
                            productive_symbols.push_back(var);
                            found_new_symbol = true;
                        }
                    }
                }
            }
            
```

Now we conver the final result into Chomsky normal Form as explained in the theory
S -> Aa will get transformed into S -> AX1 , X1 -> a.

Iterate through rules and update them according to the rules above,
also checking if there are already rules in place when finding one more match.

```

        for (Rule &r: productions) {
            if (r.Right.size() > 2) {
                string newVar = "X" + to_string(index++);
                int length = r.Right.length();
                for (int i = 0; i < length - 2; i++) {
                    string newRuleVar = (i == 0) ? r.Left : newVar;
                    string newRuleRight = r.Right.substr(i, 2);
                    productions.push_back({newRuleVar, newRuleRight});
                    r.Right.replace(i, 2, newRuleVar);
                }
                productions.push_back({newVar, r.Right.substr(length - 2, 2)});
                r.Right.replace(length - 2, 2, newVar);
            }
        }

```

## Conclusions / Screenshots / Results

Result after executing the code:

```
A -> AX1
X1 -> a
A -> AX2
X2 -> b
A -> a
A -> b
B -> AX1
B -> a
B -> b
S -> b
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

Updated from last laboratory everything unused was commented in case of need to use them in
later laboratories otherwise everything was left as is.

In conclusion i can say that i have learned a lot about CNF and reinforced the knowledge
given to me at the course.

## References

*Mr. Drumea's Laboratories*<br />
*Mrs. Cojuhari's Lectures*

https://cyberzhg.github.io/toolbox/cfg2cnf
https://en.wikipedia.org/wiki/Chomsky_normal_form
https://www.youtube.com/watch?v=-SZkkMWHBvQ
