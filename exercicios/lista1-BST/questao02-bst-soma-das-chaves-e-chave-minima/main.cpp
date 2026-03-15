// NAO MEXA NESTE ARQUIVO, JA ESTA PRONTO PARA USAR
#include <iostream>
#include <string>
#include "BST.hpp"
using namespace std;

int main()
{
    int key;
	string line;
    BST bt;

    getline(cin, line);
    stringstream ss (line);

    while(ss >> key) {
        bt.insert(key);
    }

    cout << bt.min_key() << " " << bt.sum_keys() << endl;
    cout << bt.total_internal_nodes() << " " << bt.um_filho() << endl;
    
	return 0;
}