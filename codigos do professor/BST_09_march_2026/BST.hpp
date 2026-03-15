#ifndef BST_HPP
#define BST_HPP
#include <iostream>

struct Node {
    int key;
    Node *left;
    Node *right;

    Node(int k, Node *l, Node *r) {
        key = k;
        left = l;
        right = r;
    }

    ~Node() {
        std::cout << "deletado " << key << "\n";
    }
};

// Classe que implementa uma Binary Search Tree (BST)
class BST {
private:
    Node *m_root; // ponteiro para a raiz da árvore
public:
    // Construtor da árvore: cria árvore vazia
    BST() {
        m_root = nullptr;
    }

    // Função iterativa que recebe uma chave inteira e retorna true
    // se a chave estiver na árvore; ou false, caso contrário.
    bool contains(int key) {
        Node *aux = m_root;
        while(aux != nullptr && aux->key != key) {
            if(key < aux->key) 
                aux = aux->left;
            else 
                aux = aux->right;
        }
        return (aux != nullptr);
    }

    bool contains_recursive(int key) {
        return search(m_root, key) != nullptr;
    }

    void add_recursive(int key) {
        m_root = add(m_root, key);
    }


private:
    /**
     * @brief Função privada recursiva que busca uma chave na árvore
     */
    Node* search(Node *node, int x) {
        if(node == nullptr) 
            return nullptr;
        if(node->key == x) 
            return node;
        if(node->key < x) 
            return search(node->right, x);
        else 
            return search(node->left, x);
    }

    /**
     * @brief Função privada recursiva que adiciona uma chave na árvore
     */
    Node* add(Node *node, int x) {
        if(node == nullptr) {
            return new Node(x, nullptr, nullptr);
        }
        if(x < node->key)
            node->left = add(node->left, x);
        else if(x > node->key) 
            node->right = add(node->right, x);
        return node;
    }

};

#endif