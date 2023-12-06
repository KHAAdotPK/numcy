# NumCy C++ Library
`Numcy` is a `C++` library aimed at providing functionalities similar to the popular `Python` `NumPy` library. It offers a set of classes and functions to work with multi-dimensional arrays, perform mathematical operations, and support numerical computing in `C++`.

## Classes

## `DimensionsOfArray` Class
The `DimensionsOfArray` class is a utility class that helps manage dynamically allocated arrays for storing dimensions.
#### Example Usage:
```C++
// A three dimensional array, with six inner arrays, and then each inner array is a single line array(the innermost array)  of 10 columns. 
cc_tokenizer::string_character_traits<char>::size_type arrayA[] = {2, 3, 10};
DimensionsOfArray dimsA(array, sizeof(arrayA)/sizeof(cc_tokenizer::string_character_traits<char>::size_type));
// The dimensions of the array.
std::cout<< "Size = " << dimsA.size() << std::endl;
// Number of inner arrays.
std::cout<< dimsA.getNumberOfInnerArrays() << std::endl;

cc_tokenizer::string_character_traits<char>::size_type arrayB[] = {2, 3, 8};
DimensionsOfArray dimsB(arrayB, sizeof(arrayB)/sizeof(cc_tokenizer::string_character_traits<char>::size_type));    
if ( dimsB.compare(dimsA) ) 
{
    std::cout<< "They have same number of inner arrays..." << std::endl;
}
```

## `Dimensions` Class
The `Dimensions` class represents the shape and size of a multi-dimensional array. It holds information about the number of dimensions, size along each dimension, and pointers to previous and next `Dimensions` instances.
#### Example Usage:
```C++
// Manually initializing a doubly linkedlist...
//  dim -> dim1 -> dim2 -> dim3
DIMENSIONS dim3 = {10, 3, NULL, NULL};
DIMENSIONS dim2 = {0, 10, &dim3, NULL};
dim3.prev = &dim2;
DIMENSIONS dim1 = {0, 78, &dim2, NULL};
dim2.prev = &dim1;
DIMENSIONS dim = {0, 9, &dim1, NULL};
dim1.prev = &dim;
std::cout<< "Total number of inner arrays = " << dim.getDimensionsOfArray().getNumberOfInnerArrays() << std::endl;
std::cout<< "Total number of links = " << dim.getNumberOfLinks() << std::endl;
```

## `Collective` Class
The `Collective` class is a wrapper for multi-dimensional arrays. It encapsulates the data array along with its shape and provides convenient accessors and operations.
#### Example Usage:
```C++
float* data = new float[10];
Dimensions shape = {10, 1, NULL, NULL};
Collective<float> array(data, shape);
```
## `Numcy` Class
The `Numcy` class contains static methods that provide various mathematical and array operations, such as creating arrays filled with zeros, computing dot products, performing concatenation, and more.
### Example Usage:
```C++
Collective<float> zerosArray = Numcy::zeros<float>({0, 1, &shape, NULL});
```

## Getting Started
1. Include the `NumCy` library in your `C++` project. You can do this by adding the necessary source files to your project or linking against the compiled Numcy library.
2. Use the provided classes and functions to perform numerical computations similar to `NumPy` in `Python`. The library offers a range of functionalities, from creating multi-dimensional arrays to performing mathematical operations.
3. Reference the `NumCy` documentation for detailed information on each function. The documentation, although currently in progress, will include examples and explanations to help you understand and utilize the capabilities of the `NumCy` library effectively.
4. Explore the [USAGE.md](USAGE.md) file for a comprehensive example code that demonstrates the usage of various `NumCy` functions. This example code serves as a practical guide to implementing numerical computations using the `NumCy` library. 

## Contributing
If you find issues or have suggestions for improvement, feel free to contribute!

## License
This project is licensed under a license, the copy of which can be found in file - see the LICENSE file for details.
