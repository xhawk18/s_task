#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../s_rbtree.h"

typedef struct {
    int a;
    RBTNode rbt_node;
    int b;
} Obj;


int rbt_comparator_a(const RBTNode* a, const RBTNode* b, void* arg) {
    Obj* obj_a = GET_PARENT_ADDR(a, Obj, rbt_node);
    Obj* obj_b = GET_PARENT_ADDR(b, Obj, rbt_node);
    return obj_a->a - obj_b->a;
}

void rbt_combiner_a(RBTNode* existing, const RBTNode* newdata, void* arg) {
    Obj* obj_a = GET_PARENT_ADDR(existing, Obj, rbt_node);
    const Obj* obj_b = GET_PARENT_ADDR(newdata, Obj, rbt_node);
    obj_a->a = obj_b->a;
    obj_a->b = obj_b->b;
}

void dump(RBTree *rbt, RBTNode *self) {
    if (self == RBTNIL)
        printf("D: NIL \n");
    else {
        RBTNode* node = self;
        Obj* obj = GET_PARENT_ADDR(node, Obj, rbt_node);
        //printf("D: %p %p %p %p\n", self, node->left, node->right, node->parent);
        printf("D: ");
        //printf("%p ", self);
        printf("%d[%d] ", obj->a, (int)node->color);

        if (node->left == RBTNIL)
            printf("NIL ");
        else {
            //printf("%p ", node->left);
            Obj* obj = GET_PARENT_ADDR(node->left, Obj, rbt_node);
            printf("%d[%d] ", obj->a, (int)node->left->color);
        }

        if (node->right == RBTNIL)
            printf("NIL ");
        else {
            //printf("%p ", node->right);
            Obj* obj = GET_PARENT_ADDR(node->right, Obj, rbt_node);
            printf("%d[%d] ", obj->a, (int)node->right->color);
        }

        printf("\n");
        dump(rbt, node->left);
        dump(rbt, node->right);
    }
    
}

int main() {
    RBTree rbtree;

    rbt_create(&rbtree,
        rbt_comparator_a,
        NULL
    );

    srand((unsigned int)time(NULL));
    int i = 0;
    while (true) {
        int rnd = rand() % 10;

        if (rnd < 7) {
            //²åÈë
            Obj *obj = (Obj *)malloc(sizeof(Obj));
            obj->a = (++i);
            if(!rbt_insert(&rbtree, &obj->rbt_node))
                free(obj);
        }
        else {
            //É¾³ý
            Obj fobj;
            fobj.a = rand() % 100;
            fobj.a = 6;
            RBTNode *found = rbt_find(&rbtree, &fobj.rbt_node);
            if (found != NULL) {
                RBTNode* found2 = rbt_find(&rbtree, &fobj.rbt_node);
                rbt_delete(&rbtree, found);

                Obj* obj = GET_PARENT_ADDR(found, Obj, rbt_node);
                printf("delete %d %d %p %p\n", obj->a, fobj.a, found, found2);
                free(obj);
            }
        }

        printf("=============================\n");
        dump(&rbtree, rbtree.root);

        RBTreeIterator itr;
        rbt_begin_iterate(&rbtree, LeftRightWalk, &itr);
        while (true) {
            RBTNode *node = rbt_iterate(&itr);
            if (node == NULL) break; 
            Obj* obj = GET_PARENT_ADDR(node, Obj, rbt_node);
            printf("%d ", obj->a);
        }
        printf("\n");
    }

    return 0;
}