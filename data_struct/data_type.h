#ifndef DATA_TYPE_H
#define DATA_TYPE_H
#include "./identifier_data.h"

struct DataType {
    int ival;
    float fval;
    char* idName;
    // 0: ival, 1: fval, 2: identifier
    int type;
};

enum DataTypeCalType {
    plus, minus, multi, division, mod
};

struct DataType* calDataType(struct IdentifierData** idDataList, struct DataType* d1, struct DataType* d2, enum DataTypeCalType type);

#endif