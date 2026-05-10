#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>

using namespace std;

enum Color { RED, BLACK };

// Estrutura do Nó da Árvore Rubro-Negra
struct Node {
    int data;
    Color color;
    Node* left;
    Node* right;
    Node* parent;

    Node(int val) : data(val), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class Set {
private:
    Node* root;
    Node* TNULL; // Nó sentinela para representar as folhas (NIL)
    size_t count;

    // Inicializa o nó sentinela
    void initTNULL() {
        TNULL = new Node(0);
        TNULL->color = BLACK;
        TNULL->left = nullptr;
        TNULL->right = nullptr;
    }

    // Rotação à esquerda
    void leftRotate(Node* x) {
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

    // Rotação à direita
    void rightRotate(Node* x) {
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

    // Balanceamento após inserção
    void insertFixup(Node* k) {
        Node* u;
        while (k->parent != nullptr && k->parent->color == RED) {
            if (k->parent == k->parent->parent->right) {
                u = k->parent->parent->left; // tio
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
                u = k->parent->parent->right; // tio
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

    // Auxiliar para transplante de nós durante a remoção
    void transplant(Node* u, Node* v) {
        if (u->parent == nullptr) root = v;
        else if (u == u->parent->left) u->parent->left = v;
        else u->parent->right = v;
        v->parent = u->parent;
    }

    // Balanceamento após remoção
    void eraseFixup(Node* x) {
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

    Node* searchTreeHelper(Node* node, int key) const {
        if (node == TNULL || key == node->data) return node;
        if (key < node->data) return searchTreeHelper(node->left, key);
        return searchTreeHelper(node->right, key);
    }

    Node* minimumHelper(Node* node) const {
        while (node->left != TNULL) node = node->left;
        return node;
    }

    Node* maximumHelper(Node* node) const {
        while (node->right != TNULL) node = node->right;
        return node;
    }

    void destroyTree(Node* node) {
        if (node != TNULL) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    void inorderHelper(Node* node, vector<int>& elements) const {
        if (node != TNULL) {
            inorderHelper(node->left, elements);
            elements.push_back(node->data);
            inorderHelper(node->right, elements);
        }
    }

public:
    // CONSTRUTOR
    Set() {
        initTNULL();
        root = TNULL;
        count = 0;
    }

    // DESTRUTOR
    ~Set() {
        clear();
        delete TNULL;
    }

    // INSERT(X)
    void insert(int key) {
        if (contains(key)) return; // Conjuntos não possuem duplicatas

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

    // ERASE(X)
    void erase(int data) {
        Node* z = TNULL;
        Node* x;
        Node* y;
        Node* node = this->root;

        while (node != TNULL) {
            if (node->data == data) z = node;
            if (node->data <= data) node = node->right;
            else node = node->left;
        }

        if (z == TNULL) return; // Não encontrado

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

    // CONTAINS(X)
    bool contains(int key) const {
        return searchTreeHelper(this->root, key) != TNULL;
    }

    // CLEAR()
    void clear() {
        destroyTree(root);
        root = TNULL;
        count = 0;
    }

    // SWAP(T) - Complexidade O(1)
    void swap(Set& T) {
        std::swap(root, T.root);
        std::swap(TNULL, T.TNULL);
        std::swap(count, T.count);
    }

    // MINIMUM()
    int minimum() const {
        if (empty()) throw std::runtime_error("O conjunto está vazio.");
        return minimumHelper(this->root)->data;
    }

    // MAXIMUM()
    int maximum() const {
        if (empty()) throw std::runtime_error("O conjunto está vazio.");
        return maximumHelper(this->root)->data;
    }

    // SUCCESSOR(X)
    int successor(int key) const {
        Node* x = searchTreeHelper(root, key);
        if (x == TNULL) throw std::invalid_argument("Elemento não está no conjunto.");
        
        if (x->right != TNULL) return minimumHelper(x->right)->data;

        Node* y = x->parent;
        while (y != nullptr && x == y->right) {
            x = y;
            y = y->parent;
        }
        if (y == nullptr) throw std::out_of_range("Elemento não possui sucessor.");
        return y->data;
    }

    // PREDECESSOR(X)
    int predecessor(int key) const {
        Node* x = searchTreeHelper(root, key);
        if (x == TNULL) throw std::invalid_argument("Elemento não está no conjunto.");
        
        if (x->left != TNULL) return maximumHelper(x->left)->data;

        Node* y = x->parent;
        while (y != nullptr && x == y->left) {
            x = y;
            y = y->parent;
        }
        if (y == nullptr) throw std::out_of_range("Elemento não possui predecessor.");
        return y->data;
    }

    // EMPTY()
    bool empty() const {
        return count == 0;
    }

    // SIZE()
    size_t size() const {
        return count;
    }

    // Método auxiliar para obter elementos (usado nas operações binárias)
    vector<int> toVector() const {
        vector<int> elements;
        inorderHelper(root, elements);
        return elements;
    }
};

// Operações Binárias (Fora da classe Set conforme o requisito 5)

Set Union(const Set& S, const Set& R) {
    Set result;
    vector<int> elemsS = S.toVector();
    vector<int> elemsR = R.toVector();
    
    for (int x : elemsS) result.insert(x);
    for (int x : elemsR) result.insert(x); // Duplicatas são ignoradas pelo insert
    
    return result;
}

Set Intersection(const Set& S, const Set& R) {
    Set result;
    vector<int> elemsS = S.toVector();
    
    for (int x : elemsS) {
        if (R.contains(x)) {
            result.insert(x);
        }
    }
    return result;
}

Set Difference(const Set& S, const Set& R) {
    Set result;
    vector<int> elemsS = S.toVector();
    
    for (int x : elemsS) {
        if (!R.contains(x)) {
            result.insert(x);
        }
    }
    return result;
}

// Utilitário para imprimir um conjunto
void printSet(const Set& s, const string& name) {
    vector<int> elems = s.toVector();
    cout << name << " = { ";
    for (size_t i = 0; i < elems.size(); ++i) {
        cout << elems[i] << (i < elems.size() - 1 ? ", " : "");
    }
    cout << " } (Tamanho: " << s.size() << ")\n";
}


// MAIN.CPP - Menu de Opções
int main() {
    vector<Set*> conjuntos;
    // Inicializar com dois conjuntos para facilitar o teste das operações binárias
    conjuntos.push_back(new Set()); // Índice 0
    conjuntos.push_back(new Set()); // Índice 1

    int opcao;
    do {
        cout << "\n--- MENU DE CONJUNTOS (ARVORE RUBRO-NEGRA) ---\n";
        cout << "Temos " << conjuntos.size() << " conjunto(s) em memoria (Indices 0 a " << conjuntos.size()-1 << ").\n";
        cout << "1. Criar novo conjunto vazio\n";
        cout << "2. Inserir elemento (INSERT)\n";
        cout << "3. Remover elemento (ERASE)\n";
        cout << "4. Verificar se contem (CONTAINS)\n";
        cout << "5. Obter Minimo e Maximo (MINIMUM / MAXIMUM)\n";
        cout << "6. Obter Sucessor e Predecessor\n";
        cout << "7. Limpar conjunto (CLEAR)\n";
        cout << "8. Fazer Swap entre dois conjuntos (SWAP)\n";
        cout << "9. Uniao (UNION)\n";
        cout << "10. Intersecao (INTERSECTION)\n";
        cout << "11. Diferenca (DIFFERENCE)\n";
        cout << "12. Imprimir todos os conjuntos\n";
        cout << "0. Sair\n";
        cout << "Escolha uma opcao: ";
        if (!(cin >> opcao)) break;

        try {
            if (opcao == 1) {
                conjuntos.push_back(new Set());
                cout << "Conjunto criado no indice " << conjuntos.size() - 1 << ".\n";
            } 
            else if (opcao >= 2 && opcao <= 7) {
                int id, val;
                cout << "Em qual conjunto operar? (0 a " << conjuntos.size() - 1 << "): ";
                cin >> id;
                if (id < 0 || id >= conjuntos.size()) { cout << "Indice invalido.\n"; continue; }

                if (opcao == 2) {
                    cout << "Valor a inserir: "; cin >> val;
                    conjuntos[id]->insert(val);
                } else if (opcao == 3) {
                    cout << "Valor a remover: "; cin >> val;
                    conjuntos[id]->erase(val);
                } else if (opcao == 4) {
                    cout << "Valor a buscar: "; cin >> val;
                    cout << (conjuntos[id]->contains(val) ? "Contem!" : "Nao contem.") << "\n";
                } else if (opcao == 5) {
                    cout << "Minimo: " << conjuntos[id]->minimum() << " | Maximo: " << conjuntos[id]->maximum() << "\n";
                } else if (opcao == 6) {
                    cout << "Valor base: "; cin >> val;
                    try { cout << "Sucessor: " << conjuntos[id]->successor(val) << "\n"; }
                    catch(const exception& e) { cout << "Erro Sucessor: " << e.what() << "\n"; }
                    try { cout << "Predecessor: " << conjuntos[id]->predecessor(val) << "\n"; }
                    catch(const exception& e) { cout << "Erro Predecessor: " << e.what() << "\n"; }
                } else if (opcao == 7) {
                    conjuntos[id]->clear();
                    cout << "Conjunto limpo.\n";
                }
            }
            else if (opcao == 8) {
                int id1, id2;
                cout << "Indices dos conjuntos para SWAP (ex: 0 1): "; cin >> id1 >> id2;
                if (id1 >= 0 && id1 < conjuntos.size() && id2 >= 0 && id2 < conjuntos.size()) {
                    conjuntos[id1]->swap(*conjuntos[id2]);
                    cout << "Swap realizado com complexidade O(1).\n";
                }
            }
            else if (opcao >= 9 && opcao <= 11) {
                int id1, id2;
                cout << "Indices dos conjuntos S e R (ex: 0 1): "; cin >> id1 >> id2;
                if (id1 >= 0 && id1 < conjuntos.size() && id2 >= 0 && id2 < conjuntos.size()) {
                    Set resultado;
                    if (opcao == 9) resultado = Union(*conjuntos[id1], *conjuntos[id2]);
                    else if (opcao == 10) resultado = Intersection(*conjuntos[id1], *conjuntos[id2]);
                    else if (opcao == 11) resultado = Difference(*conjuntos[id1], *conjuntos[id2]);
                    
                    printSet(resultado, "Resultado");
                }
            }
            else if (opcao == 12) {
                for (size_t i = 0; i < conjuntos.size(); i++) {
                    printSet(*conjuntos[i], "Conjunto[" + to_string(i) + "]");
                }
            }
        } catch(const exception& e) {
            cout << "Excecao capturada: " << e.what() << "\n";
        }

    } while (opcao != 0);

    // Limpeza de memória
    for (Set* s : conjuntos) delete s;

    return 0;
}