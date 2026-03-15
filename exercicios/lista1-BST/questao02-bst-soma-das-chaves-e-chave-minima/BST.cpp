#include <iostream>
#include <sstream>
#include <string>
#include <climits>
#include "BST.hpp"

struct Node {
    int key;
    Node *left;
    Node *right;

    Node(int k, Node *l = nullptr, Node *r = nullptr) {
        this->key = k;
        this->left = l;
        this->right = r;
    }
};


BST::BST() {
    _root = nullptr;
}

void BST::insert(int v) {
    _root = _insert(_root, v);
}

Node* BST::_insert(Node* node, int k) {
    if (node == nullptr){
        return new Node(k, nullptr, nullptr);
    } else if (k < node->key){
        node->left = _insert(node->left, k);
    } else if (k > node->key){
        node->right = _insert(node->right, k);
    }
    return node;
}

BST::~BST() {
    _root = _clear(_root);
}

Node *BST::_clear(Node *node) {
    if(node != nullptr) { // caso geral: vamos liberar essa arvore
        node->left = _clear(node->left);
        node->right = _clear(node->right);
        delete node;
    }
    return nullptr;
}

void BST::inorder() {
    _inorder(_root);
}

void BST::_inorder(Node *node) {
    if(node != nullptr) { // Caso Geral
        _inorder(node->left);
        std::cout << node->key << " ";
        _inorder(node->right);  
    }
}

void BST::bshow(){
    _bshow(_root, "");
}

void BST::_bshow(Node *node, std::string heranca) {
    if(node != nullptr && (node->left != nullptr || node->right != nullptr))
        _bshow(node->right , heranca + "r");
    for(int i = 0; i < (int) heranca.size() - 1; i++)
        std::cout << (heranca[i] != heranca[i + 1] ? "│   " : "    ");
    if(heranca != "")
        std::cout << (heranca.back() == 'r' ? "┌───" : "└───");
    if(node == nullptr){
        std::cout << "#" << std::endl;
        return;
    }
    std::cout << node->key << std::endl;
    if(node != nullptr && (node->left != nullptr || node->right != nullptr))
        _bshow(node->left, heranca + "l");
}


int BST::sum_keys() {
    return _sum_keys(_root);
}

int BST::_sum_keys(Node *node) {

    if (node == nullptr) {
        return 0;
    }
    else {
        return node->key + _sum_keys(node->left) + _sum_keys(node->right);
    }
}

// Para fazer essa funcao, suponha que as arvores dos testes nunca serao vazias,
// assim, sempre havera um menor valor de chave a ser retornado
int BST::min_key() { // TODO
    return _min_key(_root);
}

// Supoe que o ponteiro recebido sempre eh diferente de nullptr
int BST::_min_key(Node *node) { // TODO
    if(node->left == nullptr){
        return node->key;
    }
    return _min_key(node->left);
}

int BST::total_internal_nodes() { // TODO
    return _total_internal_nodes(_root);
}
    
int BST::_total_internal_nodes(Node *node) { // TODO
    if(node == nullptr){
        return 0;
    }
    if(node->left == nullptr && node->right == nullptr){
        return 0;
    }
    return 1 + _total_internal_nodes(node->left) + _total_internal_nodes(node->right); 
}

int BST::um_filho() { // TODO
    return _um_filho(_root);
}

int BST::_um_filho(Node *node) {

    if (node == nullptr)
        return 0;

    bool umFilho = (node->left == nullptr) != (node->right == nullptr);

    return umFilho + _um_filho(node->left) + _um_filho(node->right);
}


