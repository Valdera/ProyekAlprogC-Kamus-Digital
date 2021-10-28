#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define KAMUS_SIZE 100
#define MAX_SIZE_WORD 15
#define MAX_SIZE_DESC 100
#define FILE_NAME "database.txt"

typedef struct Item {
    char* word;
    char* description;
    char* uncommon;
} Item;

typedef struct Kamus {
    Item* data;
    int length;
} Kamus;

void menu(Kamus* kamus);
void display_item(Item item);
void display_all(Kamus* kamus);
void write_data(Kamus* kamus);
void read_data(Kamus* kamus);
void add_item(Kamus* kamus);
void sortAscending(Kamus* kamus);
void sortDescending(Kamus* kamus);
void searchWord(Kamus* kamus);
void swapItem(Item* item_1, Item* item_2);
Kamus initialize();

/**
 * Main function
 */
int main() {
    // Initialize kamus
    Kamus kamus = initialize();

    // Run the menu function
    menu(&kamus);

    // Free allocated heap memory
    free(kamus.data);

    return 0;
}

/**
 * Function to run menu program
 * @param Kamus* kamus
 */
void menu(Kamus* kamus) {
    int loop = 1;
    int menu;
    while (loop) {
        printf("-------KAMUS DIGITAL-------");
        printf("\n1. Display all words in dictionary");
        printf("\n2. Insert word in dictionary");
        printf("\n3. Sort all words in dictionary A-Z ");
        printf("\n4. Sort all words in dictionary Z-A ");
        printf("\n5. Search word in dictionary");
        printf("\n6. Exit");
        printf("\nMasukkan menu pilihan : ");
        scanf("%d", &menu);
        switch (menu) {
            case 1:
                display_all(kamus);
                break;
            case 2:
                add_item(kamus);
                break;
            case 3:
                sortAscending(kamus);
                display_all(kamus);
                break;
            case 4:
                sortDescending(kamus);
                display_all(kamus);
                break;
            case 5:
                searchWord(kamus);
                break;
            default:
                loop = 0;
                break;
        }
        printf("\n");
    }
}

/**
 * Function to display word in dictionary
 * @param Item item
 */
void display_item(Item item) {
    printf("----------------------");
    printf("\nKata            : %s", item.word);
    printf("\nBentuk tak baku : %s", item.uncommon);
    printf("\nDeskripsi       : %s", item.description);
    printf("----------------------");
}

/**
 * Function to display all words in dictionary
 * @param Kamus* kamus
 */
void display_all(Kamus* kamus) {
    int i;
    printf("\n-------LIST OF WORDS-------\n");
    for (i = 0; i < kamus->length; i++) {
        display_item(kamus->data[i]);
        printf("\n");
    }
}

/**
 * Function to refresh the state of dictionary
 * @param Kamus* kamus
 */
void refresh(Kamus* kamus) {
    free(kamus->data);
    kamus->data = (Item*)malloc(KAMUS_SIZE * sizeof(Item));
    kamus->length = 0;
    read_data(kamus);
}

/**
 * Function to save the dictionary to .txt
 * @param Kamus* kamus
 */
void write_data(Kamus* kamus) {
    FILE* fp = fopen(FILE_NAME, "w");
    int i;

    // If file not found
    if (fp == NULL) {
        printf("Error opening the file %s", FILE_NAME);
        return;
    }

    for (i = 0; i < kamus->length; i++) {
        char* text = (char*)malloc((MAX_SIZE_DESC + MAX_SIZE_WORD) * sizeof(char));
        // Insert the word
        strcpy(text, kamus->data[i].word);
        strcat(text, "-");
        // Insert uncommon form of the word
        strcat(text, kamus->data[i].uncommon);
        strcat(text, "-");
        // Insert description of the word
        strcat(text, kamus->data[i].description);
        fprintf(fp, text);
    }
    fprintf(fp, "\n");

    // Close the file
    fclose(fp);
}

/**
 * Function to read the saved dictionary in .txt
 * @param Kamus* kamus
 */
