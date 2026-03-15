// NAO MEXA NESTE ARQUIVO, JA ESTA PRONTO PARA USAR
#include <iostream>
#include <string>
#include <sstream>
#include "BST.hpp"
using namespace std;

int main()
{
    int key;
	string line;
    BST tree;

    getline(cin, line);
    stringstream ss (line);

    while(ss >> key) {
        tree.insert(key);
    }

    cout << tree.height() << " " << tree.size() << endl;

    return 0;
}