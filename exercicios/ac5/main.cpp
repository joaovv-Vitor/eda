#include <iostream>
#include <string>
#include "Set.h"

using namespace std;

// Utilitário para imprimir um conjunto na main
void printSet(const Set& s, const string& name) {
    vector<int> elems = s.toVector();
    cout << name << " = { ";
    for (size_t i = 0; i < elems.size(); ++i) {
        cout << elems[i] << (i < elems.size() - 1 ? ", " : "");
    }
    cout << " } (Tamanho: " << s.size() << ")\n";
}

int main() {
    vector<Set*> conjuntos;
    conjuntos.push_back(new Set()); 
    conjuntos.push_back(new Set()); 

    int opcao;
    do {
        cout << "\n--- MENU DE CONJUNTOS (ARVORE RUBRO-NEGRA) ---\n";
        cout << "Temos " << conjuntos.size() << " conjunto(s) em memoria.\n";
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

    for (Set* s : conjuntos) delete s;

    return 0;
}