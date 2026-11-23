
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ALPHABET 26
#define MAX_LEN 100

typedef struct Trie {
    struct Trie *child[ALPHABET];
    int words;
    int end;
} Trie;

Trie *node(void) {
    return calloc(1, sizeof(Trie));
}

int indexOf(char c) {
    return c - 'a';
}

void insert(Trie *t, const char *w) {
    Trie *path[MAX_LEN + 1];
    int len = strlen(w);

    if (len > MAX_LEN) return;

    path[0] = t;

    for (int i = 0; i < len; i++) {
        int index = indexOf(w[i]);

        if (index < 0 || index >= ALPHABET)
            return;

        if (!t->child[index])
            t->child[index] = node();

        t = t->child[index];
        path[i + 1] = t;
    }

    if (!t->end) {
        t->end = 1;
        for (int i = 0; i <= len; i++)
            path[i]->words++;
    }
}

int search(Trie *t, const char *w) {
    for (int i = 0; w[i]; i++) {
        int index = indexOf(w[i]);

        if (index < 0 || index >= ALPHABET ||
            !t->child[index])
            return 0;

        t = t->child[index];
    }

    return t->end;
}

int count_prefix(const Trie *t, const char *p) {
    for (int i = 0; p[i]; i++) {
        int index = indexOf(p[i]);

        if (index < 0 || index >= ALPHABET ||
            !t->child[index])
            return 0;

        t = t->child[index];
    }

    return t->words;
}

int deleteWord(Trie *root, const char *w) {
    Trie *path[MAX_LEN + 1];
    int indexes[MAX_LEN];
    int len = strlen(w);
    Trie *t = root;

    if (len > MAX_LEN) return 0;

    path[0] = root;

    for (int i = 0; i < len; i++) {
        int index = indexOf(w[i]);

        if (index < 0 || index >= ALPHABET ||
            !t->child[index])
            return 0;

        indexes[i] = index;
        t = t->child[index];
        path[i + 1] = t;
    }

    if (!t->end) return 0;

    t->end = 0;

    for (int i = 0; i <= len; i++)
        path[i]->words--;

    for (int i = len; i > 0; i--) {
        if (path[i]->words == 0) {
            free(path[i]);
            path[i - 1]->child[indexes[i - 1]] = NULL;
        } else {
            break;
        }
    }

    return 1;
}

void autocomplete(Trie *t, char *buffer, int depth) {
    if (t->end) {
        buffer[depth] = '\0';
        printf("%s\n", buffer);
    }

    for (int i = 0; i < ALPHABET; i++) {
        if (t->child[i]) {
            buffer[depth] = 'a' + i;
            autocomplete(t->child[i], buffer, depth + 1);
        }
    }
}

void showSuggestions(Trie *root, const char *prefix) {
    Trie *t = root;
    char buffer[MAX_LEN + 1];

    for (int i = 0; prefix[i]; i++) {
        int index = indexOf(prefix[i]);

        if (index < 0 || index >= ALPHABET ||
            !t->child[index]) {
            printf("No suggestions\n");
            return;
        }

        t = t->child[index];
    }

    strcpy(buffer, prefix);
    autocomplete(t, buffer, strlen(prefix));
}

int countNodes(Trie *t) {
    if (!t) return 0;

    int count = 1;

    for (int i = 0; i < ALPHABET; i++)
        count += countNodes(t->child[i]);

    return count;
}

void destroy(Trie *t) {
    if (!t) return;

    for (int i = 0; i < ALPHABET; i++)
        destroy(t->child[i]);

    free(t);
}

void generateName(int number, char *name) {
    strcpy(name, "person");

    int pos = 6;
    int value = number;

    for (int i = 0; i < 3; i++) {
        name[pos++] = 'a' + (value % 26);
        value /= 26;
    }

    name[pos] = '\0';
}

int main(void) {
    const char *contacts[] = {
        "anand", "anitha", "anu",
        "arun", "bala", "anusuya"
    };

    Trie *root = node();

    printf("TRIE CONTACT SEARCH\n");

    for (int i = 0; i < 6; i++)
        insert(root, contacts[i]);

    const char *typed[] = {"", "a", "an", "anu", "b", "z"};

    printf("\nPrefix counts:\n");

    for (int i = 0; i < 6; i++)
        printf("'%s': %d  ",
               typed[i], count_prefix(root, typed[i]));

    printf("\n\nSearch for anand: %s\n",
           search(root, "anand") ? "Found" : "Not Found");

    printf("Search for pooja: %s\n",
           search(root, "pooja") ? "Found" : "Not Found");

    printf("\nAutocomplete for 'an':\n");
    showSuggestions(root, "an");

    printf("\nAutocomplete for 'ar':\n");
    showSuggestions(root, "ar");

    printf("\nDeleting 'anu': %s\n",
           deleteWord(root, "anu") ? "Successful" : "Not Found");

    printf("Search for anu after deletion: %s\n",
           search(root, "anu") ? "Found" : "Not Found");

    printf("\nNodes created: %d\n", countNodes(root));
    printf("Approximate memory: %zu bytes\n",
           (size_t)countNodes(root) * sizeof(Trie));

    destroy(root);

    /* 5,000-name prefix-search experiment */
    Trie *largeRoot = node();
    char name[MAX_LEN + 1];

    for (int i = 0; i < 5000; i++) {
        generateName(i, name);
        insert(largeRoot, name);
    }

    clock_t start = clock();

    int result = 0;
    for (int i = 0; i < 10000; i++)
        result += count_prefix(largeRoot, "person");

    clock_t end = clock();

    printf("\n5000 CONTACT EXPERIMENT\n");
    printf("Contacts loaded: 5000\n");
    printf("Prefix 'person' count: %d\n",
           count_prefix(largeRoot, "person"));

    printf("Average prefix-search time: %.6f seconds\n",
           (double)(end - start) / CLOCKS_PER_SEC / 10000);

    printf("Total Trie nodes: %d\n", countNodes(largeRoot));

    destroy(largeRoot);

    return 0;
}

