/* Copyright xhawk, MIT license */

#include "s_list.h"

/*******************************************************************/
/* list                                                            */
/*******************************************************************/

list_t *list_get_prev (list_t *list) {
    return list->prev;
}

list_t *list_get_next (list_t *list) {
    return list->next;
}

void list_set_prev (list_t *list, list_t *other) {
    list->prev = other;
}

void list_set_next (list_t *list, list_t *other) {
    list->next = other;
}

/* Initilization a list */
void list_init(list_t *list) {
    list_set_prev(list, list);
    list_set_next(list, list);
}

/* Connect or disconnect two lists. */
void list_toggle_connect(list_t *list1, list_t *list2) {
    list_t *prev1 = list_get_prev(list1);
    list_t *prev2 = list_get_prev(list2);
    list_set_next(prev1, list2);
    list_set_next(prev2, list1);
    list_set_prev(list1, prev2);
    list_set_prev(list2, prev1);
}

/* Connect two lists. */
void list_connect (list_t *list1, list_t *list2) {
    list_toggle_connect (list1, list2);
}

/* Disconnect tow lists. */
void list_disconnect (list_t *list1, list_t *list2) {
    list_toggle_connect (list1, list2);
}

/* Same as list_connect */
void list_attach (list_t *node1, list_t *node2) {
    list_connect (node1, node2);
}

/* Make node in detach mode */
void list_detach (list_t *node) {
    list_disconnect (node, list_get_next(node));
}

/* Check if list is empty */
int list_is_empty (list_t *list) {
    return (list_get_next(list) == list);
}

int list_size(list_t *list) {
    int n = 0;
    list_t *next;
    for(next = list_get_next(list); next != list; next = list_get_next(next))
        ++n;
    return n;
}
