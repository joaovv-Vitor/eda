#ifndef BST_HPP
#define BST_HPP
#include <string>
#include <sstream>

struct Node;

class BST {
public:
    BST();
    void insert(int v);
    void inorder();   // percurso em ordem simetrica
    void bshow();
    int sum_keys();
    int min_key();
    int total_internal_nodes();
    int um_filho();
    ~BST();
private:
    Node *_root;
    void _bshow(Node *node, std::string heranca);
    Node* _insert(Node* node, int k);
    Node *_clear(Node *root);
    void _inorder(Node *node);
    int _sum_keys(Node *node);
    int _min_key(Node *node);
    int _total_internal_nodes(Node *node);
    int _um_filho(Node *node);
};

#endif