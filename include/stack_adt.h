/**
 * @file stack_adt.h
 * @brief A simple ADT `stack` module. 
 *
 * ## Key Points
 *  + Relies on `void` pointers to allow manipulating elements of any type. 
 *  + Uses `errno` for managing stack underflows/overflows. 
 *  + Dynamically allocated. 
 *  + Stack object size can be __fixed__ or __variable__.  
 *
 * ## Nuisances
 *  + Clients are responsible for managing the memory space of the objects 
 *    loaded to the structure.  
 *  + No type safety.   
 */

#ifndef ADT_STACK_H
#define ADT_STACK_H

/** @cond */
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
/** @endcond */
#include "common/data_types.h"

/**
 * @brief Incomplete type definition. Implementation details for this `struct` 
 * are documented in the source file for this data structure. 
 */
typedef struct stack_type* StackADT;

/**
 * @brief Creates a variable-size stack.
 *
 * Allocates a stack with a minimum size of `size` that behaves as follows:
 *  + __Doubles__ in size whenever _full_.
 *  + __Halves__ if usage falls _below 25%_ and the shrinking leaves it at a 
 *    size at least _equal to_ its original definition.
 *
 * In case of failure to allocate memory `errno` will be set accordingly and 
 * the interpreted error message outputted to `stderr`.  
 *
 * @param size The number of elements for initialization.
 * @return Returns an `StackADT` object on success, `NULL` on failure setting 
 *         `errno` to `ENOMEM`
 */
StackADT create_stackadt(size_t size);

/**
 * @brief Creates a fixed-size stack.
 *
 * In case of failure to allocate memory `errno` will be set accordingly and 
 * the interpreted error message outputted to `stderr`.  
 *
 * @param size The maximum number of items the stack allows.
 * @return Returns an `StackADT` object on success, `NULL` on failure setting 
 *         `errno` to `ENOMEM`
 */
StackADT create_fixsize_stackadt(size_t size);

/**
 * @brief Deallocates a `StackADT` object.
 *
 * @note Client-side is responsible for deallocating the memory in-use by the 
 *       elements of the stack `s`.  
 *
 * @param s The object stack to deallocate.  
 * @return Returns no value. 
 */
void destroy_stackadt(StackADT s);

/**
 * @brief Returns the number of elements `s` currently holds.
 *
 * @param s The stack to check.  
 * @return Returns the number of elements currently held by `s`.  
 */
size_t nelems_of_stackadt(StackADT s);

/**
 * @brief Tests whether the size of `s` size is variable or fixed.
 *
 * @param s The stack to check.  
 * @return Returns zero if `s` is of variable size, non-zero otherwise.  
 */
int is_fix_stackadt(StackADT s);

/**
 * @brief Empties the stack pointed to by `stackptr`.
 *
 *
 * @param stackptr A pointer to the stack to empty.  
 * @return Returns an `StackADT` object on success, `NULL` on failure.
 */
StackADT make_empty_stackadt(StackADT *stackptr);

/**
 * @brief Pushes an element into a stack.
 *
 * The behavior of the operation depends upon the type of stack object being 
 * pushed to:  
 *  + If s is of fixed-size and there is no room to add `e` 
 *    (__Stack overflow__), `NULL` is returned and `errno` is set to `EPERM`.
 *  + If `s` is of variable-size with no room for `e` and the system fails to 
 *    allocate memory `NULL` is returned and `errno` is set to `ENOMEM`. 
 *
 * @param s The stack to push to.
 * @param e The element to push into `s`.
 * @return Returns `e` on success, `NULL` on failure. 
 */
Element push_stackadt(StackADT s, Element e);

/**
 * @brief Pops an item from a stack.
 *
 * Returns and removes the last element added to it. If `s` is empty (__Stack 
 * underflow__), `NULL` is returned and `errno` is set to `EPERM`. 
 *
 * If the stack is a variable-size stack and its usage falls below 25% before 
 * the pop operation, the stack size is halved before popping the item. 
 * If the reallocation of memory fails, `ENOMEM` is set, but the top element is 
 * still popped from the stack.
 *
 * @param s The stack to pop from.
 * @return Returns an `Element` on success, `NULL` on underflow. 
 */
Element pop_stackadt(StackADT s);

#endif
