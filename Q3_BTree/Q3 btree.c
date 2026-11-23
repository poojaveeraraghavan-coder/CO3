
#include <stdio.h>
#include <stdlib.h>

#define T 2

typedef struct BTreeNode {
    int keys[2 * T - 1];
    struct BTreeNode *child[2 * T];
    int n;
    int leaf;
} BTreeNode;

BTreeNode* createNode(int leaf) {
    BTreeNode *node = malloc(sizeof(BTreeNode));

    if (node == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    node->n = 0;
    node->leaf = leaf;

    for (int i = 0; i < 2 * T; i++)
        node->child[i] = NULL;

    return node;
}

void traverse(BTreeNode *root) {
    if (root == NULL) return;

    int i;

    for (i = 0; i < root->n; i++) {
        if (!root->leaf)
            traverse(root->child[i]);

        printf("%d ", root->keys[i]);
    }

    if (!root->leaf)
        traverse(root->child[i]);
}

BTreeNode* search(BTreeNode *root, int key) {
    if (root == NULL)
        return NULL;

    int i = 0;

    while (i < root->n && key > root->keys[i])
        i++;

    if (i < root->n && key == root->keys[i])
        return root;

    if (root->leaf)
        return NULL;

    return search(root->child[i], key);
}

void splitChild(BTreeNode *parent, int i,
                BTreeNode *fullChild) {

    BTreeNode *newChild = createNode(fullChild->leaf);
    newChild->n = T - 1;

    for (int j = 0; j < T - 1; j++)
        newChild->keys[j] = fullChild->keys[j + T];

    if (!fullChild->leaf) {
        for (int j = 0; j < T; j++)
            newChild->child[j] = fullChild->child[j + T];
    }

    fullChild->n = T - 1;

    for (int j = parent->n; j >= i + 1; j--)
        parent->child[j + 1] = parent->child[j];

    parent->child[i + 1] = newChild;

    for (int j = parent->n - 1; j >= i; j--)
        parent->keys[j + 1] = parent->keys[j];

    parent->keys[i] = fullChild->keys[T - 1];
    parent->n++;
}

void insertNonFull(BTreeNode *node, int key) {
    int i = node->n - 1;

    if (node->leaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }

        node->keys[i + 1] = key;
        node->n++;
    }
    else {
        while (i >= 0 && key < node->keys[i])
            i--;

        i++;

        if (node->child[i]->n == 2 * T - 1) {
            splitChild(node, i, node->child[i]);

            if (key > node->keys[i])
                i++;
        }

        insertNonFull(node->child[i], key);
    }
}

BTreeNode* insert(BTreeNode *root, int key) {
    if (root == NULL) {
        root = createNode(1);
        root->keys[0] = key;
        root->n = 1;
        return root;
    }

    if (search(root, key) != NULL) {
        printf("Duplicate key %d ignored\n", key);
        return root;
    }

    if (root->n == 2 * T - 1) {
        BTreeNode *newRoot = createNode(0);

        newRoot->child[0] = root;

        splitChild(newRoot, 0, root);

        int i = 0;

        if (key > newRoot->keys[0])
            i++;

        insertNonFull(newRoot->child[i], key);

        return newRoot;
    }

    insertNonFull(root, key);
    return root;
}

int countNodes(BTreeNode *root) {
    if (root == NULL) return 0;

    int count = 1;

    if (!root->leaf) {
        for (int i = 0; i <= root->n; i++)
            count += countNodes(root->child[i]);
    }

    return count;
}

void freeTree(BTreeNode *root) {
    if (root == NULL) return;

    if (!root->leaf) {
        for (int i = 0; i <= root->n; i++)
            freeTree(root->child[i]);
    }

    free(root);
}

int main() {
    int values[] = {10, 20, 5, 6, 12, 30, 7, 17};
    int n = sizeof(values) / sizeof(values[0]);

    BTreeNode *root = NULL;

    printf("B-TREE INSERTION (MINIMUM DEGREE 2)\n");

    for (int i = 0; i < n; i++) {
        root = insert(root, values[i]);

        printf("\nAfter inserting %d: ", values[i]);
        traverse(root);
        printf("\n");
    }

    printf("\nFinal sorted traversal: ");
    traverse(root);

    printf("\nTotal nodes: %d", countNodes(root));

    int tests[] = {5, 30, 99};

    printf("\n\nSEARCH RESULTS\n");

    for (int i = 0; i < 3; i++) {
        if (search(root, tests[i]) != NULL)
            printf("%d found in B-Tree\n", tests[i]);
        else
            printf("%d not found in B-Tree\n", tests[i]);
    }

    printf("\n");
    freeTree(root);

    return 0;
}
