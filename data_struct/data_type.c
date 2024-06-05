#include <stddef.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./data_type.h"
#include "./identifier_data.h"

struct DataType* calDataType(struct IdentifierData** idDataList, struct DataType* d1, struct DataType* d2, enum DataTypeCalType type) {
    struct IdentifierData *id1, *id2;
    if (d1->type == 2) {
        id1 = findIdentifer(idDataList, d1->idName);
    } else {
        id1 = malloc(sizeof(struct IdentifierData));
        id1->iValue = d1->ival;
        id1->fValue = d1->fval;
        id1->type = d1->type;
    }
    if (d2->type == 2) {
        id2 = findIdentifer(idDataList, d2->idName);
    } else {
        id2 = malloc(sizeof(struct IdentifierData));
        id2->iValue = d2->ival;
        id2->fValue = d2->fval;
        id2->type = d2->type;
    }
    struct DataType *result = malloc(sizeof(struct DataType));
    switch (type) {
        case plus:
            result->ival = id1->iValue + id2->iValue;
            result->fval = id1->fValue + id2->fValue;
            break;
        case minus:
            result->ival = id1->iValue - id2->iValue;
            result->fval = id1->fValue - id2->fValue;
            break;
        case multi:
            result->ival = id1->iValue * id2->iValue;
            result->fval = id1->fValue * id2->fValue;
            break;
        case division:
            result->ival = id1->iValue / id2->iValue;
            result->fval = id1->fValue / id2->fValue;
            break;
        case mod:
            result->ival = id1->iValue % id2->iValue;
            break;
    }
    result->type = id1->type;
    return result;
}