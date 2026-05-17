#ifndef BTREE_H
#define BTREE_H
#include <utility>
#include <stdexcept>
#include <iostream>
#include <queue>
#include "Bnode.h"

class Btree {
private:
    Bnode *m_root;  // ponteiro para a raiz
    int m_degree;   // grau minimo da arvore

    // Deleta todos os nodes nessa subarvore
    // enraizada em node
    void clear(Bnode *node) {
        if(node->leaf == true) {
            delete node;
        }
        else {
            for(int i = 0; i <= node->n; ++i) {
                clear(node->child[i]);
            }
            delete node;
        }
    }

    // Recebe como entrada um node e uma chave k
    std::pair<Bnode*,int> search(Bnode *x, int k) {
        int i = 0;
        while(i < x->n && k > x->key[i]) {
            i++;
        }
        if(i < x->n && k == x->key[i])
            return {x,i};
        else if(x->leaf) 
            return {nullptr,-1};
        else 
            return search(x->child[i],k);
    }

    // função que divide um nó cheio 
    void split_child(Bnode *x, int i) {
        // y eh o filho que sera dividido
        Bnode *y = x->child[i];

        // aloca memoria para um novo node que armazenara as 
        // d-1 maiores chaves de y
        Bnode *z = new Bnode(m_degree, y->leaf);
        z->n = m_degree - 1;

        // Copia as ultimas d-1 chaves de y para z
        for(int j = 0; j <= m_degree-2; ++j) {
            z->key[j] = y->key[m_degree + j];
        }

        // Copia os ultimos d filhos de y para z
        if(y->leaf == false) {
            for(int j = 0; j <= m_degree-1; ++j) {
                z->child[j] = y->child[m_degree + j];
            }
        }

        // Atualiza o numero de chaves de y
        y->n = m_degree-1;

        // Como este node tera um novo filho, criamos
        // espaco para o novo filho
        for(int j = x->n; j >= i+1; --j)
            x->child[j+1] = x->child[j];
        
        // Faz child[i+1] apontar para z
        x->child[i+1] = z;

        // Move todas as chaves maiores que key[i] uma posicao para a frente
        for(int j = x->n-1; j >= i; --j)
            x->key[j+1] = x->key[j];

        // Copia a chave mediana de y para este node
        x->key[i] = y->key[m_degree-1];

        // Incrementa o numero de chaves deste node
        x->n++;   
    }

    // Funcao que insere uma nova chave k na subarvore enraizada no node x.
    // O node x nao esta cheio quando esta funcao eh chamada.
    void insert_nonfull(Bnode *x, int k) {
        // variavel i eh o indice para a ultima chave
        int i = x->n - 1;
        // se o node x for uma folha
        if(x->leaf) {
            // move as chaves uma posicao para a direita ate encontrar
            // a posicao correta para a chave k.
            while(i >= 0 && k < x->key[i]) {
                x->key[i+1] = x->key[i];
                i = i - 1;
            }
            // insere a nova chave k na posicao correta
            x->key[i+1] = k;
            // incrementa a quantidade de chaves neste node
            x->n = x->n + 1;
        }
        // se este node nao for uma folha
        else {
            while(i >= 0 && k < x->key[i]) {
                i--;
            }
            i++;
            if(x->child[i]->n == 2*m_degree-1) {
                // divide o filho cheio
                split_child(x,i);
                // apos o antigo node child[i] ter sido dividido,
                // decidimos em qual das duas metades a chave k deve pertencer.
                if(k > x->key[i]) {
                    i++;
                }
            }
            insert_nonfull(x->child[i], k);
        }
    }

