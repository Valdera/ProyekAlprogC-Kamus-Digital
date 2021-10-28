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
} Item;

typedef struct Kamus {
    Item* data;
    int length;
} Kamus;

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

int main() {
    Kamus kamus = initialize();
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
                display_all(&kamus);
                break;
            case 2:
                add_item(&kamus);
                break;
            case 3:
                sortAscending(&kamus);
                display_all(&kamus);
                break;
            case 4:
                sortDescending(&kamus);
                display_all(&kamus);
                break;
            case 5:
                searchWord(&kamus);
                break;
            default:
                loop = 0;
                break;
        }
        printf("\n");
    }
    free(kamus.data);
    return 0;
}

void display_item(Item item) {
    printf("----------------------");
    printf("\nKata      : %s", item.word);
    printf("\nDeskripsi : %s", item.description);
    printf("----------------------");
}

void display_all(Kamus* kamus) {
    int i;
    printf("\n-------LIST OF WORDS-------\n");
    for (i = 0; i < kamus->length; i++) {
        display_item(kamus->data[i]);
        printf("\n");
    }
}

void write_data(Kamus* kamus) {
    FILE* fp = fopen(FILE_NAME, "w");
    int i;

    if (fp == NULL) {
        printf("Error opening the file %s", FILE_NAME);
        return;
    }

    for (i = 0; i < kamus->length; i++) {
        char* text = (char*)malloc((MAX_SIZE_DESC + MAX_SIZE_WORD) * sizeof(char));
        strcpy(text, kamus->data[i].word);
        strcat(text, "-");
        strcat(text, kamus->data[i].description);
        fprintf(fp, text);
    }
    fprintf(fp, "\n");

    fclose(fp);
}

void read_data(Kamus* kamus) {
    FILE* fp = fopen(FILE_NAME, "r");
    char line[256];

    if (fp == NULL) {
        printf("Error opening the file %s", FILE_NAME);
        return;
    }

    while (fgets(line, sizeof(line), fp)) {
        Item item;
        char *word, *description;
        item.word = (char*)malloc(MAX_SIZE_WORD * sizeof(char));
        word = strtok(line, "-");
        strcpy(item.word, word);
        item.description = (char*)malloc(MAX_SIZE_DESC * sizeof(char));
        description = strtok(NULL, "-");
        strcpy(item.description, description);
        kamus->data[kamus->length++] = item;
    }

    fclose(fp);
}

void add_item(Kamus* kamus) {
    Item item;
    item.word = (char*)malloc(MAX_SIZE_WORD * sizeof(char));
    printf("Masukkan suatu kata : ");
    fflush(stdin);
    gets(item.word);
    item.description = (char*)malloc(MAX_SIZE_DESC * sizeof(char));
    printf("Masukkan deskripsi dari %s : ", item.word);
    fflush(stdin);
    gets(item.description);
    kamus->data[kamus->length++] = item;
    write_data(kamus);
    read_data(kamus);
}

void sortAscending(Kamus* kamus) {
    int i, j;
    for (i = 0; i < kamus->length - 1; i++) {
        for (j = i; j < kamus->length; j++) {
            if (strcmp(kamus->data[i].word, kamus->data[j].word) > 0) {
                swapItem(&kamus->data[i], &kamus->data[j]);
            }
        }
    }
}

void sortDescending(Kamus* kamus) {
    int i, j;
    for (i = 0; i < kamus->length - 1; i++) {
        for (j = i; j < kamus->length; j++) {
            if (strcmp(kamus->data[i].word, kamus->data[j].word) < 0) {
                swapItem(&kamus->data[i], &kamus->data[j]);
            }
        }
    }
}

/**
 * Function to swap 2 string element
 * @param : *element1, *element2
 */
void swapItem(Item* item_1, Item* item_2) {
    Item temp = *item_1;
    *item_1 = *item_2;
    *item_2 = temp;
}

/**
 * Function to search word in dictionary
 */
void searchWord(Kamus* kamus) {
    char* search = (char*)malloc(MAX_SIZE_WORD * sizeof(char));
    printf("\nSearch words: ");
    fflush(stdin);
    gets(search);

    printf("-------RESULTS FOUNDED-------\n");
    for (int i = 0; i < kamus->length; i++) {
        char* ret;
        ret = strstr(kamus->data[i].word, search);
        if (ret != NULL) {
            display_item(kamus->data[i]);
        }
    }

    printf("\n");
    free(search);
}

Kamus initialize() {
    Kamus kamus;
    kamus.data = (Item*)malloc(KAMUS_SIZE * sizeof(Item));
    kamus.length = 0;
    read_data(&kamus);
    return kamus;
}
