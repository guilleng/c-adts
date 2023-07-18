# C ADTs

A basic C data structure library.  

## Implemented Data Structures

+ Stack
+ Queue

## Table of Contents

+ [Project Motivation](#project-motivation)
+ [Installation and Usage](#installation-and-usage)
+ [Basic Usage](#basic-usage)
+ [Contributing](#contributing)
    + [Testing](#unit-testing)
    + [Documentation](#documentation)

---

### Project Motivation

The primary motivation for building this project is to foster personal growth
and learning in the areas of unit testing and documentation generation.
While the project may not be novel or extensive, it serves as a small practical 
platform to delve into these concepts. Additionally, I anticipate the 
opportunity to reuse this code in future projects.

### Installation and Usage

To incorporate one of the data structures of the library into a project, copy 
the header (interface) and source file (implementation) corresponding to the 
desired data structure from the `/include` and `/src` folders. 
Then, either add a `typedef` for the `Element` type or incorporate the folder 
`/include/common` too.

`main.c` contains code snippets that demonstrate the usage of various data 
structures provided by the library through function calls.

### Contributing

Feedback and contributions are appreciated and welcome. Feel free to report 
bugs, suggest enhancements, or submit a pull request. 

The implementations rely on a simple `makefile` that streamlines the building, 
testing, and generating documentation process. 

+ `make tests`: Builds and executes __all__ unit tests silently. 

+ `make test_%`: This rule allows you to test a specific unit by specifying 
  its name, displaying the tests results.  

For example, running `make test_stack_adt_priv` builds, executes and displays 
the results for the tests suite designed for the stack implementation file. This 
targeted testing approach enables efficient debugging and validation of 
individual units. See [Unit Testing](#unit-testing) for more details.  

#### Unit Testing

The project integrates the [minunit](https://github.com/siu/minunit) testing 
framework, which consists of only a header file, making it lightweight and easy 
to use.

The testing process involves two distinct strategies:

+ __Black Box Testing__  
    Focuses on the external behavior of the system without considering its 
    internal implementation details. Its primary objective is to validate the 
	functionality of the system from a user's perspective. Tests files that
    target this approach end with the suffix `_publ`.

+ __White Box Testing__  
    White box testing, on the other hand, examines the internal structure and
    implementation details of the system. It involves testing individual 
    components, functions, or modules to ensure their correctness and 
    robustness. Test files appended the suffix `_priv` contain tests that 
    take this approach.  

####  Documentation 

The de facto standard for generating documentation, 
[Doxygen](https://www.doxygen.nl/index.html) is integrated into this project 

The generated documentation focuses on the interfaces of the implemented data 
structures, providing comprehensive information about the usage and 
functionality of the code.  

If you wish to delve deeper, comments within the source files and test files 
serve to describe relevant implementation details when deemed necessary.  

The rationale behind this documenatation layout is to keep it somewhat simple, 
avoid comment-cluttering and set a clear distinction between the interface's 
functionality and the internal workings of the implementation.
