#pragma once

#include <stdint.h>
typedef uint8_t bit_item_t;

#define BIT_ITEM_SIZE (8 * sizeof(bit_item_t))
#define GET_BIT_SET_SIZE(size) ((size + BIT_ITEM_SIZE - 1) / BIT_ITEM_SIZE)
#define IS_BIT_SET(set, index) (set[index / BIT_ITEM_SIZE] & (1 << (index % BIT_ITEM_SIZE)))
#define SET_BIT(set, index) set[index / BIT_ITEM_SIZE] |= (1 << (index % BIT_ITEM_SIZE))
#define UNSET_BIT(set, index) set[index / BIT_ITEM_SIZE] &= ~(1 << (index % BIT_ITEM_SIZE))
