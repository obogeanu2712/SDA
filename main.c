#include <stdio.h>
#include <stdlib.h>

struct Bucket {
    int value;
};

struct Bucket* fillBucket(int value) {
    struct Bucket* bucket = malloc(sizeof(struct Bucket));
    bucket->value = value;
    return bucket;
}

struct Hashmap {
    struct Bucket** buckets;
    int size;
};

int hash(int value, int size) {
    int hashValue = value % size;
    
    if(hashValue < 0) 
    {
        hashValue += size;
    }
    return hashValue;
    
}

int counts(struct Hashmap* hashmap) {
    
    int countsValue = 0;
    
    for(int i = 0; i < hashmap->size; i++) {
        if(hashmap->buckets[i] != NULL) {
            countsValue++;
        }
    }
    
    return countsValue;
}

float load(struct Hashmap* hashmap) {
    return 1.0f * counts(hashmap) / hashmap->size;
}

struct Hashmap* createHashmap(int size) {
    struct Hashmap* hashmap = malloc(sizeof(struct Hashmap));
    hashmap->size = size;
    hashmap->buckets = calloc(size, sizeof(struct Bucket));
    return hashmap;
}

void deleteHashmap(struct Hashmap* hashmap) {
    for(int i = 0; i < hashmap->size; i++) {
        if(hashmap->buckets[i] != NULL) {
            free(hashmap->buckets[i]);
            hashmap->buckets[i] = NULL;
        }
        free(hashmap->buckets);
        hashmap->buckets = NULL;
        free(hashmap);
    }
}

struct Hashmap* reshashing(struct Hashmap* source) {
    
    struct Hashmap* destination = createHashmap(2 * source->size);
    
    for(int i = 0; i < source->size; i++) {
        if(source->buckets[i] != NULL) {
            int valueToInsert = source->buckets[i]->value;
            int index = hash(valueToInsert, destination->size);
            destination->buckets[index] = fillBucket(valueToInsert);
        }
    }
    
    deleteHashmap(source);
    
    return destination;
}

struct Hashmap* addHashmap(struct Hashmap* hashmap, int value) {
    
    float loadValue = load(hashmap);
    
    if(loadValue > 0.75f) {
        hashmap = reshashing(hashmap);
    }
    
    int index = hash(value, hashmap->size);
    
    hashmap->buckets[index] = fillBucket(value);
    
    return hashmap;
}

void displayHashmap(struct Hashmap* hashmap, FILE* output) {
    for(int i = 0; i < hashmap->size; i++) {
        fprintf(output, "%d: ", i);
        if(hashmap->buckets[i] != NULL) {
            fprintf(output, "%d", hashmap->buckets[i]->value);
        }
        fprintf(output, "\n");
    }
}

int main() {

    FILE* input = fopen("input.txt", "r");
    FILE* output = fopen("output.txt", "w");

    int test;
    do {
        fscanf(input, "%d", &test);
        
        struct Hashmap* hashmap = createHashmap(2);
        
        switch(test) {
            case 1 : {
                int value;
                fscanf(input, "%d", &value);
                hashmap = addHashmap(hashmap, value);
                break;
            }
            case 2 : {
                displayHashmap(hashmap, output);
                break;
            }
        }
    }while(test != 3);
    
    fclose(input);
    fclose(output);

    return 0;
}