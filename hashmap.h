#ifndef HASH_H
#define HASH_H

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_BOOL
} ValueType;

typedef struct {
    ValueType type;
    union {
        int i;
        float f;
        char *s;
    } data;
} Value;

typedef struct Entry {
    char *key;
    Value value;
    struct Entry *next;
} Entry;

typedef struct {
    Entry **buckets;
    int size;
    int count;
} HashMap;

HashMap *create_map(int size);
void insert_value(HashMap *map, const char *key, Value value);
void insert_int(HashMap *map, const char *key, int i);
void insert_float(HashMap *map, const char *key, float f);
void insert_string(HashMap *map, const char *key, const char *s);
void insert_bool(HashMap *map, const char *key, int b);
Value *get(HashMap *map, const char *key);
void free_map(HashMap *map);

#endif
