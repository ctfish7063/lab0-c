#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

/*Merge two queues into one queue*/
void q_merge_two(struct list_head *L1, struct list_head *L2);


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head;
    head = malloc(sizeof(struct list_head));
    if (head) {
        INIT_LIST_HEAD(head);
    }
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    element_t *entry, *safe;
    list_for_each_entry_safe (entry, safe, l, list) {
        free(entry->value);
        free(entry);
    };
    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head || !s) {
        return false;
    }
    element_t *element = malloc(sizeof(element_t));
    if (!element) {
        return false;
    }
    char *tmp_str = strdup(s);
    if (!tmp_str) {
        free(element);
        return false;
    }
    element->value = tmp_str;
    INIT_LIST_HEAD(&element->list);
    list_add(&element->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head || !s) {
        return false;
    }
    element_t *element = malloc(sizeof(element_t));
    if (!element) {
        return false;
    }
    char *tmp_str = strdup(s);
    if (!tmp_str) {
        free(element);
        return false;
    }
    element->value = tmp_str;
    INIT_LIST_HEAD(&element->list);
    list_add_tail(&element->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }
    element_t *element = list_first_entry(head, element_t, list);
    list_del(head->next);
    if (sp && bufsize > 0) {
        strncpy(sp, element->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    return element;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }
    element_t *element = list_last_entry(head, element_t, list);
    list_del(head->prev);
    if (sp && bufsize > 0) {
        strncpy(sp, element->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    return element;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head) {
        return 0;
    }
    int len = 0;
    struct list_head *li;
    list_for_each (li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head)) {
        return false;
    }
    struct list_head *slow = head->next;
    for (struct list_head *fast = head->next;
         fast != head && fast->next != head; fast = fast->next->next) {
        slow = slow->next;
    }
    list_del(slow);
    element_t *ele = list_entry(slow, element_t, list);
    free(ele->value);
    free(ele);
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head)
        return false;
    if (list_empty(head) || list_is_singular(head))
        return true;
    struct list_head *cur = head->next;
    int flag = 0;
    while (cur != head) {
        element_t *ele_cur = list_entry(cur, element_t, list);
        struct list_head *tmp_node = cur->next;
        while (tmp_node != head) {
            element_t *ele_tmp = list_entry(tmp_node, element_t, list);
            tmp_node = tmp_node->next;
            if (strcmp(ele_cur->value, ele_tmp->value) == 0) {
                flag = 1;
                list_del(tmp_node->prev);
                free(ele_tmp->value);
                free(ele_tmp);
            } else
                break;
        }
        cur = cur->next;
        if (flag) {
            flag = 0;
            list_del(cur->prev);
            free(ele_cur->value);
            free(ele_cur);
        }
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Merge two queues into the first parameter queue, which is in ascending
 * order*/
void q_merge_two(struct list_head *L1, struct list_head *L2)
{
    if (L1 && L2) {
        if (L1 == L2) {
            return;
        }
        if (list_empty(L1) || list_empty(L2)) {
            list_splice_tail_init(L2, L1);
            return;
        }
        struct list_head *tail = L1->prev, *node = NULL;
        while (node != tail && !list_empty(L2)) {
            element_t *ele_1 = list_first_entry(L1, element_t, list);
            element_t *ele_2 = list_first_entry(L2, element_t, list);
            node = strcmp(ele_1->value, ele_2->value) < 0 ? L1->next : L2->next;
            list_move_tail(node, L1);
        }
        if (list_empty(L2)) {
            struct list_head left;
            list_cut_position(&left, L1, tail);
            list_splice_tail_init(&left, L1);
        } else {
            list_splice_tail_init(L2, L1);
        }
    }
    return;
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head)) {
        return;
    }
    struct list_head *slow = head, *tail = head->prev;
    for (struct list_head *fast = slow; fast != tail && fast->next != tail;
         fast = fast->next->next) {
        slow = slow->next;
    }
    struct list_head left;
    list_cut_position(&left, head, slow);
    q_sort(head);
    q_sort(&left);
    q_merge_two(head, &left);
}


/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    if (!head || list_empty(head)) {
        return 0;
    }
    if (list_is_singular(head)) {
        queue_contex_t *qct = list_first_entry(head, queue_contex_t, chain);
        return qct->size;
    }
    struct list_head *end = head->prev;
    while (end != head->next) {
        for (struct list_head *start = head->next;
             start->prev != end && start != end;
             start = start->next, end = end->prev) {
            queue_contex_t *qct_s = list_entry(start, queue_contex_t, chain);
            queue_contex_t *qct_e = list_entry(end, queue_contex_t, chain);
            q_merge_two(qct_s->q, qct_e->q);
            qct_s->size += qct_e->size;
            qct_e->size = 0;
        }
    }
    queue_contex_t *qct_res = list_first_entry(head, queue_contex_t, chain);
    return qct_res->size;
}