/** @file listy.h
 *  @brief Function prototypes for the linked list.
 */
#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#define MAX_WORD_LEN 50

/**
 * @brief An struct that represents a node in the linked list.
 */
typedef struct node_t
{
    char *word;
	int count;
    struct node_t *next;
} node_t;

/**
 * Function protypes associated with a linked list.
 */
node_t *new_node(char *val, int count);
node_t *add_front(node_t *, node_t *);
node_t *add_end(node_t *, node_t *);
node_t *add_inorder(node_t *, node_t *);
node_t *add_ascending_order(node_t *list, node_t *new);
node_t *peek_front(node_t *);
node_t *remove_front(node_t *);
void apply(node_t *, void (*fn)(node_t *, void *), void *arg);
int is_in(node_t *list, char *name);
void decrease(node_t *list);

#endif
