
#include <stdio.h>
#include <stdlib.h>

#define RED 1
#define BLACK 0

typedef struct Node {
    int key, color;
    struct Node *left, *right, *parent;
} Node;

Node *NIL, *root;
int rotations = 0;

void initialize() {
    NIL = malloc(sizeof(Node));
    NIL->color = BLACK;
    NIL->left = NIL->right = NIL->parent = NIL;
    root = NIL;
}

Node *createNode(int key) {
    Node *n = malloc(sizeof(Node));
    n->key = key;
    n->color = RED;
    n->left = n->right = n->parent = NIL;
    return n;
}

void leftRotate(Node *x) {
    Node *y = x->right;
    x->right = y->left;

    if (y->left != NIL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == NIL)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
    rotations++;
}

void rightRotate(Node *y) {
    Node *x = y->left;
    y->left = x->right;

    if (x->right != NIL)
        x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == NIL)
        root = x;
    else if (y == y->parent->right)
        y->parent->right = x;
    else
        y->parent->left = x;

    x->right = y;
    y->parent = x;
    rotations++;
}

void fixInsert(Node *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node *uncle = z->parent->parent->right;

            if (uncle->color == RED) {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(z->parent->parent);
            }
        } else {
            Node *uncle = z->parent->parent->left;

            if (uncle->color == RED) {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

void insert(int key) {
    Node *z = createNode(key);
    Node *y = NIL;
    Node *x = root;

    while (x != NIL) {
        y = x;
        if (key < x->key)
            x = x->left;
        else if (key > x->key)
            x = x->right;
        else {
            free(z);
            return;
        }
    }

    z->parent = y;

    if (y == NIL)
        root = z;
    else if (key < y->key)
        y->left = z;
    else
        y->right = z;

    fixInsert(z);
}

void inorder(Node *n) {
    if (n == NIL) return;

    inorder(n->left);
    printf("%d(%s) ", n->key,
           n->color == RED ? "R" : "B");
    inorder(n->right);
}

Node *findMinimum(Node *n) {
    if (n == NIL) return NIL;

    while (n->left != NIL)
        n = n->left;

    return n;
}

int search(Node *n, int key) {
    while (n != NIL && n->key != key) {
        if (key < n->key)
            n = n->left;
        else
            n = n->right;
    }
    return n != NIL;
}

int checkProperties(Node *n) {
    if (n == NIL) return 1;

    if (n->color == RED &&
        (n->left->color == RED || n->right->color == RED))
        return -1;

    int leftHeight = checkProperties(n->left);
    int rightHeight = checkProperties(n->right);

    if (leftHeight == -1 || rightHeight == -1 ||
        leftHeight != rightHeight)
        return -1;

    return leftHeight + (n->color == BLACK);
}

int main() {
    int keys[] = {10, 20, 30, 15, 25, 5};
    int n = 6;

    initialize();

    printf("RED-BLACK TREE\n");

    for (int i = 0; i < n; i++) {
        insert(keys[i]);
        printf("After inserting %d: ", keys[i]);
        inorder(root);
        printf("\n");
    }

    printf("\nFinal Inorder Traversal: ");
    inorder(root);

    Node *smallest = findMinimum(root);
    if (smallest != NIL)
        printf("\nSmallest key: %d", smallest->key);

    printf("\nSearch for 25: %s",
           search(root, 25) ? "Found" : "Not Found");

    printf("\nSearch for 99: %s",
           search(root, 99) ? "Found" : "Not Found");

    printf("\nNumber of rotations: %d", rotations);

    printf("\nRed-Black properties: %s\n",
           root->color == BLACK && checkProperties(root) != -1
           ? "Verified" : "Not Verified");

    return 0;
}
