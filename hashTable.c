#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define RESTOCK "RESTOCK"
#define SHOW_STOCK "SHOW_STOCK"
#define SALE "SALE"

typedef struct HashNode {
    int count;
    char* album_name;
    struct HashNode* next;
} HashNode;

static HashNode** table = NULL;
static int table_fullness = 0, table_size = 8; // Start with initial size 8

int hash(char* str) {
    // Simple hash function based on the length of the string
    int hash = 0;
    for (int i = 0; str[i] != '\0'; i++)
        hash = (hash * 31 + str[i]) % table_size;
    return hash;
}

int retrieve(char* album_name) {
    // Retrieve the stock count of the given album
    int index = hash(album_name);
    HashNode* current = table[index];
    while (current != NULL) {
        if (strcmp(current->album_name, album_name) == 0) {
            return current->count;
        }
        current = current->next;
    }
    return 0; // Return 0 if album not found
}

void resize() {
    // Double the size of the table when it's halfway full
    int old_size = table_size;
    table_size *= 2;

    HashNode** new_table = (HashNode**) malloc(table_size * sizeof(HashNode*));
    if (new_table == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < table_size; i++) {
        new_table[i] = NULL;
    }

    // Rehash all items
    for (int i = 0; i < old_size; i++) {
        HashNode* current = table[i];
        while (current != NULL) {
            HashNode* next = current->next;
            int index = hash(current->album_name);
            current->next = new_table[index];
            new_table[index] = current;
            current = next;
        }
    }

    free(table);
    table = new_table;
}

void update(char* album, int k) {
    // Update the stock of the given album by k
    int index = hash(album);
    HashNode* current = table[index];
    while (current != NULL) {
        if (strcmp(current->album_name, album) == 0) {
            current->count += k;
            return;
        }
        current = current->next;
    }

    // Album not found, create new node
    HashNode* new_node = (HashNode*) malloc(sizeof(HashNode));
    if (new_node == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    new_node->count = k;

    // Use malloc and strcpy instead of strdup
    new_node->album_name = malloc(strlen(album) + 1);
    if (new_node->album_name == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    strcpy(new_node->album_name, album);

    new_node->next = table[index];
    table[index] = new_node;

    table_fullness++;

    // Resize if necessary
    if (table_fullness >= table_size / 2) {
        printf("Resizing the table from %d to %d\n", table_size, table_size * 2);
        resize();
    }
}

void pretty_print_table() {
    // Print the current stock for each album in alphabetical order
    printf("-------- FINAL COUNTS --------\n");
    // Sorting the albums alphabetically
    char* albums[table_size];
    int album_count = 0;
    for (int i = 0; i < table_size; i++) {
        HashNode* current = table[i];
        while (current != NULL) {
            albums[album_count++] = current->album_name;
            current = current->next;
        }
    }
    // Sorting the album names
    for (int i = 0; i < album_count - 1; i++) {
        for (int j = i + 1; j < album_count; j++) {
            if (strcmp(albums[i], albums[j]) > 0) {
                char* temp = albums[i];
                albums[i] = albums[j];
                albums[j] = temp;
            }
        }
    }
    // Printing in sorted order
    for (int i = 0; i < album_count; i++) {
        printf("%s: %d\n", albums[i], retrieve(albums[i]));
    }
    printf("------------------------------\n");
}

void free_table() {
    // Free all memory allocated for the hash table
    for (int i = 0; i < table_size; i++) {
        HashNode* current = table[i];
        while (current != NULL) {
            HashNode* temp = current;
            current = current->next;
            free(temp->album_name);
            free(temp);
        }
    }
    free(table);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    // Initialize the hash table
    table = (HashNode**) malloc(table_size * sizeof(HashNode*));
    if (table == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < table_size; i++) {
        table[i] = NULL;
    }

    char command[20], album[150];
    int count;
    while (fscanf(fp, "%s %d %[^\n]s", command, &count, album) == 3) {
        if (strcmp(command, SALE) == 0) {
            int current_stock = retrieve(album);
            if (current_stock < count) {
                printf("Not enough stock of %s\n", album);
            } else {
                update(album, -count);
            }
        } else if (strcmp(command, RESTOCK) == 0) {
            update(album, count);
        } else if (strcmp(command, SHOW_STOCK) == 0) {
            int current_stock = retrieve(album);
            printf("Current stock of %s: %d\n", album, current_stock);
        }
    }

    pretty_print_table();
    free_table();
    fclose(fp);
    return 0;
}
