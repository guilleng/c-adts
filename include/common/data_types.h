/**
 * @file data_types.h
 * @brief A "global" typedef for a `void` pointer, common to all interfaces.
 *
 * @note Loss of Type Safety
 *
 * The main caveat of the `typedef void* Element` declaration is the loss of 
 * type safety. The implementation cannot verify if the operations performed on 
 * `Element` are valid for the actual data type being pointed to.
 *
 * As a result, the developer is responsible for the type and operations
 * performed in the data held in the objects.  
 *
 * When using `Element`, be aware of the following considerations:
 *
 * + Typecasting:  
 *  Explicit typecasting is necessary when assigning or dereferencing an 
 *  `Element` pointer. Failure to perform appropriate typecasting can lead to 
 *  incorrect data interpretation or __undefined behavior__.
 *
 * + Memory Management:  
 *  As `Element` can represent __any data type__, proper memory management 
 *  becomes the developer's responsibility. Ensure that memory allocation, 
 *  deallocation, and reallocation are performed correctly based on the actual 
 *  data type.
 *
 * + Validation:  
 *  Without type safety, it is essential to validate the underlying data before 
 *  performing operations or accessing its properties. Carefully check the data 
 *  type and integrity to prevent unexpected behavior or errors.
 *
 */

#ifndef ADT_DATA_TYPES_H
#define ADT_DATA_TYPES_H

/**
 * @brief Defines a generic `void` pointer type named `Element`.
 *
 * It allows for flexible data manipulation by providing a mechanism to store 
 * pointers to different types of elements. 
 */
typedef void* Element;

#endif
