#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
    int height; // Altura do nó para cálculo de equilíbrio
} Node;

int height(Node *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

Node *create_node(int key) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1; // Novo nó é sempre folha, altura 1
    return node;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

// criação do nó (inicializa altura como 1)
Node *create_node(int key) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1; // novo nó é sempre folha, altura 1
    return node;
}


// rotação à Direita (quando o peso está na esquerda)
Node *rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    // rotação
    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// rotação à Esquerda (quando o peso está na direita)
Node *leftRotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;

    //rotação
    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(Node *N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

Node *insert_node(Node *node, int key) {

    if (node == NULL)
        return create_node(key);

    if (key < node->key)
        node->left = insert_node(node->left, key);
    else if (key > node->key)
        node->right = insert_node(node->right, key);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);


    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left); // rotação dupla
        return rightRotate(node);
    }

    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right); // rotação dupla
        return leftRotate(node);
    }

    return node;
}

// função para imprimir em pré-ordem (pra ver a raiz mudando)
void preorder(Node *root) {
    if (root != NULL) {
        printf("%d ", root->key);
        preorder(root->left);
        preorder(root->right);
    }
}

