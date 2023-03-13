#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

void q_merge_two(struct list_head *, struct list_head *);

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
        free(element);
        element = NULL;
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
    struct list_head *tail = head->prev;
    tail->next = NULL;  // unlink tail and head
    for (struct list_head *fast = head->next; fast && fast->next;
         fast = fast->next->next) {
        slow = slow->next;
    }
    tail->next = head;  // relink tail and head
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
    if (!head || list_empty(head)) {
        return false;
    }
    struct list_head *node, *safe;
    list_for_each_safe (node, safe, head) {
        int flag = 0;
        element_t *ele_cur = list_entry(node, element_t, list);
        if (!ele_cur->value) {
            list_del(node);
            free(ele_cur);
            continue;
        }
        struct list_head *tmp_node = node->next;
        while (tmp_node != head) {
            element_t *ele_next = list_entry(tmp_node, element_t, list);
            tmp_node = tmp_node->next;
            if (strcmp(ele_cur->value, ele_next->value) == 0) {
                flag = 1;
                free(ele_next->value);
                ele_next->value = NULL;
            }
        }
        if (flag == 1) {
            list_del(node);
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
    if (!head) {
        return;
    }
    struct list_head *node;
    list_for_each (node, head) {
        if (node->next == head) {
            break;
        }
        list_move(node, node->next);
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head)) {
        return;
    }
    struct list_head *node, *safe;
    list_for_each_safe (node, safe, head) {
        struct list_head *tmp = node->next;
        node->next = node->prev;
        node->prev = tmp;
    }
    struct list_head *tmp = head->next;
    head->next = head->prev;
    head->prev = tmp;
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head || list_empty(head) || list_is_singular(head)) {
        return;
    }
    int len = q_size(head);
    for (struct list_head *cur = head->next, *node = head->next; cur && node;
         cur = node) {
        if (len < k)
            break;
        node = cur;
        struct list_head *start = node;
        node = node->next;
        for (int i = 1; i < k - 1; i++) {
            struct list_head *tmp = node->next;
            node->next = node->prev;
            node->prev = tmp;
            node = node->prev;
        }
        struct list_head *tmp1 = start->next;
        struct list_head *tmp2 = start->prev;
        start->next = node->next;
        start->prev = node->prev;
        node->next = tmp1;
        node->prev = tmp2;
        len -= k;
    }
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
    struct list_head left, *left_p = &left;
    INIT_LIST_HEAD(left_p);
    list_cut_position(left_p, head, slow);
    q_sort(head);
    q_sort(left_p);
    q_merge_two(head, left_p);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head)) {
        return 0;
    }
    if (list_is_singular(head)) {
        return 1;
    }
    struct list_head *tail = head->prev, *node = tail;
    while (node != head->next) {
        struct list_head *prev = node->prev;
        element_t *cur_ele = list_entry(node, element_t, list);
        element_t *prev_ele = list_entry(prev, element_t, list);
        if (strcmp(cur_ele->value, prev_ele->value) > 0) {
            list_del(prev);
            free(prev_ele->value);
            free(prev_ele);
        }
        node = node->prev;
    }
    return q_size(head);
}

/*Merge two queue into one sorted queue, which is in ascending order */
void q_merge_two(struct list_head *L1, struct list_head *L2)
{
    if (L1 && L2) {
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

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    if (!head || list_empty(head)) {
        return 0;
    }
    if (list_is_singular(head)) {
        return 1;
    }
    while (q_size(head) > 1) {
        struct list_head *tail = head->prev;
        for (struct list_head *start = head->next, *end = tail; start != end;
             start = start->next, end = end->prev) {
            queue_contex_t *qct_sta = list_entry(start, queue_contex_t, chain);
            queue_contex_t *qct_end = list_entry(tail, queue_contex_t, chain);
            q_merge_two(qct_sta->q, qct_end->q);
            qct_sta->size = q_size(qct_sta->q);
            qct_end->size = 0;
        }
    }
    queue_contex_t *qct_res = list_first_entry(head, queue_contex_t, chain);
    return qct_res->size;
}
