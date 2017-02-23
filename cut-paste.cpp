
#include <iostream>
using std::endl;
using std::cout;
using std::cin;

#include <string>
using std::string;

#include <sstream>

#include "splaytree.h"

/*
  This function does a cut-paste operation on the sequence represented by T.
  The cut portion is the subsequence from indices i to j (j is always >= i),
  and this subsequence is pasted at index k. The resulting sequence is stored
  in T.
*/
void cut_paste(SplayTree *T, int i, int j, int k) {
	// TBD:
    SplayTree* s1 = new SplayTree();
    T->split(j, s1);
    SplayTree* pasteSection = new SplayTree();
    T->split(i - 1, pasteSection);
    T->join(s1);
    SplayTree* s2 = new SplayTree();
    if (k <= i)
    {
        T->split(k, s2);
        T->join(pasteSection);
        T->join(s2);
    }
    else
    {
        int pasteLength = j - i + 1;
        T->split(k - pasteLength, s2);
        T->join(pasteSection);
        T->join(s2);
    }
    delete s1;
    delete pasteSection;
    delete s2;
}

int main() {
	
	string line;
	string word;
	SplayTree *T = new SplayTree();
	int num_words = 1;
	string newline = "\n";
	
	int n;			// Number of lines.
	cin >> n;
	getline(cin, line);
	
	for (int i = 0; i < n; i++) {
		
		getline(cin, line);
		std::istringstream iss(line);
		
		// while there are words in the line...
		while (iss >> word) {
			T->insert(word, num_words++);
            //cout << "Inserted word ";
            //cout << num_words;
            //cout << "\n";
		}
		T->insert("\n", num_words++);
        //cout << "Inserted word ";
        //cout << num_words;
        //cout << "\n";
		iss.clear();
	}

	int m;			// Number of queries.
	int i, j, k;	// Variables to hold each query.
	cin >> m;

	for (int x = 0; x < m; x++) {
		cin >> i >> j >> k;		
		cut_paste(T, i, j, k);
        //cout << "Finishing op ";
        //cout << x;
        //cout << "\n";
	}
	T->print();
	
	delete T;
	return 0;
}
