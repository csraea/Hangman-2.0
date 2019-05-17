#include "backpack.h"
#include <stdlib.h>

struct backpack* create_backpack(const int capacity){
    struct backpack* bp = (struct backpack*)calloc(1, sizeof(struct backpack));
    bp->capacity=capacity;
    bp->size=0;
    bp->items=NULL;
    return bp;
}

struct backpack* destroy_backpack(struct backpack* bp){
    if(bp==NULL) return NULL;
    bp->items = destroy_containers(bp->items);
    free(bp);
    return NULL;
}

bool add_item_to_backpack(struct backpack* bp, struct item* item){
    if(bp==NULL || item==NULL) return false;
    if(bp->size==bp->capacity || (MOVABLE!=(item->properties & MOVABLE))) return false;
    bp->items = create_container(bp->items, ITEM, item);
    bp->size += 1;
    return true;
}

struct item* get_item_from_backpack(const struct backpack* bp, char* name){
    if(bp==NULL || name==NULL)
        return NULL;
    return get_from_container_by_name(bp->items, name);
}

void delete_item_from_backpack(struct backpack* bp, struct item* item){
    if(bp == NULL || item == NULL) return;
    if(bp->size==0) return;
    bp->items = remove_container(bp->items, item);
    bp->size -= 1;
}