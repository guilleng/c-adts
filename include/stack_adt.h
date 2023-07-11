#ifndef ADT_STACK_H
#define ADT_STACK_H

/** @cond */
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
/** @endcond */
#include "common/data_types.h"

/** @cond */
typedef struct stack_type StackADT;
/** @endcond */

/**
 * @brief Creates a variable-size stack.
 *
 * Allocates a stack with a minimum size of `size` that behaves as follows:
 *  + __Doubles__ in size whenever _full_.
 *  + __Halves__ if usage falls _below 25%_ and the shrinking leaves it at a 
 *    size at least _equal to_ its original definition.
 *
 * If the size argument passed is zero, `errno` is set to `EPERM`. In case of 
 * failure to allocate memory `errno` is set to `ENOMEM`. For both cases the 
 * interpreted error message is outputted to `stderr`, and `NULL` is returned.
 *
 * @param size The number of elements for initialization.
 * @return Returns a `StackADT` handle on success, `NULL` on failure. 
 */
StackADT *cadtstack_new(size_t size);

/**
 * @brief Creates a fixed-size stack.
 *
 * If the size argument passed is zero, `errno` is set to `EPERM`. In case of 
 * failure to allocate memory `errno` is set to `ENOMEM`. For both cases the 
 * interpreted error message is outputted to `stderr`, and `NULL` is returned.
 *
 * @param size The maximum number of items the stack allows.
 * @return Returns a `StackADT` handle on success, `NULL` on failure.
 */
StackADT *cadtstack_new_fix(size_t size);

/**
 * @brief Deallocates a `StackADT`.
 *
 * @note Client-side is responsible for deallocating the memory in-use by all 
 *       elements in `s`.  
 *
 * @param s The stack to deallocate.  
 * @return Returns no value. 
 */
void cadtstack_destroy(StackADT *s);

/**
 * @brief Returns the number of elements `s` currently holds.
 *
 * @param s The stack to check.  
 * @return Returns the number of elements currently held by `s`.  
 */
size_t cadtstack_nelems(StackADT *s);

/**
 * @brief Empties the stack pointed to by `stackptr`.
 *
 * @note Client-side is responsible for deallocating the memory in-use by all 
 *       elements in `s`.  
 *
 * @param stackptr A pointer to the stack pointer to be emptied. 
 * @return Returns a `StackADT` handle on success, `NULL` on failure.
 */
StackADT *cadtstack_clear(StackADT **stackptr);

/**
 * @brief Pushes an `Element` into a stack.
 *
 * The behavior of the operation depends upon the type of stack object being 
 * pushed to:  
 *  + If `s` is of fixed-size and there is no room to add `e` 
 *    (__Stack overflow__), `NULL` is returned and `errno` is set to `EPERM`.
 *  + If `s` is of variable-size with no room for `e` and the system fails to 
 *    allocate memory `NULL` is returned and `errno` is set to `ENOMEM`. 
 *
 * @param s The stack to push to.
 * @param e The element to push into `s`.
 * @return Returns `e` on success, `NULL` on failure. 
 */
Element cadtstack_push(StackADT *s, Element e);

/**
 * @brief Pops an item from a stack.
 *
 * Returns and removes the last element added to it. If `s` is empty (__Stack 
 * underflow__), `NULL` is returned and `errno` is set to `EPERM`. 
 *
 * + If `s` is a variable-size stack and its usage is below 25% before the pop 
 *   operation, the stack size is halved before popping the item. 
 * + If the reallocation of memory fails, `ENOMEM` is set, but the top element 
 *   is still popped from the stack.
 *
 * @note Client-side is responsible for deallocating the memory in-use by all 
 *       elements in `s`.  
 *
 * @param s The stack to pop from.
 * @return Returns `NULL` on underflow, an `Element` otherwise. 
 */
Element cadtstack_pop(StackADT *s);

#endif

/**
 * @file stack_adt.h
 *
 * @code{.c}
 * struct stack_type StackADT
 * {
 *      // No available fields
 * }
 * @endcode
 *
 * @note See the html rendered version of the C code for the implementation of 
 * this module here: <a href="stack_adt_8c-example.html">stack_adt.c</a>.
 *
 * ---
 *
 * ### Key Points
 *  + Relies on `void` pointers to allow manipulating elements of any type. 
 *  + Uses `errno` for managing stack underflows/overflows. 
 *  + Dynamically allocated. 
 *  + Stack object size can be __fixed__ or __variable__.  
 *
 * ### Considerations
 *  + Clients are responsible for managing the memory space of the objects 
 *    loaded to the structure.  
 *  + No type safety.   
 *
 * To incorporate this data structure into one of your projects, just copy the 
 * header (`.h`), source file (`.c`), and the `/inlcude/common` folder.
 */
