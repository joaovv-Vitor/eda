#include <iostream>
#include <stack>
#include <string>
#include <cctype>

using namespace std;

// Estrutura do nó da árvore binária
struct Node {
    char value;
    Node* left;
    Node* right;
    
    Node(char val) : value(val), left(nullptr), right(nullptr) {}
};

// Função auxiliar para definir a precedência dos operadores
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Função auxiliar que cria um nó de operador conectando dois nós filhos
void buildSubTree(stack<Node*>& nodes, stack<char>& ops) {
    Node* rightNode = nodes.top(); nodes.pop();
    Node* leftNode = nodes.top(); nodes.pop();
    char op = ops.top(); ops.pop();
    
    Node* newNode = new Node(op);
    newNode->left = leftNode;
    newNode->right = rightNode;
    
    nodes.push(newNode);
}

// Função principal para construir a árvore a partir da expressão
Node* buildExpressionTree(string expression) {
    stack<Node*> nodes;
    stack<char> ops;

    for (int i = 0; i < expression.length(); i++) {
        char ch = expression[i];

        // Ignora espaços em branco
        if (ch == ' ') continue;

        // Se for um operando (algarismo), cria um nó folha e empilha
        if (isdigit(ch)) {
            nodes.push(new Node(ch));
        } 
        // Se for parêntese de abertura, empilha nos operadores
        else if (ch == '(') {
            ops.push(ch);
        } 
        // Se for parêntese de fechamento, resolve tudo até achar o '('
        else if (ch == ')') {
            while (!ops.empty() && ops.top() != '(') {
                buildSubTree(nodes, ops);
            }
            if (!ops.empty()) ops.pop(); // Remove o '(' da pilha
        } 
        // Se for um operador (+, -, *, /)
        else {
            // Resolve operadores anteriores com maior ou igual precedência
            while (!ops.empty() && precedence(ops.top()) >= precedence(ch)) {
                buildSubTree(nodes, ops);
            }
            ops.push(ch);
        }
    }

    // Resolve quaisquer operadores restantes na pilha
    while (!ops.empty()) {
        buildSubTree(nodes, ops);
    }

    // O último nó restante na pilha é a raiz da árvore
    return nodes.empty() ? nullptr : nodes.top();
}

// Função para imprimir a árvore em ordem (Inorder) para testar
void printInorder(Node* root) {
    if (root) {
        if (!isdigit(root->value)) cout << "(";
        printInorder(root->left);
        cout << root->value;
        printInorder(root->right);
        if (!isdigit(root->value)) cout << ")";
    }
}

int main() {
    string expressao = "3+(4*5)-2";
    
    Node* raiz = buildExpressionTree(expressao);
    
    cout << "Expressao original: " << expressao << endl;
    cout << "Arvore reconstruida (Inorder com parenteses): ";
    printInorder(raiz);
    cout << endl;

    return 0;
}