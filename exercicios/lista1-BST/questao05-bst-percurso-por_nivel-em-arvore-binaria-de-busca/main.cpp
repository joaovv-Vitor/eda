#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <queue>

using namespace std;

struct Node
{
    int key;
    Node *left;
    Node *right;

    Node(int key, Node* l = nullptr, Node* r = nullptr){
        this->key = key,
        this->left = l;
        this->right = r;
    }
};
 
Node* add_node(Node* node, int key){
    if(node == nullptr){
        return new Node(key);
    }
    if(key< node->key){
        node->left = add_node(node->left, key);
    }
    else{
        node->right = add_node(node->right, key);
    }

    return node;
}

void free_tree(Node* node){
    if(node == nullptr){
        return;
    }
    free_tree(node->left);
    free_tree(node->right);

    delete node;
}

void percurso_bst(Node* node){
    if(node == nullptr){
        return;
    }

    queue<Node*> fila;

    fila.push(node);

    while(!fila.empty()){
        Node* n = fila.front();
        fila.pop();
        cout << n->key << " ";

        if(n->left){
            fila.push(n->left);
        }
        if(n->right){
            fila.push(n->right);
        }
    }
}


int main(){

    int c;
    cin >> c;


    for (int i = 0; i < c; i++)
    {
        
        Node* root = nullptr;

        int n;

        cin >> n;

        for (int j = 0; j < n; j++)
        {
            
            int x;
            cin >> x;
            root = add_node(root, x);
        }
        
        cout << "Case " << i+1 << ":" << endl;

        percurso_bst(root);
        cout << endl;
    }
}