    /**
     * Funcao que concatena o filho x.c[i] e o filho x.c[i + 1]. 
     * A soma do numero de chaves de x.c[i] e x.c[i + 1] eh 
     * menor que 2d − 1 quando essa funcao eh chamada. 
     * Apos essa operacao ser realizada, 
     * o node x.c[i] fica como no maximo 2d − 1 chaves e o node x.c[i + 1] 
     * eh liberado (deleted). Apos essa operacao, a chave x.key[i] que 
     * separava x.c[i] e x.c[i + 1] no node x, deixa de existir em x e
     * passa a fazer parte do filho x.c[i]. Com isso, o pai x deve ter 
     * seus atributos tambem atualizados.
     */
    void concatenate(Bnode *x, int i) {
        Bnode *y = x->child[i];
        Bnode *z = x->child[i + 1];

        // 1. Move a chave separadora de x para o final de y
        y->key[y->n] = x->key[i];

        // 2. Move as chaves de z para y
        for (int j = 0; j < z->n; ++j) {
            y->key[y->n + 1 + j] = z->key[j];
        }

        // 3. Move os filhos de z para y, se y nao for folha
        if (!y->leaf) {
            for (int j = 0; j <= z->n; ++j) {
                y->child[y->n + 1 + j] = z->child[j];
            }
        }

        // 4. Atualiza a quantidade de chaves de y
        y->n += z->n + 1;

        // 5. Remove a chave separadora de x, movendo as outras para a esquerda
        for (int j = i + 1; j < x->n; ++j) {
            x->key[j - 1] = x->key[j];
        }

        // 6. Remove o ponteiro para o filho z de x, movendo os outros para a esquerda
        for (int j = i + 2; j <= x->n; ++j) {
            x->child[j - 1] = x->child[j];
        }

        // 7. Atualiza a quantidade de chaves de x
        x->n--;

        // 8. Libera a memoria do no z
        delete z;
    }

    /**
     * Funcao que redistribui as chaves do filho x.c[i-1] para o filho x.c[i] 
     * do node x. Atencao: esses dois filhos devem sao irmaos adjacentes. 
     * O filho x.c[i] tem menos do que d − 1 chaves e o filho x.c[i-1] tem mais
     * do que d − 1 chaves. Apos esta operacao, x.c[i] passa a ter 
     * exatamente uma chave a mais e o filho x.c[i-1] passa a 
     * ter uma chave a menos. Alem disso, a chave que separa estes 
     * dois irmaos adjacentes tambem eh modificada, porem, o pai 
     * fica com a mesma quantidade de chaves.
     */
    void borrowFromLeft(Bnode *x, int i) {
        Bnode *y = x->child[i];
        Bnode *z = x->child[i - 1];

        // Move as chaves de y uma posicao para a direita
        for (int j = y->n - 1; j >= 0; --j) {
            y->key[j + 1] = y->key[j];
        }

        // Move os filhos de y uma posicao para a direita, se nao for folha
        if (!y->leaf) {
            for (int j = y->n; j >= 0; --j) {
                y->child[j + 1] = y->child[j];
            }
        }

        // A primeira chave de y recebe a chave de x que os separa
        y->key[0] = x->key[i - 1];

        // Se nao for folha, o primeiro filho de y recebe o ultimo filho de z
        if (!y->leaf) {
            y->child[0] = z->child[z->n];
        }

        // A chave de x que os separa recebe a ultima chave de z
        x->key[i - 1] = z->key[z->n - 1];

        // Atualiza a quantidade de chaves de y e z
        y->n++;
        z->n--;
    }

