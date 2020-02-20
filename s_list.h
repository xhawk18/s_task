#ifndef INC_S_LIST_H_
#define INC_S_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

struct tag_list;
typedef struct tag_list {
    struct tag_list *next;
    struct tag_list *prev;
} list_t;

#define GET_PARENT_ADDR(pMe,tParent,eMyName) \
    ((tParent *)((char *)(pMe) - (ptrdiff_t)&((tParent *)0)->eMyName))

list_t *list_get_prev (list_t *list);
list_t *list_get_next (list_t *list);
void list_set_prev (list_t *list, list_t *other);
void list_set_next (list_t *list, list_t *other);
/* Initilization a list */
void list_init(list_t *list);
/* Connect two lists. */
void list_connect (list_t *list1, list_t *list2);
/* Disconnect tow lists. */
void list_disconnect (list_t *list1, list_t *list2);
/* Same as list_connect */
void list_attach (list_t *node1, list_t *node2);
/* Make node in detach mode */
void list_detach (list_t *node);
/* Check if list is empty */
int list_is_empty (list_t *list);
int list_size(list_t *list);


#ifdef __cplusplus
}
#endif
#endif
