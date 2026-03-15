#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

struct Node
{
    int key;
    Node *left;
    Node *right;

    Node(int k, Node* l = nullptr, Node* r = nullptr){
        this->key = k;
        this->left = l;
        this->right = r;
    }
};


Node* add_node(Node* node, int key){
    if(node == nullptr){
        return new Node(key);
    }

    if(key < node->key){
        node->left = add_node(node->left, key);
    } else {
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

void pre_ordem(Node* node){
    if(node == nullptr){
        return;
    }
    cout << node->key << " ";

    pre_ordem(node->left);
    pre_ordem(node->right);

}

void in_ordem(Node* node){
    if(node == nullptr){
        return;
    }

    in_ordem(node->left);
    cout << node->key << " ";
    in_ordem(node->right);
}

void pos_ordem(Node* node){
    if(node==nullptr){
        return;
    }   

    pos_ordem(node->left);
    pos_ordem(node->right);
    
    cout << node->key << " ";

}

/*
Exemplo de Entrada
2 qnt casos
3 qnt de na arvore
5 2 7 arvore
9 qnt na arvore
8 3 10 14 6 4 13 7 1 arvore

Exemplo de Saída
Case 1:
Pre.: 5 2 7
In..: 2 5 7
Post: 2 7 5

Case 2:
Pre.: 8 3 1 6 4 7 10 14 13
In..: 1 3 4 6 7 8 10 13 14
Post: 1 4 7 6 3 13 14 10 8
*/



int main(){

    int C;
    cin >> C;

    for(int i = 1; i <= C; i++){

        Node* root = nullptr;

        int N;
        cin >> N;

        for(int j = 0; j < N; j++){
            int x;
            cin >> x;
            root = add_node(root, x);
        }

        cout << "Case " << i << ":" << endl;

        cout << "Pre.: ";
        pre_ordem(root);
        cout << endl;

        cout << "In..: ";
        in_ordem(root);
        cout << endl;

        cout << "Post: ";
        pos_ordem(root);
        cout << endl << endl;
        
        free_tree(root);  // ← libera memória
    }
}
