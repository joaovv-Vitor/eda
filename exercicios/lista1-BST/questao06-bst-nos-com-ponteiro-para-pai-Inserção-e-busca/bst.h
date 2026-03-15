#ifndef BST_H
#define BST_H
#include <iostream>
#include <stack>
#include <string>
using namespace std;

/***********************************
 * Definicao do struct Node
 ***********************************/
struct Node {
    int key;
    Node *left;
    Node *right;
    Node *parent;

    // Construtor
    Node(int k, Node* l = nullptr, Node* r = nullptr, Node* p = nullptr) {
        this->key = k;
        this->left = l;
        this->right = r;
        this->parent = p;
    }
    // Destrutor
    ~Node() {
        cout << "Node removed: " << this->key << endl;
    }
};

/***********************************
 * Declaracao da classe
 ***********************************/
class BST {
public:
    BST();
    BST(int k);
    void bshow();
    void add(int k); // ---> Implementar
    bool contains(int k); // ---> Implementar
    void inorderParent();
    ~BST();
private:
    Node *root;
    void bshow(Node *node, std::string heranca);
    Node *clear(Node *node); 
    Node *search(int k); // ---> Implementar
};

/************************************
 * Implementacao da classe
 ************************************/
// Constructor 1
BST::BST() {
    root = nullptr;
}

// Constructor 2
BST::BST(int k) {
    root = new Node(k);
}

void BST::bshow(){
    bshow(root, "");
}

void BST::bshow(Node *node, std::string heranca) {
    if(node != nullptr && (node->left != nullptr || node->right != nullptr))
        bshow(node->right , heranca + "r");
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
        bshow(node->left, heranca + "l");
}

Node *BST::clear(Node *node) {
    if(node != nullptr) {
        node->left = clear(node->left);
        node->right = clear(node->right);
        delete node;
    }
    return nullptr;
}

BST::~BST() {
    root = clear(root);
}

// Percorre os nos da arvore em ordem simetrica, imprimindo os pais
void BST::inorderParent() {
    stack<Node*> p; // Cria pilha do tipo Node* (ponteiro para Node)
    if(root != nullptr)
        p.push(root);
    while(!p.empty()) {
        Node *no = p.top();
        p.pop();
        if(no->parent != nullptr) cout << no->parent->key << " ";
        if(no->right != nullptr) p.push(no->right);
        if(no->left != nullptr) p.push(no->left);
    }
}

// Funcao publica 'add'
// Esta funcao deve obrigatoriamente ser iterativa.
// Esta funcao adiciona um no com chave k na arvore.
// Lembre-se que nao podem haver chaves repetidas na nossa arvore.
void BST::add(int k) {

    if(root == nullptr){
        root = new Node(k);
        return;
    }

    Node* atual = root;

    while(true){

        if(k < atual->key){

            if(atual->left == nullptr){
                Node* novo = new Node(k);
                novo->parent = atual;
                atual->left = novo;
                return;
            }

            atual = atual->left;
        }

        else if(k > atual->key){

            if(atual->right == nullptr){
                Node* novo = new Node(k);
                novo->parent = atual;
                atual->right = novo;
                return;
            }

            atual = atual->right;
        }

        else{
            return;
        }
    }
}

// Funcao publica 'contains'
// Esta funcao devolve 'true' se a arvore contem a chave k; 
// e devolve 'false' caso contrário.
bool BST::contains(int k) {
    Node* atual = root;

    while (atual !=nullptr)
    {
        if(k == atual->key){
            return true;
        }
        else if (k<atual->key){
            atual = atual->left;
        }
        else{
            atual = atual->right;
        }
    }
    return false;
}

#endif 