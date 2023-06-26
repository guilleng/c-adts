/* 
 * Sample code snippets of basic usage for the data structures in the library. 
 */
#include "stack_adt.h"

int main(void)
{
 /*************************************************************** Stack usage */
    {
        StackADT s = create_stackadt(2);

        /* Push elements onto the stack */
        push_stackadt(s, "Element 1"); 
        push_stackadt(s, "Element 2"); 
        push_stackadt(s, "Element 3"); 
        push_stackadt(s, "Element 4"); 

        /* Pop the top element of the stack */
        printf("Top element: %s\n", (char*) pop_stackadt(s));

        /* Print number of elements in stack */
        printf("Elements in stack: %zu\n", nelems_of_stackadt(s));

        /* Clear the stack */
        make_empty_stackadt(&s);

        /* Print error message because popping from empty stack */
        if (pop_stackadt(s) == NULL)
        {
            fprintf(stderr, "Error, popping from emtpy stack!\n");
        }

        /* Clean up the stack */
        destroy_stackadt(s);
    }

    exit(EXIT_SUCCESS);
}
