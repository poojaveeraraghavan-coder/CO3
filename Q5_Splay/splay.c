
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100
#define ACCESS 10000

typedef struct Node {
    int key;
    struct Node *left, *right;
} Node;

Node *newNode(int key) {
    Node *n = malloc(sizeof(Node));
    n->key = key;
    n->left = n->right = NULL;
    return n;
}

Node *rightRotate(Node *x) {
    Node *y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

Node *leftRotate(Node *x) {
    Node *y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

Node *splay(Node *root, int key, int *visits) {
    if (root == NULL || root->key == key)
        return root;

    (*visits)++;

    if (key < root->key) {
        if (root->left == NULL)
            return root;

        if (key < root->left->key) {
            root->left->left =
                splay(root->left->left, key, visits);
            root = rightRotate(root);
        }
        else if (key > root->left->key) {
            root->left->right =
                splay(root->left->right, key, visits);

            if (root->left->right != NULL)
                root->left = leftRotate(root->left);
        }

        return root->left == NULL ? root : rightRotate(root);
    }
    else {
        if (root->right == NULL)
            return root;

        if (key > root->right->key) {
            root->right->right =
                splay(root->right->right, key, visits);
            root = leftRotate(root);
        }
        else if (key < root->right->key) {
            root->right->left =
                splay(root->right->left, key, visits);

            if (root->right->left != NULL)
                root->right = rightRotate(root->right);
        }

        return root->right == NULL ? root : leftRotate(root);
    }
}

Node *insert(Node *root, int key) {
    if (root == NULL)
        return newNode(key);

    int visits = 0;
    root = splay(root, key, &visits);

    if (root->key == key)
        return root;

    Node *n = newNode(key);

    if (key < root->key) {
        n->right = root;
        n->left = root->left;
        root->left = NULL;
    } else {
        n->left = root;
        n->right = root->right;
        root->right = NULL;
    }

    return n;
}

Node *search(Node *root, int key, int *visits) {
    *visits = 0;

    Node *current = root;
    while (current != NULL) {
        (*visits)++;

        if (key == current->key)
            break;
        else if (key < current->key)
            current = current->left;
        else
            current = current->right;
    }

    return splay(root, key, visits);
}

void freeTree(Node *root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

void runSimulation(int uniform) {
    Node *root = NULL;

    for (int i = 1; i <= N; i++)
        root = insert(root, i);

    long totalVisits = 0;
    int worst = 0;

    printf("\n%s ACCESS\n",
           uniform ? "UNIFORM" : "80/20 SKEWED");

    for (int i = 0; i < ACCESS; i++) {
        int key;

        if (uniform)
            key = rand() % N + 1;
        else if (rand() % 100 < 80)
            key = rand() % 5 + 1;
        else
            key = rand() % N + 1;

        int visits = 0;
        root = search(root, key, &visits);

        totalVisits += visits;
        if (visits > worst)
            worst = visits;

        if (i < 10)
            printf("Access %d: key %d, root = %d\n",
                   i + 1, key, root->key);
    }

    printf("Average nodes visited: %.2f\n",
           (double)totalVisits / ACCESS);
    printf("Worst nodes visited: %d\n", worst);

    freeTree(root);
}

int main() {
    srand(42);

    runSimulation(0);
    runSimulation(1);

    return 0;
}

