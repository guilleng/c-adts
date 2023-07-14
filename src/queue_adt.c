#include "queue_adt.h"

#define HALF 0.5
#define TWICE 2.0

/*********************************************************** Data Definitions */
/*
 * # Datatype completion
 *
 * A `QueueADT` object is:  
 *  + A dynamically allocated array of void pointers.
 *  + The index of the first item that arrived in the queue.
 *  + The index of the last item that arrived in the queue.
 *  + The number of elements currently in the queue.
 *  + The array's minimum size.
 *  + The array's current maximum size.
 *  + A flag that determines whether the queue is dynamic or not.
 */
struct queue_type
{
    Element *contents;
    size_t head;
    size_t tail;
    size_t nelems;
    size_t min_size;
    size_t curr_max_size;
    int is_fix;
};

/********************************************************** Private Functions */ 

/*
 * Returns non-zero if `q` is full
 */
static inline int is_full(QueueADT *q)
{
    return (q->nelems == q->curr_max_size);
}

/*
 * Returns non-zero if `q` is a fixed-size queue
 */
static inline int is_fix(QueueADT *q)
{
    return q->is_fix;
}


/* 
 * Copies into `new[]` `q->contents[]` shifting to lower indexes if necessary.
 */ 
static inline void shift_elements(QueueADT *q, Element new[])
{
    if (q->head > q->tail)
    {
        /*    <---                   --->            */
        /* | x | x |   |   |   |   | x | x | x | x | */
        /*       tl                  hd              */

        size_t hdbytes = sizeof(Element) * (q->curr_max_size - q->head);
        size_t tlbytes = sizeof(Element) * (q->tail + 1);

        memcpy(&new[0], &q->contents[q->head], hdbytes);
        memcpy(&new[q->curr_max_size - q->head], &q->contents[0], tlbytes);
    }
    else
    {
        /*                       --->    < ---
         * |   |   |   |   |   | x | x | x | x |   | */
        /*                       hd          tl      */

        size_t bytes = sizeof(Element) * (q->nelems);

        memcpy(&new[0], &q->contents[q->head], bytes);
    }
}

/*
 * Reallocates `q->contents[]` to an array half or twice the current size
 * depending upon factor.
 */
static inline Element *resize_contents_array(QueueADT *q, double factor)
{     
    Element *new; 
    size_t new_size; 

    /* Set new size */
    if (factor == TWICE)
    {
        new_size = q->curr_max_size * 2;
    }
    if (factor == HALF)
    {
        new_size = q->curr_max_size / 2;
    }

    /* Allocate new array */
    new = malloc(new_size * sizeof(Element));
    if (new == NULL)
    {
        perror("double_contents_size malloc failed allocating Element array: ");
        return NULL;
    }

    shift_elements(q, new);
    free(q->contents);

    /* Update internal values */
    q->contents = new;
    q->head = 0;
    q->tail = q->nelems - 1;
    q->curr_max_size = new_size;

    return q->contents;
}

/***************************************************** Public Implementations */

/*
 * Create non-circular (dynamic) queue
 */
QueueADT *cadtqueue_new(size_t size)
{
    QueueADT *new;

    if (size == 0)
    {
        errno = EPERM;
        perror("cadtqueue_new called with 0 size parameter: ");
        return NULL;
    }

    new = malloc(sizeof(struct queue_type));
    if (new == NULL)
    { 
        perror("cadtqueue_new malloc failed allocating struct queue_type: ");
        return NULL;
    }
    
    new->contents = malloc(size * sizeof(Element));
    if (new->contents == NULL)
    {
        perror("cadtqueue_new malloc failed allocating Element array: ");
        return NULL;
    }

    new->head = 0;
    new->tail = 0;
    new->nelems = 0;
    new->min_size = size;
    new->curr_max_size = size;
    new->is_fix = 0;

    return new;
}

/*
 * Create circular (fixed-size) queue
 */
QueueADT *cadtqueue_new_circular(size_t size)
{
    QueueADT *new = cadtqueue_new(size);
    if (new == NULL)
    {
        return NULL;
    }
    new->is_fix = 1;

    return new;
}

/*
 * Destroy queue
 */
void cadtqueue_destroy(QueueADT *q)
{
    free(q->contents);
    free(q);
    return;
}

/*
 * Return the number of elements `q` currently holds
 */
size_t cadtqueue_nelems(QueueADT *q)
{
    return q->nelems;
}

/*
 * Make `q` empty
 */
QueueADT *cadtqueue_clear(QueueADT *q)
{
    /* q has grown, make a new queue for resizing*/
    if (q->curr_max_size > q->min_size) 
    {                                    
        Element *new = malloc(q->min_size * sizeof(Element));
        if (new== NULL)
        {
            perror("cadtqueue_clear malloc failed allocating Element array: ");
            return NULL;
        }

        free(q->contents);
        q->contents = new;
    }
    /* q hasn't grown or is fixed in size */
    q->head = 0;
    q->tail = 0;
    q->nelems = 0;
    q->curr_max_size = q->min_size;
    return q;
}

/*
 * Return the first item in the queue without changing the queue
 */
Element cadtqueue_peek_first(QueueADT *q)
{
    /* handle queue underflow */
    if (cadtqueue_nelems(q) == 0)
    {
        errno = EPERM;
        return NULL;
    }

    return q->contents[q->head];
}

/*
 * Return the last item in the queue without changing the queue
 */
Element cadtqueue_peek_rear(QueueADT *q)
{
    /* handle queue underflow */
    if (cadtqueue_nelems(q) == 0)
    {
        errno = EPERM;
        return NULL;
    }

    return q->contents[q->tail];
}

/*
 * Append element to `q`
 */
Element cadtqueue_enqueue(QueueADT *q, Element e)
{
    /* handle queue overflow */
    if (is_fix(q) && is_full(q))
    {
        errno = EPERM;
        return NULL;
    }
    /* handle dynamic queue */
    if (!is_fix(q) && is_full(q))
    {
        if (resize_contents_array(q, TWICE) == NULL)
        {
            return NULL;
        }
    }

    /* update tail index (wrap-around) */ 
    if (q->nelems >= 1)
    {
        q->tail = (q->tail == q->curr_max_size - 1) ? 0 : (q->tail + 1);
    }

    q->contents[q->tail] = e;
    q->nelems++; 

    return e;
}

/*
 * Remove element at the front of `q`
 */
Element cadtqueue_dequeue(QueueADT *q)
{
    Element ret;
    double usage = (double) q->nelems / (double) q->curr_max_size;

    /* handle queue underflow */
    if (q->nelems == 0)
    {
        errno = EPERM;
        return NULL;
    }

    else if (!is_fix(q) && usage < 0.25)
    {
        /* make it small */;
        if (resize_contents_array(q, HALF) == NULL)
        {
            return NULL;
        }
    }

    /* get element at head */
    ret = q->contents[q->head];
    q->nelems--;

    /* update head index (wrap-around) */
    if (q->nelems != 0)
    {
        q->head = (q->head == q->curr_max_size - 1) ? 0 : (q->head + 1);
    }

    return ret;
}
