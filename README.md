# NumCy C++ Library
`Numcy` is a `C++` library aimed at providing functionalities similar to the popular `Python` `NumPy` library. It offers a set of classes and functions to work with multi-dimensional arrays, perform mathematical operations, and support numerical computing in `C++`.

## Classes

## `Dimensions` Class
The `Dimensions` class represents the shape and size of a multi-dimensional array. It holds information about the number of dimensions, size along each dimension, and pointers to previous and next `Dimensions` instances.
#### Example Usage:
```C++
Dimensions dim = {10, 2, NULL, NULL};
```

## `DimensionsArray` Class
The `DimensionsArray` class is a utility class that helps manage dynamically allocated arrays for storing dimensions.
#### Example Usage:
```C++
DimensionsArray dimsArray(3); // Create an array for 3 dimensions
Dimensions* dims = dimsArray.allocate(); // Allocate memory for a Dimensions instance
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
