
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    struct Node *left, *right;
} Node;

Node* createNode(int key) {
    Node *n = malloc(sizeof(Node));
    if (n == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    n->key = key;
    n->left = n->right = NULL;
    return n;
}

Node* insert(Node *root, int key) {
    if (root == NULL)
        return createNode(key);

    if (key < root->key)
        root->left = insert(root->left, key);
    else if (key > root->key)
        root->right = insert(root->right, key);

    return root;
}

Node* search(Node *root, int key) {
    if (root == NULL || root->key == key)
        return root;

    if (key < root->key)
        return search(root->left, key);

    return search(root->right, key);
}

void inorder(Node *root) {
    if (root == NULL) return;
    inorder(root->left);
    printf("%d ", root->key);
    inorder(root->right);
}

void preorder(Node *root) {
    if (root == NULL) return;
    printf("%d ", root->key);
    preorder(root->left);
    preorder(root->right);
}

void postorder(Node *root) {
    if (root == NULL) return;
    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->key);
}

int height(Node *root) {
    if (root == NULL) return -1;

    int leftHeight = height(root->left);
    int rightHeight = height(root->right);

    return 1 + (leftHeight > rightHeight ?
                leftHeight : rightHeight);
}

Node* minimum(Node *root) {
    while (root && root->left != NULL)
        root = root->left;
    return root;
}

Node* deleteNode(Node *root, int key) {
    if (root == NULL) return NULL;

    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        if (root->left == NULL) {
            Node *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            Node *temp = root->left;
            free(root);
            return temp;
        }

        Node *temp = minimum(root->right);
        root->key = temp->key;
        root->right = deleteNode(root->right, temp->key);
    }

    return root;
}

void freeTree(Node *root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    Node *root = NULL;
    int values[] = {50, 30, 70, 20, 40, 60, 80};

    int n = sizeof(values) / sizeof(values[0]);

    for (int i = 0; i < n; i++)
        root = insert(root, values[i]);

    printf("Binary Search Tree\n");

    printf("\nInorder Traversal: ");
    inorder(root);

    printf("\nPreorder Traversal: ");
    preorder(root);

    printf("\nPostorder Traversal: ");
    postorder(root);

    printf("\nHeight of Tree: %d", height(root));

    int key = 40;

    if (search(root, key) != NULL)
        printf("\n%d found in the tree", key);
    else
        printf("\n%d not found in the tree", key);

    root = deleteNode(root, 30);

    printf("\n\nAfter deleting 30: ");
    inorder(root);

    printf("\n");
    freeTree(root);

    return 0;
}
