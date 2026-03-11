#include <iostream>
#include <string>
#include <vector>

#include "ACTrie.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    // args: <TEXT> <PATTERN(S)>
    if (argc < 3) { 
        cout << "Korrekte Aufruf: aufgabe6_main <QUERY> <PATTERN> (eines oder mehrere)\n";
        return 1;
    } else {
        vector<string> needles;
        for (int i = 2; i < argc; i++) {
            needles.push_back(argv[i]);
        }
        
        ACTrie trie(needles);
        trie.setQuery(argv[1]);
        
        vector<Hit> hits;
        cout << "Hits (position in query, pattern):\n";
        while (trie.next(hits)) {
            for (const Hit& hit : hits) {
                cout << "  pos " << hit.pos << ", " << needles[hit.index] << endl;
            }
            hits.clear(); // Trefferliste für den nächsten Durchlauf leeren
        }
    }
    return 0;
}
