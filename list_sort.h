#pragma once
#include "list.h"

/*
 * This is an implementation of linux list_sort() function.
 * It is a stable sort, which means that the relative order of
 * elements that compare equal is preserved.
 */

int cmp(void *priv, const struct list_head *a, const struct list_head *b);

typedef int
    __attribute__((nonnull(2, 3))) (*list_cmp_func_t)(void *,
                                                      const struct list_head *,
                                                      const struct list_head *);

__attribute__((nonnull(2, 3))) void list_sort(void *priv,
                                              struct list_head *head,
                                              list_cmp_func_t cmp);