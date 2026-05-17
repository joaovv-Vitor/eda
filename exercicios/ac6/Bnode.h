#ifndef BNODE_H
#define BNODE_H

struct Bnode {
    int n;          // numero atual de chaves
    bool leaf;      // true se e somente se esse node for uma folha
    int *key;       // vetor de chaves
    Bnode **child;  // vetor de ponteiros para os filhos do node

    // Construtor: cria um node vazio.
    // Recebe como entrada o grau minimo do node e um booleano
    // indicando se ele eh ou nao folha.
    Bnode(int degree, bool is_leaf) {
        leaf = is_leaf;                 
        n = 0; 
        key = new int[2*degree-1];
        child = new Bnode*[2*degree];
    }

    // Destrutor: quando um node for deletado,
    // tem que liberar os vetores alocados.
    ~Bnode() {
        delete[] key;
        delete[] child;
    }
};

#endif