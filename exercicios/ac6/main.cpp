#include <iostream>
#include <sstream>
#include "Btree.h"

using namespace std;

int main() {
    int degree, key;
    string outkeys, inkeys;

    cin >> degree;
    cin.ignore();

    Btree arv(degree);

    getline(cin, inkeys);
    
    getline(cin, outkeys);

    stringstream ss { inkeys };

    while(ss >> key) arv.insert(key);

    arv.printNodesByLevels();

    cout << "--------------" << endl;

    stringstream rr { outkeys };

    while(rr >> key) arv.remove(key);

    arv.printNodesByLevels();
    
    return 0;
}