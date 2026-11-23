
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node {
    int key, height;
    struct Node *left, *right;
} Node;

int rotations = 0;

int max(int a, int b) {
    return a > b ? a : b;
}

int height(Node *n) {
    return n ? n->height : 0;
}

int balance(Node *n) {
    return n ? height(n->left) - height(n->right) : 0;
}

Node* createNode(int key) {
    Node *n = malloc(sizeof(Node));
    if (!n) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    n->key = key;
    n->height = 1;
    n->left = n->right = NULL;
    return n;
}

Node* rotateRight(Node *y) {
    Node *x = y->left;
    Node *temp = x->right;

    x->right = y;
    y->left = temp;

    y->height = 1 + max(height(y->left), height(y->right));
    x->height = 1 + max(height(x->left), height(x->right));

    rotations++;
    return x;
}

Node* rotateLeft(Node *x) {
    Node *y = x->right;
    Node *temp = y->left;

    y->left = x;
    x->right = temp;

    x->height = 1 + max(height(x->left), height(x->right));
    y->height = 1 + max(height(y->left), height(y->right));

    rotations++;
    return y;
}

Node* insert(Node *root, int key) {
    if (root == NULL)
        return createNode(key);

    if (key < root->key)
        root->left = insert(root->left, key);
    else if (key > root->key)
        root->right = insert(root->right, key);
    else
        return root;

    root->height = 1 + max(height(root->left),
                           height(root->right));

    int bf = balance(root);

    // LL Case
    if (bf > 1 && key < root->left->key)
        return rotateRight(root);

    // RR Case
    if (bf < -1 && key > root->right->key)
        return rotateLeft(root);

    // LR Case
    if (bf > 1 && key > root->left->key) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    // RL Case
    if (bf < -1 && key < root->right->key) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

void preorder(Node *root) {
    if (root == NULL) return;

    printf("%d ", root->key);
    preorder(root->left);
    preorder(root->right);
}

int verifyAVL(Node *root) {
    if (root == NULL) return 1;

    int bf = balance(root);

    if (bf < -1 || bf > 1)
        return 0;

    return verifyAVL(root->left) &&
           verifyAVL(root->right);
}

Node* search(Node *root, int key) {
    while (root != NULL) {
        if (key == root->key)
            return root;
        else if (key < root->key)
            root = root->left;
        else
            root = root->right;
    }
    return NULL;
}

void freeTree(Node *root) {
    if (root == NULL) return;

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    int values[] = {10, 20, 30, 40, 50, 25};
    int n = sizeof(values) / sizeof(values[0]);

    Node *root = NULL;

    printf("AVL TREE - STEP BY STEP INSERTION\n");

    for (int i = 0; i < n; i++) {
        root = insert(root, values[i]);

        printf("\nAfter inserting %d: ", values[i]);
        preorder(root);
        printf("\nHeight: %d", height(root));
    }

    printf("\n\nTotal rotations: %d", rotations);

    printf("\nAVL Balance Verification: ");
    printf(verifyAVL(root) ? "PASSED\n" : "FAILED\n");

    if (search(root, 25) != NULL)
        printf("25 found in AVL tree\n");
    else
        printf("25 not found\n");

    freeTree(root);

    // Test 100,000 sorted keys
    root = NULL;
    rotations = 0;

    clock_t start = clock();

    for (int i = 1; i <= 100000; i++)
        root = insert(root, i);

    clock_t end = clock();

    double insertTime =
        (double)(end - start) / CLOCKS_PER_SEC;

    printf("\n\n100,000 SORTED KEY TEST\n");
    printf("Number of keys: 100000");
    printf("\nAVL Tree Height: %d", height(root));
    printf("\nTotal rotations: %d", rotations);
    printf("\nBalance verification: ");
    printf(verifyAVL(root) ? "PASSED" : "FAILED");

    printf("\nInsertion time: %.6f seconds", insertTime);

    start = clock();

    search(root, 99999);

    end = clock();

    double searchTime =
        (double)(end - start) / CLOCKS_PER_SEC;

    printf("\nSearch time: %.6f seconds", searchTime);

    printf("\nOrdinary BST height for sorted keys: 99999\n");

    freeTree(root);

    return 0;
}

