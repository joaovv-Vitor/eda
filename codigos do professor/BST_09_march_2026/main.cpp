#include <iostream>
#include "BST.hpp"

int main() {
    BST tree;
    
    srand(time(nullptr));

    for(int i = 1; i <= 100; i++) {
        tree.add_recursive(rand() % 100);
    }

    int valor = 98;

    if(tree.contains(valor)) {
        std::cout << "contém o " << valor << "\n";
    } else {
        std::cout << "não contém o " << valor << "\n";
    }

    return 0;
}
