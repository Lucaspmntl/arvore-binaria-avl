#include <stdio.h>
#include <stdlib.h>


typedef struct Node{
    int key;
    struct Node *left;
    struct Node *right;
    int height; 
} Node;


int height(Node *N){
    if (N == NULL)
        return 0;
    return N->height;
}

int max(int a, int b){
    return (a > b) ? a : b;
}

Node *create_node(int key){
    Node *node = (Node *)malloc(sizeof(Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1; // Novo nó começa com altura 1
    return node;
}


Node *rightRotate(Node *y){
    Node *x = y->left;
    Node *T2 = x->right;

    // rotação
    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Node *leftRotate(Node *x){
    Node *y = x->right;
    Node *T2 = y->left;

    // rotação
    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}


int getBalance(Node *N){
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
    else {
        printf("Valor %d já existe. Inserção ignorada.\n", key);
        return node;
    }

    // atualiza a altura do ancestral
    node->height = 1 + max(height(node->left), height(node->right));

    // obtem o fator de balanceamento
    int balance = getBalance(node);

    // se desbalanceou, rotaciona


    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

Node *search_node(Node *root, int value) {
    if (!root)
        return NULL;
    if (value == root->key)
        return root;
    if (value < root->key)
        return search_node(root->left, value);
    return search_node(root->right, value);
}

Node *find_min(Node *root) {
    while (root->left)
        root = root->left;
    return root;
}

Node *remove_node(Node *root, int key) {

    if (root == NULL)
        return root;

    if (key < root->key)
        root->left = remove_node(root->left, key);
    else if (key > root->key)
        root->right = remove_node(root->right, key);
    else {
        // nó com apenas um filho ou nenhum
        if ((root->left == NULL) || (root->right == NULL)) {
            Node *temp = root->left ? root->left : root->right;

            // sem filhos
            if (temp == NULL) {
                temp = root;
                root = NULL;
            }

            else // com um filho
                *root = *temp;

            free(temp);
        }
        else {
            // com dois filhos
            Node *temp = find_min(root->right);
            root->key = temp->key;
            root->right = remove_node(root->right, temp->key);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);


    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}


void preorder(Node *root) {
    if (!root)
        return;
    printf("%d ", root->key);
    preorder(root->left);
    preorder(root->right);
}


void inorder(Node *root) {
    if (!root)
        return;
    inorder(root->left);
    printf("%d ", root->key);
    inorder(root->right);
}


void postorder(Node *root) {
    if (!root)
        return;
    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->key);
}


void free_tree(Node *root) {
    if (!root)
        return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}


void flush_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

int main(void) {
    Node *root = NULL;
    int option, value, sub;

    printf("======== ARVORE AVL ========\n");

    while (1) {
        printf("\n1-Inserir\n2-Buscar\n3-Remover\n4-Percorrer\n0-Sair\nEscolha: ");

        if (scanf("%d", &option) != 1) {
            flush_stdin();
            continue;
        }

        if (option == 0) {
            free_tree(root);
            printf("Memória liberada. Encerrando.\n");
            break;
        }

        switch (option) {

        case 1:
            printf("Valor para inserir: ");
            scanf("%d", &value);
            root = insert_node(root, value);
            // Dica visual: mostra a raiz atual para provar que mudou (rotacionou)
            printf("Inserido! Raiz atual da arvore: %d\n", root->key);
            break;

        case 2:
            printf("Valor para buscar: ");
            scanf("%d", &value);
            Node *found = search_node(root, value);
            if (found)
                printf("Encontrado: %d (Altura do no: %d)\n", found->key, found->height);
            else
                printf("Não encontrado.\n");
            break;

        case 3:
            printf("Valor para remover: ");
            scanf("%d", &value);
            root = remove_node(root, value);
            printf("Removido (se existia) e rebalanceado.\n");
            break;

        case 4:
            printf("1-Pre (Raiz-Esq-Dir)\n2-Em (Esq-Raiz-Dir)\n3-Pos (Esq-Dir-Raiz)\nEscolha: ");
            scanf("%d", &sub);
            printf("Saida: ");
            if (sub == 1)
                preorder(root);
            else if (sub == 2)
                inorder(root);
            else if (sub == 3)
                postorder(root);
            printf("\n");
            break;

        default:
            printf("Opção inválida.\n");
        }
    }
    return 0;
}