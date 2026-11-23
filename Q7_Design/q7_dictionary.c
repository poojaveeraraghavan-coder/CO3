
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORDS 1000
#define MAX_LEN 100
#define ALPHABET 26
#define MAX_RESULTS 10

typedef struct Trie {
    struct Trie *child[ALPHABET];
    int end;
} Trie;

char words[MAX_WORDS][MAX_LEN];
int count = 0;
size_t trieNodes = 0;

Trie *createNode(void) {
    Trie *t = calloc(1, sizeof(Trie));
    if (t) trieNodes++;
    return t;
}

void insert(Trie *root, const char *word) {
    Trie *p = root;

    for (int i = 0; word[i]; i++) {
        char c = word[i];
        if (c < 'a' || c > 'z') return;

        int index = c - 'a';
        if (!p->child[index])
            p->child[index] = createNode();

        p = p->child[index];
    }
    p->end = 1;
}

int search(Trie *root, const char *word) {
    Trie *p = root;

    for (int i = 0; word[i]; i++) {
        if (word[i] < 'a' || word[i] > 'z') return 0;
        int index = word[i] - 'a';

        if (!p->child[index]) return 0;
        p = p->child[index];
    }
    return p->end;
}

void autocomplete(Trie *p, char *buffer, int depth,
                  int *printed) {
    if (!p || *printed >= MAX_RESULTS) return;

    if (p->end) {
        buffer[depth] = '\0';
        printf("%s\n", buffer);
        (*printed)++;
    }

    for (int i = 0; i < ALPHABET && *printed < MAX_RESULTS; i++) {
        if (p->child[i]) {
            buffer[depth] = 'a' + i;
            autocomplete(p->child[i], buffer, depth + 1, printed);
        }
    }
}

void findPrefix(Trie *root, const char *prefix) {
    Trie *p = root;

    for (int i = 0; prefix[i]; i++) {
        if (prefix[i] < 'a' || prefix[i] > 'z') {
            printf("Invalid prefix\n");
            return;
        }

        int index = prefix[i] - 'a';
        if (!p->child[index]) {
            printf("No suggestions found.\n");
            return;
        }
        p = p->child[index];
    }

    char buffer[MAX_LEN];
    strcpy(buffer, prefix);
    int printed = 0;

    autocomplete(p, buffer, (int)strlen(prefix), &printed);

    if (printed == 0)
        printf("No suggestions found.\n");
}

int compareWords(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b);
}

int lowerBound(const char *key) {
    int low = 0, high = count;

    while (low < high) {
        int mid = low + (high - low) / 2;
        if (strcmp(words[mid], key) < 0)
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}

void rangeListing(const char *start, const char *finish) {
    int i = lowerBound(start);
    int found = 0;

    printf("Words in range:\n");
    while (i < count && strcmp(words[i], finish) <= 0) {
        printf("%s\n", words[i]);
        i++;
        found = 1;
    }

    if (!found)
        printf("No words found in this range.\n");
}

void destroy(Trie *root) {
    if (!root) return;

    for (int i = 0; i < ALPHABET; i++)
        destroy(root->child[i]);

    free(root);
}

int main(void) {
    Trie *root = createNode();
    FILE *fp = fopen("dictionary.txt", "r");

    if (!fp) {
        printf("Create dictionary.txt and add lowercase words.\n");
        destroy(root);
        return 1;
    }

    while (count < MAX_WORDS &&
           fscanf(fp, "%99s", words[count]) == 1) {
        int valid = 1;

        for (int j = 0; words[count][j]; j++) {
            if (words[count][j] < 'a' ||
                words[count][j] > 'z') {
                valid = 0;
                break;
            }
        }

        if (valid) {
            insert(root, words[count]);
            count++;
        }
    }
    fclose(fp);

    qsort(words, count, sizeof(words[0]), compareWords);

    printf("DICTIONARY SEARCH SYSTEM\n");
    printf("Words loaded: %d\n", count);

    char key[MAX_LEN];
    printf("\nEnter a word for exact lookup: ");
    scanf("%99s", key);

    clock_t start = clock();
    int found = search(root, key);
    clock_t end = clock();

    printf("Exact lookup: %s\n", found ? "Found" : "Not found");
    printf("Lookup time: %.6f seconds\n",
           (double)(end - start) / CLOCKS_PER_SEC);

    printf("\nEnter prefix for autocomplete: ");
    scanf("%99s", key);

    printf("Autocomplete suggestions:\n");
    start = clock();
    findPrefix(root, key);
    end = clock();

    printf("Autocomplete time: %.6f seconds\n",
           (double)(end - start) / CLOCKS_PER_SEC);

    char first[MAX_LEN], last[MAX_LEN];
    printf("\nEnter starting word for range: ");
    scanf("%99s", first);
    printf("Enter ending word for range: ");
    scanf("%99s", last);

    start = clock();
    rangeListing(first, last);
    end = clock();

    printf("Range listing time: %.6f seconds\n",
           (double)(end - start) / CLOCKS_PER_SEC);

    size_t trieMemory = trieNodes * sizeof(Trie);
    size_t arrayMemory = sizeof(words);

    printf("\nTrie nodes: %zu\n", trieNodes);
    printf("Approximate trie memory: %zu bytes\n", trieMemory);
    printf("Sorted array storage: %zu bytes\n", arrayMemory);
    printf("Total approximate storage: %zu bytes\n",
           trieMemory + arrayMemory);

    destroy(root);
    return 0;
}

