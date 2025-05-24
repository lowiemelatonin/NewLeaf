#include "hashmap.h"
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 8
#define LOAD_FACTOR 0.75

unsigned long hash(const char *str){
    unsigned long hash = 5381;
    int c;
    while((c = *str++)){
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

void resize(HashMap *map);

HashMap *create_map(int size){
    HashMap *map = malloc(sizeof(HashMap));
    map->size = size;
    map->count = 0;
    map->buckets = calloc(size, sizeof(Entry*));
    return map;
}

void insert_value(HashMap *map, const char *key, Value value){
    if((float)(map->count + 1) / map->size > LOAD_FACTOR){
        resize(map);
    }

    unsigned long index = hash(key) % map->size;
    Entry *entry = map->buckets[index];
    while(entry){
        if(strcmp(entry->key, key) == 0){
            if(entry->value.type == TYPE_STRING){
                free(entry->value.data.s);
            }
            entry->value = value;
            return;
        }
        entry = entry->next;
    }
    entry = malloc(sizeof(Entry));
    entry->key = strdup(key);
    entry->value = value;
    entry->next = map->buckets[index];
    map->buckets[index] = entry;
    map->count++;
}

Value *get(HashMap *map, const char *key){
    unsigned long index = hash(key) % map->size;
    Entry *entry = map->buckets[index];
    while(entry){
        if(strcmp(entry->key, key) == 0){
            return &entry->value;
        }
        entry = entry->next;
    }
    return NULL;
}

void resize(HashMap *map){
    int old_size = map->size;
    Entry **old_buckets = map->buckets;

    map->size *= 2;
    map->count = 0;
    map->buckets = calloc(map->size, sizeof(Entry*));
    for(int i = 0; i < old_size; i++){
        Entry *entry = old_buckets[i];
        while(entry){
            Entry *next = entry->next;
            Value val = entry->value;
            if(val.type == TYPE_STRING){
                val.data.s = strdup(val.data.s);
            }
            insert_value(map, entry->key, val);
            free(entry->key);
            if(entry->value.type == TYPE_STRING){
                free(entry->value.data.s);
            }
            free(entry);
            entry = next;
        }
    }
    free(old_buckets);
}

void free_map(HashMap *map){
    for(int i = 0; i < map->size; i++){
        Entry *entry = map->buckets[i];
        while(entry){
            Entry *next = entry->next;
            free(entry->key);
            if(entry->value.type == TYPE_STRING){
                free(entry->value.data.s);
            }
            free(entry);
            entry = next;
        }
    }
    free(map->buckets);
    free(map);
}

void insert_int(HashMap *map, const char *key, int i){
    Value val = { .type = TYPE_INT, .data.i = i };
    insert_value(map, key, val);
}

void insert_float(HashMap *map, const char *key, float f){
    Value val = { .type = TYPE_FLOAT, .data.f = f };
    insert_value(map, key, val);
}

void insert_string(HashMap *map, const char *key, const char *s){
    Value val = { .type = TYPE_STRING, .data.s = strdup(s) };
    insert_value(map, key, val);
}

void insert_bool(HashMap *map, const char *key, int b){
    Value val = { .type = TYPE_BOOL, .data.i = b ? 1 : 0};
    insert_value(map, key, val);
}
