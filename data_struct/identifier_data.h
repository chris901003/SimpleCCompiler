#ifndef IDENTIFIER_DATA_H
#define IDENTIFIER_DATA_H

struct IdentifierData {
    char *name;
    int hash;

    // 0: int, 1: float
    int type;
    int iValue;
    float fValue;
    struct IdentifierData *nxt;
};

struct IdentifierData** initIdentifierList();
struct IdentifierData* findIdentifer(struct IdentifierData** idDataList, char *name);
void saveIdentifier(struct IdentifierData** idDataList, struct IdentifierData* idData);
int getIdentiferHash(char *name);

#endif