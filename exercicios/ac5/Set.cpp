#include "Set.h"
#include <iostream>
#include <algorithm>

using namespace std;

// Implementação do Node
Node::Node(int val) : data(val), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}

// Implementação da Classe Set

void Set::initTNULL() {
    TNULL = new Node(0);
    TNULL->color = BLACK;
    TNULL->left = nullptr;
    TNULL->right = nullptr;
}

void Set::leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left != TNULL) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr) root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void Set::rightRotate(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if (y->right != TNULL) y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == nullptr) root = y;
    else if (x == x->parent->right) x->parent->right = y;
    else x->parent->left = y;
    y->right = x;
    x->parent = y;
}

void Set::insertFixup(Node* k) {
    Node* u;
    while (k->parent != nullptr && k->parent->color == RED) {
        if (k->parent == k->parent->parent->right) {
            u = k->parent->parent->left;
            if (u->color == RED) {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->left) {
                    k = k->parent;
                    rightRotate(k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                leftRotate(k->parent->parent);
            }
        } else {
            u = k->parent->parent->right;
            if (u->color == RED) {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->right) {
                    k = k->parent;
                    leftRotate(k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                rightRotate(k->parent->parent);
            }
        }
        if (k == root) break;
    }
    root->color = BLACK;
}

void Set::transplant(Node* u, Node* v) {
    if (u->parent == nullptr) root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;
    v->parent = u->parent;
}

void Set::eraseFixup(Node* x) {
    Node* s;
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            s = x->parent->right;
            if (s->color == RED) {
                s->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                s = x->parent->right;
            }
            if (s->left->color == BLACK && s->right->color == BLACK) {
                s->color = RED;
                x = x->parent;
            } else {
                if (s->right->color == BLACK) {
                    s->left->color = BLACK;
                    s->color = RED;
                    rightRotate(s);
                    s = x->parent->right;
                }
                s->color = x->parent->color;
                x->parent->color = BLACK;
                s->right->color = BLACK;
                leftRotate(x->parent);
                x = root;
            }
        } else {
            s = x->parent->left;
            if (s->color == RED) {
                s->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                s = x->parent->left;
            }
            if (s->right->color == BLACK && s->right->color == BLACK) {
                s->color = RED;
                x = x->parent;
            } else {
                if (s->left->color == BLACK) {
                    s->right->color = BLACK;
                    s->color = RED;
                    leftRotate(s);
                    s = x->parent->left;
                }
                s->color = x->parent->color;
                x->parent->color = BLACK;
                s->left->color = BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

Node* Set::searchTreeHelper(Node* node, int key) const {
    if (node == TNULL || key == node->data) return node;
    if (key < node->data) return searchTreeHelper(node->left, key);
    return searchTreeHelper(node->right, key);
}

Node* Set::minimumHelper(Node* node) const {
    while (node->left != TNULL) node = node->left;
    return node;
}

Node* Set::maximumHelper(Node* node) const {
    while (node->right != TNULL) node = node->right;
    return node;
}

void Set::destroyTree(Node* node) {
    if (node != TNULL) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

void Set::inorderHelper(Node* node, vector<int>& elements) const {
    if (node != TNULL) {
        inorderHelper(node->left, elements);
        elements.push_back(node->data);
        inorderHelper(node->right, elements);
    }
}

Set::Set() {
    initTNULL();
    root = TNULL;
    count = 0;
}

Set::~Set() {
    clear();
    delete TNULL;
}

void Set::insert(int key) {
    if (contains(key)) return;

    Node* node = new Node(key);
    node->parent = nullptr;
    node->data = key;
    node->left = TNULL;
    node->right = TNULL;
    node->color = RED;

    Node* y = nullptr;
    Node* x = this->root;

    while (x != TNULL) {
        y = x;
        if (node->data < x->data) x = x->left;
        else x = x->right;
    }

    node->parent = y;
    if (y == nullptr) root = node;
    else if (node->data < y->data) y->left = node;
    else y->right = node;

    if (node->parent == nullptr) {
        node->color = BLACK;
        count++;
        return;
    }
    if (node->parent->parent == nullptr) {
        count++;
        return;
    }

    insertFixup(node);
    count++;
}

void Set::erase(int data) {
    Node* z = TNULL;
    Node* x;
    Node* y;
    Node* node = this->root;

    while (node != TNULL) {
        if (node->data == data) z = node;
        if (node->data <= data) node = node->right;
        else node = node->left;
    }

    if (z == TNULL) return;

    y = z;
    int y_original_color = y->color;
    if (z->left == TNULL) {
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == TNULL) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = minimumHelper(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    delete z;
    count--;
    if (y_original_color == BLACK) eraseFixup(x);
}

bool Set::contains(int key) const {
    return searchTreeHelper(this->root, key) != TNULL;
}

void Set::clear() {
    destroyTree(root);
    root = TNULL;
    count = 0;
}

void Set::swap(Set& T) {
    std::swap(root, T.root);
    std::swap(TNULL, T.TNULL);
    std::swap(count, T.count);
}

int Set::minimum() const {
    if (empty()) throw std::runtime_error("O conjunto esta vazio.");
    return minimumHelper(this->root)->data;
}

int Set::maximum() const {
    if (empty()) throw std::runtime_error("O conjunto esta vazio.");
    return maximumHelper(this->root)->data;
}

int Set::successor(int key) const {
    Node* x = searchTreeHelper(root, key);
    if (x == TNULL) throw std::invalid_argument("Elemento nao esta no conjunto.");
    
    if (x->right != TNULL) return minimumHelper(x->right)->data;

    Node* y = x->parent;
    while (y != nullptr && x == y->right) {
        x = y;
        y = y->parent;
    }
    if (y == nullptr) throw std::out_of_range("Elemento nao possui sucessor.");
    return y->data;
}

int Set::predecessor(int key) const {
    Node* x = searchTreeHelper(root, key);
    if (x == TNULL) throw std::invalid_argument("Elemento nao esta no conjunto.");
    
    if (x->left != TNULL) return maximumHelper(x->left)->data;

    Node* y = x->parent;
    while (y != nullptr && x == y->left) {
        x = y;
        y = y->parent;
    }
    if (y == nullptr) throw std::out_of_range("Elemento nao possui predecessor.");
    return y->data;
}

bool Set::empty() const { return count == 0; }

size_t Set::size() const { return count; }

vector<int> Set::toVector() const {
    vector<int> elements;
    inorderHelper(root, elements);
    return elements;
}

// Implementação das Operações Binárias Livres
Set Union(const Set& S, const Set& R) {
    Set result;
    vector<int> elemsS = S.toVector();
    vector<int> elemsR = R.toVector();
    
    for (int x : elemsS) result.insert(x);
    for (int x : elemsR) result.insert(x);
    
    return result;
}

Set Intersection(const Set& S, const Set& R) {
    Set result;
    vector<int> elemsS = S.toVector();
    
    for (int x : elemsS) {
        if (R.contains(x)) result.insert(x);
    }
    return result;
}

Set Difference(const Set& S, const Set& R) {
    Set result;
    vector<int> elemsS = S.toVector();
    
    for (int x : elemsS) {
        if (!R.contains(x)) result.insert(x);
    }
    return result;
}