#ifndef QUEUE_ADT_H
#define QUEUE_ADT_H

/** @cond */
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
/** @endcond */
#include "common/data_types.h"

/** @cond */
typedef struct queue_type QueueADT;
/** @endcond */

/**
 * @brief Creates a _non-circular_ (dynamic) queue.
 *
 * Allocates a queue with a minimum size of `size` that behaves as follows:
 *  + __Doubles__ in size whenever _full_.
 *  + __Halves__ if usage falls _below 25%_ and the shrinking leaves it at a 
 *    size at least _equal to_ its original definition.
 *
 * If the size argument passed is zero, `errno` is set to `EPERM`. In case of 
 * failure to allocate memory `errno` is set to `ENOMEM`. For both cases the 
 * interpreted error message is outputted to `stderr`, and `NULL` is returned.
 *
 * @param size The number of elements for initialization.
 * @return Returns a `QueueADT` handle on success, `NULL` on failure.
 */
QueueADT *create_queueadt(size_t size);

/**
 * @brief Creates a _circular_ (fixed-size) queue.
 *
 * If the size argument passed is zero, `errno` is set to `EPERM`. In case of 
 * failure to allocate memory `errno` is set to `ENOMEM`. For both cases the 
 * interpreted error message is outputted to `stderr`, and `NULL` is returned.
 *
 * @param size The maximum number of items the queue allows.
 * @return Returns a `QueueADT` handle on success, `NULL` on failure.
 */
QueueADT *create_fixsize_queueadt(size_t size);

/**
 * @brief Deallocates a `QueueADT` object.
 *
 * @note Client-side is responsible for deallocating the memory in-use by all 
 *       elements of in `q`.  
 *
 * @param q The queue to deallocate.  
 * @return Returns no value. 
 */
void destroy_queueadt(QueueADT *q);

/**
 * @brief Returns the number of elements `q` currently holds.
 *
 * @param q The queue to check.  
 * @return Returns the number of elements currently held by `q`.  
 */
size_t nelems_in_queueadt(QueueADT *q);

/**
 * @brief Tests whether `q` is a dynamic or fixed-size queue.
 *
 * @param q The queue to check.  
 * @return Returns non-zero if `q` is circular (fixed-size), zero otherwise.
 */
int is_fix_queueadt(QueueADT *q);

/**
 * @brief Empties the queue pointed to by `queueptr`.
 *
 * @note Client-side is responsible for deallocating the memory in-use by the 
 *       elements of the queue `q`.  
 *
 * @param queueptr A pointer to the queue pointer to be emptied.  
 * @return Returns an `QueueADT` handle on success, `NULL` on failure.
 */
QueueADT *clear_queueadt(QueueADT **queueptr);

/**
 * @brief Returns the first item in the queue without changing the queue.
 *
 * If `q` is empty (__Queue underflow__), `NULL` is returned and `errno` is set 
 * to `EPERM`. 
 *
 * @param q The queue to peek from.
 * @return Returns an `Element` on success, `NULL` on failure.
 */
Element peek_first_queueadt(QueueADT *q);

/**
 * @brief Returns the last item in the queue without changing the queue.
 *
 * If `q` is empty (__Queue underflow__), `NULL` is returned and `errno` is set 
 * to `EPERM`. 
 *
 * @param q The queue to peek from.
 * @return Returns an `Element` on success, `NULL` on failure.
 */
Element peek_rear_queueadt(QueueADT *q);

/**
 * @brief Adds an element to the rear of `q`.
 *
 * The behavior of the operation depends upon the type of queue object being 
 * pushed to:  
 *  + If `q` is circular and there is no room to add `e` (__Queue overflow__), 
 *    `NULL` is returned and `errno` is set to `EPERM`.
 *  + If `q` is of dynamic, has no room for `e` and the system fails to 
 *    allocate memory `NULL` is returned and `errno` is set to `ENOMEM`. 
 *
 * @param q The queue to push to.
 * @param e The element to append to `q`.
 * @return Returns `e` on success, `NULL` on failure. 
 */
Element enqueue_queueadt(QueueADT *q, Element e);

/**
 * @brief Removes the element at the front of `q`.
 *
 * Returns and removes the front element in `q`. If `q` is empty (__Queue 
 * underflow__), `NULL` is returned and `errno` is set to `EPERM`. 
 *
 * If the queue is non-circular and its usage is below 25% before the dequeue 
 * operation, the queue size is halved before removing the item. 
 * If the reallocation of memory fails, `ENOMEM` is set, but the front element 
 * is still returned.
 *
 * @note Client-side is responsible for deallocating the memory in-use by the 
 *       elements of the queue `q`.  
 *
 * @param q The queue to dequeue from.
 * @return Returns an `Element` on success, `NULL` on underflow. 
 */
Element dequeue_queueadt(QueueADT *q);

#endif

/**
 * @file queue_adt.h
 *
 * An opaque data structure which represents a queue. It should only be
 * accessed through the `*_queueadt` functions.  
 *
 * @code{.c}
 * struct queue_type QueueADT
 * {
 *      // No available fields
 * }
 * @endcode
 *
 * @note See the html rendered version of the C code for the implementation of 
 * this module here: <a href="queue_adt_8c-example.html">queue_adt.c</a>.
 *
 * ---
 *
 * ### Key Points
 *  + Relies on `void` pointers to allow manipulating elements of any type. 
 *  + Uses `errno` for managing underflows/overflows.
 *  + Dynamically allocated. 
 *  + Clients can allocate __circular__ and __non-circular__ queues:
 *      + Circular queue (fixed-size): Predefined size that remains constant. 
 *      + Non-circular queue (dynamic/variable-size): Can dynamically adjust its 
 *        size based on the number of elements it holds.
 *
 * ### Considerations
 *  + Clients are responsible for managing the memory space of the objects 
 *    loaded to the structure.  
 *  + No type safety.
 *
 * To incorporate this data structure into one of your projects, just copy the 
 * header (`.h`), source file (`.c`), and the `/inlcude/common` folder.
 */