void read_data(Kamus* kamus) {
    FILE* fp = fopen(FILE_NAME, "r");
    char line[256];

    // If file not found
    if (fp == NULL) {
        printf("Error opening the file %s", FILE_NAME);
        return;
    }

    while (fgets(line, sizeof(line), fp)) {
        Item item;
        char *word, *uncommon, *description;

        // Insert the word
        item.word = (char*)malloc(MAX_SIZE_WORD * sizeof(char));
        word = strtok(line, "-");
        strcpy(item.word, word);

        // Insert uncommon form of the word
        item.uncommon = (char*)malloc(MAX_SIZE_WORD * sizeof(char));
        uncommon = strtok(NULL, "-");
        strcpy(item.uncommon, uncommon);

        // Insert description of the word
        item.description = (char*)malloc(MAX_SIZE_DESC * sizeof(char));
        description = strtok(NULL, "-");
        strcpy(item.description, description);

        kamus->data[kamus->length++] = item;
    }

    // Close the file
    fclose(fp);
}

/**
 * Function to add item to dictionary
 * @param Kamus* kamus
 */
void add_item(Kamus* kamus) {
    Item item;

    // Get the word
    item.word = (char*)malloc(MAX_SIZE_WORD * sizeof(char));
    printf("Masukkan suatu kata : ");
    fflush(stdin);
    gets(item.word);

    // Get the uncommon form of the word
    item.uncommon = (char*)malloc(MAX_SIZE_WORD * sizeof(char));
    printf("Masukkan bentuk tidak baku dari %s : ", item.word);
    fflush(stdin);
    gets(item.uncommon);

    // Get the description of the word
    item.description = (char*)malloc(MAX_SIZE_DESC * sizeof(char));
    printf("Masukkan deskripsi dari %s : ", item.word);
    fflush(stdin);
    gets(item.description);

    // Insert the word item to item array
    kamus->data[kamus->length++] = item;

    // Save the dictionary state
    write_data(kamus);

    // Refresh the dictionary state
    refresh(kamus);
}

/**
 * Function to sort word in dictionary A-Z
 * @param Kamus* kamus
 */
void sortAscending(Kamus* kamus) {
    int i, j;
    for (i = 0; i < kamus->length - 1; i++) {
        for (j = i; j < kamus->length; j++) {
            if (strcmp(kamus->data[i].word, kamus->data[j].word) > 0) {
                // Swap item if word[i] > word[j]
                swapItem(&kamus->data[i], &kamus->data[j]);
            }
        }
    }
}

/**
 * Function to sort word in dictionary Z-A
 * @param Kamus* kamus
 */
void sortDescending(Kamus* kamus) {
    int i, j;
    for (i = 0; i < kamus->length - 1; i++) {
        for (j = i; j < kamus->length; j++) {
            if (strcmp(kamus->data[i].word, kamus->data[j].word) < 0) {
                // Swap item if word[i] < word[j]
                swapItem(&kamus->data[i], &kamus->data[j]);
            }
        }
    }
}

/**
 * Function to swap Item in an array of Item
 * @param *item_1 *item_2
 */
void swapItem(Item* item_1, Item* item_2) {
    Item temp = *item_1;
    *item_1 = *item_2;
    *item_2 = temp;
}

/**
 * Function to search word in dictionary
 * @param Kamus* kamus
 */
void searchWord(Kamus* kamus) {
    // Get the word to search from the user
    char* search = (char*)malloc(MAX_SIZE_WORD * sizeof(char));
    printf("\nSearch words: ");
    fflush(stdin);
    gets(search);

    // Print all results that contain the searched word
    printf("-------RESULTS FOUNDED-------\n");
    for (int i = 0; i < kamus->length; i++) {
        char* ret;
        ret = strstr(kamus->data[i].word, search);
        // Display word if the it contain the searched word
        if (ret != NULL) {
            display_item(kamus->data[i]);
        }
    }

    printf("\n");
    free(search);
}

/**
 * Function to initialize dictionary
 * @return Kamus
 */
Kamus initialize() {
    Kamus kamus;
    // Allocated memory from item array
    kamus.data = (Item*)malloc(KAMUS_SIZE * sizeof(Item));
    kamus.length = 0;
    // Read the data from saved dictionary
    read_data(&kamus);
    return kamus;
}
