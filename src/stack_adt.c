#include "stack_adt.h"

/*********************************************************** Data Definitions */
/*
 * # Datatype completion
 *
 * A `StackADT` object is:  
 *  + A dynamically allocated array of void pointers.
 *  + The array's minimum size.
 *  + The array's current maximum size.
 *  + The index of its top element.
 *  + A flag that determines whether the stack is of fixed size or may possibly
 *    grow 
 */
struct stack_type
{
    Element *contents;
    size_t min_size;
    size_t curr_max_size;
    size_t top;
    int is_fix;
};

/**************************************************** Private Implementations */ 

/* 
 * Return non-zero if `s` is full
 */
static inline int is_full(StackADT *s)
{
   return (s->top == s->curr_max_size);
}

/* 
 * Tests whether `s` is variable or fixed
 */
static inline int is_fix(StackADT *s)
{
    return s->is_fix;
}

/***************************************************** Public Implementations */

/* 
 * Create variable size stack 
 */
StackADT *cadtstack_new(size_t size)
{
    StackADT *new;

    if (size == 0)
    {
        errno = EPERM;
        perror("cadtstack_new called with 0 size parameter: ");
        return NULL;
    }

    new = malloc(sizeof (struct stack_type));
    if (new == NULL)
    {
        perror("cadtstack_new malloc failed allocating struct stack_type: ");
        return NULL;
    }
    new->contents = malloc(size * sizeof(Element));

    if (new->contents == NULL)
    {
        perror("cadtstack_new malloc failed allocating Element: ");
        return NULL;
    }

    new->min_size = size;
    new->curr_max_size = size;
    new->top = 0;
    new->is_fix = 0;

    return new;
}

/* 
 * Create fixed-size stack
 */
StackADT *cadtstack_new_fix(size_t size)
{
    StackADT *new = cadtstack_new(size);
    if (new == NULL)
    {
        return NULL;
    }
    new->is_fix = 1;

    return new;
}

/*
 * Destroy stack
 */
void cadtstack_destroy(StackADT *s)
{
    free(s->contents);
    free(s);
    return;
}

/* 
 * Return the number of elements `s` currently holds
 */
size_t cadtstack_nelems(StackADT *s)
{
    return s->top;
}

/* 
 * Make `s` empty 
 */
StackADT *cadtstack_clear(StackADT **s)
{
    /* s has grown, make a new stack for resizing*/
    if ((*s)->curr_max_size > (*s)->min_size) 
    {                                    
        StackADT *new = cadtstack_new((*s)->min_size);
        if (new == NULL)
        {
            perror("cadtstack_clear (new object): ");
            return NULL;
        }
        cadtstack_destroy(*s);
        *s = new;
        return *s;
    }
    /* s hasn't grown or is fixed in size */
    else                                  
    {                                     
        (*s)->top = 0;
    }
    return *s;
}

/* 
 * Push operation 
 */
Element cadtstack_push(StackADT *s, Element e)
{
    /* handle stack overflow of fixed-size stack */
    if (is_fix(s) && is_full(s))     
    {
        errno = EPERM;
        return NULL; 
    }
    /* handle full variable-size stack */
    else if (!is_fix(s) && is_full(s))
    {
        Element *p = 
            realloc(s->contents, (s->curr_max_size *= 2) * sizeof(Element));
        if (p == NULL)
        {
            perror("push_ezstack (Realloc): ");
            return NULL;
        }
        s->contents = p;
    }

    s->contents[s->top++] = e;
    return e;
}

/* 
 * Pop operation 
 */
Element cadtstack_pop(StackADT *s)
{
    double usage = (double) s->top / (double) s->curr_max_size;

    /* handle stack underflow */
    if (s->top == 0)
    {
        errno = EPERM;
        return NULL;
    }

    /* handle shrinking case */
    if (!is_fix(s) && usage < 0.25)
    {
        Element *p = 
            realloc(s->contents, (s->curr_max_size /= 2) * sizeof(Element));
        if (p == NULL)
        {
            perror("push_ezstack (Realloc): ");
            return s->contents[--s->top];
        }
        s->contents = p;
    }
    return s->contents[--s->top];
}