    /**
     * Funcao que redistribui as chaves do filho x.c[i+1] para o filho x.c[i] 
     * do node x. Atencao: esses dois filhos devem sao irmaos adjacentes. 
     * O filho x.c[i] tem menos do que d − 1 chaves e o filho x.c[i+1] tem mais
     * do que d − 1 chaves. Apos esta operacao, x.c[i] passa a ter 
     * exatamente uma chave a mais e o filho x.c[i+1] passa a 
     * ter uma chave a menos. Alem disso, a chave que separa estes 
     * dois irmaos adjacentes tambem eh modificada, porem, o pai 
     * fica com a mesma quantidade de chaves.
     */
    void borrowFromRight(Bnode *x, int i) {
        Bnode *y = x->child[i];
        Bnode *z = x->child[i + 1];

        // A ultima chave de y recebe a chave de x que os separa
        y->key[y->n] = x->key[i];

        // Se nao for folha, o ultimo filho de y recebe o primeiro filho de z
        if (!y->leaf) {
            y->child[y->n + 1] = z->child[0];
        }

        // A chave de x que os separa recebe a primeira chave de z
        x->key[i] = z->key[0];

        // Move as chaves de z uma posicao para a esquerda
        for (int j = 1; j < z->n; ++j) {
            z->key[j - 1] = z->key[j];
        }

        // Move os filhos de z uma posicao para a esquerda, se nao for folha
        if (!z->leaf) {
            for (int j = 1; j <= z->n; ++j) {
                z->child[j - 1] = z->child[j];
            }
        }

        // Atualiza a quantidade de chaves de y e z
        y->n++;
        z->n--;
    }

    /**
     * Esta funcao recebe o node folha x e um indice i
     * e remove a chave no indice i 
     */
    void remove_from_leaf(Bnode *x, int i) {
        // Move as chaves posteriores uma posicao para a esquerda
        for (int j = i + 1; j < x->n; ++j) {
            x->key[j - 1] = x->key[j];
        }
        // Atualiza a quantidade de chaves
        x->n--;
    }

    /**
     * Esta funcao recursiva recebe como entrada um node x tal 
     * que x.key[i] eh a chave que queremos remover. Porem, quando 
     * esta funcao eh chamada pela primeira vez, sabemos que x nao eh um node folha. 
     * Logo, a chave x.key[i] possui um sucessor, que esta na subarvore 
     * com raiz y. Portanto, esta funcao encontrara a chave Z sucessora 
     * da chave x.key[i], copiara a chave Z em x.key[i] e, entao, removera 
     * a chave Z do node original ao qual ela pertencia. 
     * Esta funcao deve regular os nodes que foram visitados toda vez
     * que for necessario. 
     */
    void remove_sucessor(Bnode *x, int i, Bnode *y) {
        if (y->leaf) {
            // A chave sucessora eh a primeira chave do no folha
            x->key[i] = y->key[0];
            remove_from_leaf(y, 0);
        } else {
            remove_sucessor(x, i, y->child[0]);
            
            // Rebalanceamento bottom-up
            if (y->child[0]->n < m_degree - 1) {
                if (y->child[1]->n > m_degree - 1) {
                    borrowFromRight(y, 0);
                } else {
                    concatenate(y, 0);
                }
            }
        }
    }

    /** 
     * Recebe como argumento um ponteiro x para raiz de arvore-B e 
     * um valor de chave k e remove esta chave da arvore enraizada neste node 
     * se e somente se a chave estiver na arvore;
     * caso contrario, nao faz nada e a arvore permanece como estava.
     * Esta funcao deve ajustar os nodes que foram visitados toda 
     * vez que for necessario, pois a arvore deve permanecer uma arvore-B 
     * apos cada remocao. Ou seja, essa funcao deve fazer o 
     * rebalanceamento do no visitado.
    */
    void remove_key(Bnode *x, int k) {
        int i = 0;
        while (i < x->n && k > x->key[i]) {
            i++;
        }

        if (i < x->n && k == x->key[i]) {
            // A chave k esta neste no x
            if (x->leaf) {
                remove_from_leaf(x, i);
            } else {
                remove_sucessor(x, i, x->child[i + 1]);
                
                // Rebalanceamento bottom-up do filho que sofreu a remocao
                if (x->child[i + 1]->n < m_degree - 1) {
                    if (i + 1 > 0 && x->child[i]->n > m_degree - 1) {
                        borrowFromLeft(x, i + 1);
                    } else if (i + 2 <= x->n && x->child[i + 2]->n > m_degree - 1) {
                        borrowFromRight(x, i + 1);
                    } else {
                        if (i + 2 <= x->n) {
                            concatenate(x, i + 1);
                        } else {
                            concatenate(x, i);
                        }
                    }
                }
            }
        } else {
            // A chave k nao esta neste no x
            if (x->leaf) {
                return; // Chave nao encontrada na arvore
            }

            remove_key(x->child[i], k);

            // Rebalanceamento bottom-up do filho que sofreu a remocao
            if (x->child[i]->n < m_degree - 1) {
                if (i > 0 && x->child[i - 1]->n > m_degree - 1) {
                    borrowFromLeft(x, i);
                } else if (i < x->n && x->child[i + 1]->n > m_degree - 1) {
                    borrowFromRight(x, i);
                } else {
                    if (i < x->n) {
                        concatenate(x, i);
                    } else {
                        concatenate(x, i - 1);
                    }
                }
            }
        }
    }

