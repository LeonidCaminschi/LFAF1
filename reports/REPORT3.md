# The title of the work

### Course: Formal Languages & Finite Automata
### Author: Caminschi Leonid FAF-211 (Leonidas)

----

## Theory
Short theory from generating a grammar from an FA is easy since we have done the other way around at the
first report it involved opening up δ expressions and make them regular grammar expressions
About automatas there are 2 types encountered in this laboratory NFA and DFA, NFA is a type of 
automata with just one path / decision between states as DFA could have more than 1 state change at 1 time.


## Objectives:

* Get accustomed to Grammar and Finite Automatas
* Learn how to transform from one to another
* Understand how it is aplicable to language creation
* Learn differences of NFA and DFA and their applications

## Implementation description

On the implementation of the solution to this laboratory work i tried doing everything on paper and understand
everything thoroughly before trying to code the solution to this problem so that i have the basics in the head
before telling the computer to do it, it was harder than expected but in the end i think it ended up working well.

* Code snippets from your files.

```

//for classification of type 3 grammar
if (prod.Right.length() > 1 || !isupper(prod.Left[0]) || !islower(prod.Right[0])) {
  return false;
}

```

```

//for classification of type 2 grammar 
if (prod.Right.empty() || !isupper(prod.Left[0])) {
  return false;
}

```

```

//for classification of type 1 grammar
if (prod.Right.length() < 2 || !isupper(prod.Left[0]) || prod.Right[0] == prod.Left[0]) {
  return false;
}
bool foundNonterminal = false;
for (char c : prod.Right) {
  if (isupper(c)) {
    if (foundNonterminal) {
      return false;
    }
  foundNonterminal = true;
} else {
    if (!foundNonterminal) {
      return false;
    }
 }

```

self made function to check the grammars production rules and decide what type of grammer it is 
it returns a string which needs to pe outputed to see the information for my grammar the result was
also if none of the 3 types coincide with the given grammar we automaticaly deduce that it is a type 0 grammar.

```

static Grammar AutomataToGrammar(const Automata& automata) {
  Grammar grammar;
  grammar.nonTerminalVariables = automata.states;
  grammar.terminalVariables = automata.symbols;
  grammar.startingCharacter = automata.start_state;

  for (const auto& [key, value] : automata.transitions) {
    string lhs = key.first;
    string rhs = value;
    grammar.productions.push_back({lhs, rhs});
  }

  return grammar;
}

```

self made function to convert an automata to a grammar having the GrammarToAutomata it was eaey to
do the same thing but the other way around, removing δ and shifting 1 caracter to the right to do the
production rules and everything else was just assigning of variables since they are named differently
but they have the same data

```

bool isDeterministic() const {
  std::set<std::pair<std::string, std::string>> visited;
  for (const auto& [key, value] : transitions) {
    std::string state = key.first;
    std::string symbol = key.second;
    if (visited.count({state, symbol}))
      return false;
    visited.insert({state, symbol});
  }
  return true;
}

```

function to check if the said automata is deterministic or not returns true if it is deterministic and 
false otherwise. the logic behind it being if we visited the node with the state and symbol already
it is NonDeterministic FA but if we never visited the same state and symbol this means it is a deterministic
FA

```

void convertNFAToDFA() {
        std::map<std::pair<std::string, std::string>, std::string> dfaTransitions;
        std::set<std::string> unprocessedDFAStates;
        std::map<std::string, std::set<std::string>> nfaStateSubsets;
        std::set<std::string> startNFAStateSubset = epsilonClosure({start_state});
        std::string startDFAState = stateSetToString(startNFAStateSubset);
        nfaStateSubsets[startDFAState] = startNFAStateSubset;
        unprocessedDFAStates.insert(startDFAState);

        while (!unprocessedDFAStates.empty()) {
            std::string dfaState = *unprocessedDFAStates.begin();
            unprocessedDFAStates.erase(dfaState);

            for (const auto& symbol : symbols) {
                std::set<std::string> nfaStateSubset = move(nfaStateSubsets[dfaState]);
                nfaStateSubset = epsilonClosure(nfaStateSubset);

                std::string nfaStateSubsetString = stateSetToString(nfaStateSubset);
                if (nfaStateSubsets.count(nfaStateSubsetString) == 0) {
                    nfaStateSubsets[nfaStateSubsetString] = nfaStateSubset;
                    unprocessedDFAStates.insert(nfaStateSubsetString);
                }

                std::string nextDFAState = nfaStateSubsetString;
                dfaTransitions[make_pair(dfaState, symbol)] = nextDFAState;
            }
        }

        transitions = dfaTransitions;

        states.clear();
        final_states.clear();
        for (const auto& [dfaState, nfaStateSubset] : nfaStateSubsets) {
            states.push_back(dfaState);
            if (hasIntersection(nfaStateSubset, std::set(final_states.begin(), final_states.end()))) {
                final_states.push_back(dfaState);
            }
        }
    }

```

function to convert an NFA to DFA here were used helper functions such as:<br />
* epsilonClosure
* stateSetToString
* hasIntersection
which to be fair i did not do myself fully, rather reinterpreted other people's ideas
after which after calculating NFA's transitions and states we just swap them with his
current ones to change it from being a NFA to a DFA.


## Conclusions / Screenshots / Results

"Type 2" also known as Context-Free was the output of the clasify method from within the grammar 

"This automata is: Deterministic" output of the isDeterministic() function

Sadly couldnt upload photos but uploaded the output of the program execution.
for execution on own machine do the following commands:<br />
cd src<br />
mkdir build<br />
cd build<br />
cmake ..<br />
make<br />
./LFAF<br />

Updated from previouse laboratory work so now the work speed is increased significantly without any 
drawbacks and also made code cleaner and a lot easier to read but at times it can still be messy.

In conclusion i can say that i have learned a lot about Finite Automatas and their properties ant how
to change a NFA to DFA and a FA to Grammar while understanding the problems of each type of FA.

## References

Mr. Drumea's Laboratories<br />
Mrs. Cojuhari's Lectures
