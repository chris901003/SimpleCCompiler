#include <stddef.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "identifier_data.h"
#include "../constant.h"

struct IdentifierData** initIdentifierList() {
    struct IdentifierData **idDataList = malloc(MAX_IDENTIFIER_SIZE * sizeof(struct IdentifierData*));
    for (int idx = 0; idx < MAX_IDENTIFIER_SIZE; idx++) {
        idDataList[idx] = NULL;
    }
    return idDataList;
}

struct IdentifierData* findIdentifer(struct IdentifierData** idDataList, char *name) {
    int hash = getIdentiferHash(name);
    struct IdentifierData *start = idDataList[hash];
    while (start != NULL) {
        if (strcmp(name, start->name) == 0) {
            return start;
        }
        start = start->nxt;
    }
    return NULL;
}

void saveIdentifier(struct IdentifierData** idDataList, struct IdentifierData* idData) {
    int hash = getIdentiferHash(idData->name);
    struct IdentifierData *start = idDataList[hash];
    if (start == NULL) {
        idDataList[hash] = idData;
    } else {
        while (start->nxt != NULL) {
            start = start->nxt;
        }
        start->nxt = idData;
    }
    return;
}

int getIdentiferHash(char *name) {
    int hash = 0;
    for (int i = 0; name[i] != '\0'; i++) {
        hash = (hash * 26) % MAX_IDENTIFIER_SIZE;
        hash = (hash + name[i] - 'a') % MAX_IDENTIFIER_SIZE;
    }
    return hash;
}