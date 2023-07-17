/* 
 * Sample code snippets. 
 * Basic usage of implemented data structures.
 */
#include "stack_adt.h"
#include "queue_adt.h"

void stack_example(void);
void queue_example(void);

int main(void)
{
    stack_example();
    queue_example();

    exit(EXIT_SUCCESS);
}

void stack_example(void)
{
    StackADT *s = cadtstack_new(2);

    /* Push elements onto the stack */
    cadtstack_push(s, (Element) "Element 1"); 
    cadtstack_push(s, (Element) "Element 2"); 
    cadtstack_push(s, (Element) "Element 3"); 
    cadtstack_push(s, (Element) "Element 4"); 

    /* Pop the top element of the stack */
    printf("Top element: %s\n", (char*) cadtstack_pop(s));

    /* Print number of elements in stack */
    printf("Elements in stack: %zu\n", cadtstack_nelems(s));

    /* Clear the stack */
    cadtstack_clear(s);

    /* Print error message because popping from empty stack */
    errno = 0;
    if (cadtstack_pop(s) == NULL && errno == EPERM)
    {
        fprintf(stderr, "Error, popping from emtpy stack!\n");
    }

    /* Clean up */
    cadtstack_destroy(s);
    printf("\n");
}

void queue_example(void)
{
    QueueADT *q = cadtqueue_new(1);

    /* Enqueue elements */
    cadtqueue_enqueue(q, (Element) "First");
    cadtqueue_enqueue(q, (Element) "Second");
    cadtqueue_enqueue(q, (Element) "Third");
    cadtqueue_enqueue(q, (Element) "Fourth");

    printf("Elements in queue: %zu\n", cadtqueue_nelems(q));

    /* Peeking elements at front and rear */
    printf("First element: %s\n", (char*) cadtqueue_peek_first(q));
    printf("Last element: %s\n", (char*) cadtqueue_peek_rear(q));

    /* Dequeuing */
    while(cadtqueue_nelems(q) > 0)
    {
        printf("Dequeued: %s\n", (char*) cadtqueue_dequeue(q));
    }

    /* Print error message because dequeuing from empty queue */
    errno = 0;
    if (cadtqueue_dequeue(q) == NULL && errno == EPERM)
    {
        fprintf(stderr, "Error, queue underflow!\n");
    }

    /* Clean up */
    cadtqueue_destroy(q);
    printf("\n");
}
