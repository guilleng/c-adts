# C ADTs

A basic C data structure library.  

## Implemented Data Structures

+ Stack

---

## Table of Contents

+ [Installation](#installation)
+ [Basic Usage](#basic-usage)
+ [Contributing](#contributing)
+ [Project Motivation](#project-motivation)
    + [Testing](#unit-testing)
    + [Documentation](#documentation)

---

### Installation

To incorporate one of the data structures of the library into a project, copy 
the header (interface) and source file (implementation) corresponding to the 
desired data structure. Then, either add a `typedef` for the `Element` type or
incorporate the folder `/include/common` too.

### Basic Usage

`main.c` contains code snippets that demonstrate the usage of various data 
structures provided by the library through function calls.

### Contributing

The application relies on a simple `makefile` that streamlines the building, 
testing, and generating documentation process. 

+ `make all`: This rule is self-explanatory.  

+ `make docs`: Invoking this rule generates HTML documentation using 
[Doxygen](https://www.doxygen.nl/index.html). Details [here](#documentation).  

+ `make tests`: Invoking this rule builds and executes all unit tests associated 
  with the project.  

+ `make test_%`: This rule allows you to test a specific unit by specifying 
  its name.  

For example, running `make test_stack_adt_priv` builds, executes and displays 
the results for the tests suite designed for the stack implementation file. This 
targeted testing approach enables efficient debugging and validation of 
individual units. See [Unit Testing](#unit-testing) for more details.  

### Project Motivation

The primary motivation for building this project is to foster personal growth
and learning in the areas of unit testing and Doxygen documentation. While the
project may not be novel or extensive, it serves as a practical platform to 
delve into the concepts and practices of unit testing and leveraging Doxygen for 
code documentation. 

I hope that the knowledge acquired through this project will serve as a
valuable asset for future endeavors, allowing for growth and reuse of skills 
and insights.

#### Unit Testing

The project integrates a simple testing framework called 
[minunit](https://github.com/siu/minunit), which consists of only a header 
file, making it lightweight and easy to use.

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

I have integrated [Doxygen](https://www.doxygen.nl/index.html) into this project 
to familiarize myself with its basics. The generated documentation focuses on
the interface, providing comprehensive information about the usage and
functionality of the code.  

If you wish to delve deeper, comments within the source files and test files 
serve to describe relevant implementation details when deemed necessary.  

The rationale behind this is to keep it somewhat simple, avoid 
comment-cluttering and set a clear distinction between the interface's 
functionality and the internal workings of the implementation.