    // imprime as chaves em ordem crescente (ordem simetrica)
    void print_keys(Bnode *node) {
        for(int i = 0; i < node->n; ++i) {
            // se node nao eh folha, entao antes de imprimir x->key[i],
            // percorra a subarvore enraizada em x->child[i]
            if(node->leaf == false) {
                print_keys(node->child[i]);
            }
            std::cout << node->key[i] << " ";
        }
        if(node->leaf == false) {
            print_keys(node->child[node->n]);
        }
    }

public:
    // Construtor: cria arvore vazia, recebe como entrada o grau minimo
    Btree(int d) {
        if(d < 2) {
            throw std::runtime_error("wrong degree");
        }
        m_degree = d;
        m_root = new Bnode(m_degree, true);
    }

    // Destrutor
    ~Btree() {
        clear(m_root);
    }

    // funcao publica: retorna true se e somente se
    // a chave k estiver na arvore
    bool contains(int k) {
        return search(m_root, k).first != nullptr;
    }

    // funcao publica: insere uma chave na arvore B
    void insert(int k) {
        // Caso 1: raiz cheia
        if(m_root->n == 2*m_degree-1) { 
            Bnode *newnode = new Bnode(m_degree, false);
            newnode->child[0] = m_root;
            m_root = newnode;
            split_child(m_root, 0);
            insert_nonfull(m_root, k);
        }
        // Caso 2: raiz nao cheia
        else {  
            insert_nonfull(m_root, k);
        }
    }

    // Imprime as chaves no terminal em ordem crescente
    void print_keys() {
        print_keys(m_root);
    }

    // remove a chave k se e somente se ela existir.
    // se nao existir, nao faz nada.
    void remove(int k) {
        remove_key(m_root, k);

        // se a raiz tiver ficado vazia, entao a arvore diminui de altura
        if(m_root->n < 1 && !m_root->leaf) {
            Bnode *aux = m_root;
            m_root = m_root->child[0];
            delete aux;
        }
    }

    // imprime os nodes da arvore por nivel
    // primeiro a raiz no nivel 1, depois os filhos da raiz no nivel 2,
    // e assim por diante...
    void printNodesByLevels() {
        std::queue<Bnode*> fila;
        fila.push(m_root);
        int levelEnded = 1;
        while(!fila.empty()) {
            if(levelEnded == 0) {
                levelEnded = fila.size();
                std::cout << "\n" << std::endl;
            }
            Bnode *node = fila.front();
            fila.pop();
            std::cout << "[";
            int i = 0;
            for(; i < node->n; i++) {
                std::cout << node->key[i];
                if(i < node->n-1) 
                    std::cout << ",";
                if(!node->leaf)
                    fila.push(node->child[i]);
            }
            if(!node->leaf)
                fila.push(node->child[i]);
            std::cout << "] ";
            levelEnded--; 
        }
        std::cout << std::endl;
    }
    
};

#endif