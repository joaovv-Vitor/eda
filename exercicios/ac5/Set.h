#ifndef SET_H
#define SET_H

#include <vector>
#include <stdexcept>

enum Color { RED, BLACK };

// Estrutura do Nó da Árvore Rubro-Negra
struct Node {
    int data;
    Color color;
    Node* left;
    Node* right;
    Node* parent;

    Node(int val);
};

class Set {
private:
    Node* root;
    Node* TNULL; // Nó sentinela para representar as folhas (NIL)
    size_t count;

    // Métodos utilitários privados
    void initTNULL();
    void leftRotate(Node* x);
    void rightRotate(Node* x);
    void insertFixup(Node* k);
    void transplant(Node* u, Node* v);
    void eraseFixup(Node* x);
    Node* searchTreeHelper(Node* node, int key) const;
    Node* minimumHelper(Node* node) const;
    Node* maximumHelper(Node* node) const;
    void destroyTree(Node* node);
    void inorderHelper(Node* node, std::vector<int>& elements) const;

public:
    Set();
    ~Set();

    void insert(int key);
    void erase(int data);
    bool contains(int key) const;
    void clear();
    void swap(Set& T);
    int minimum() const;
    int maximum() const;
    int successor(int key) const;
    int predecessor(int key) const;
    bool empty() const;
    size_t size() const;
    
    std::vector<int> toVector() const;
};

// Declaração das Operações Binárias
Set Union(const Set& S, const Set& R);
Set Intersection(const Set& S, const Set& R);
Set Difference(const Set& S, const Set& R);

#endif // SET_H