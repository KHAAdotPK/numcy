/*
    numcy/numcy.hh
    Q@Khaa.pk
 */

#include "header.hh"

#ifndef REPLIKA_PK_NUMCY_HEADER_HH
#define REPLIKA_PK_NUMCY_HEADER_HH

/*
   Does not modify the internal state of the class within the most of the methods.
 */
class Numcy
{   
    public: 
        class Random
        {
            public:
            
                static void randn(DIMENSIONS& dim) throw (ala_exception)
                {
                    // TODO,
                    std::cout<< dim.getN() << std::endl;    
                }

                /*
                    Return random integers from low (inclusive) to high (exclusive).
                 */
                //static int* randint(int, int, DIMENSIONS shape = {1, 1, NULL, NULL}) throw(ala_exception);
                static int* randint(int low, int high, DIMENSIONS shape) throw (ala_exception)
                {
                    cc_tokenizer::string_character_traits<char>::size_type n = shape.getN();

                    if (n == 0)
                    {
                        throw ala_exception("Numcy::NumcyRandom::randint(): Malformed shape of return array");
                    }

                    cc_tokenizer::allocator<char> alloc_obj;

                    int* ptr = reinterpret_cast<int*>(alloc_obj.allocate(sizeof(int)*n));

                    int range = high - low;

                    for (cc_tokenizer::string_character_traits<char>::size_type i = 0; i < n; i++)
                    {        
                        ptr[i] = (low + (rand() % range));
                    }
    
                    return ptr;
                }
        };
            
        class Random random;

        /*
            @stop, integer or real; end of interval. The interval does not include this value, except in some cases where step is not an integer and floating point round-off affects the length of out.
            @step, integer or real, optional; spacing between values. For any output out, this is the distance between two adjacent values, out[i+1] - out[i]. The default step size is 1. If step is specified as a position argument, start must also be given.
            @like, array_like, optional; at the moment it is here for documentation purposes.
            Returns an array like the instance of DIMENSIONS passed
         */
        template <typename E = float, typename OutType = float>
        OutType* arange(E stop, E step = 1 /*NUMCY_DTYPE dtype = NUMCY_DTYPE_FLOAT*/, DIMENSIONS like = {1, 1, NULL, NULL}) const throw (ala_exception)
        {            
            return arange<E, OutType>(0, stop, step, like);
        }

        /*
            @start, integer or real; start of interval. The default start value is 0 and the interval includes this value.
            @stop, integer or real; end of interval. The interval does not include this value, except in some cases where step is not an integer and floating point round-off affects the length of out.
            @step, integer or real, optional; spacing between values. For any output out, this is the distance between two adjacent values, out[i+1] - out[i]. The default step size is 1. If step is specified as a position argument, start must also be given.
            @like, array_like, optional; at the moment it is here for documentation purposes.
            Returns an array like the instance of DIMENSIONS passed 
         */
        template <typename E = float, typename OutType = float>
        static OutType* arange(E start /*= 0*/, E stop, E step = 1 /*NUMCY_DTYPE dtype = NUMCY_DTYPE_FLOAT*/, DIMENSIONS like = {1, 1, NULL, NULL}) throw (ala_exception)
        {   
            if ((stop - start)/step > like.getN())
            {   
                unsigned int num = (stop - start)/step;
                throw ala_exception(cc_tokenizer::String<char>("Numcy::arange(): Range Size Error - The requested range size of ") + cc_tokenizer::String<char>(num) + cc_tokenizer::String<char>(" exceeds the requested size ") + cc_tokenizer::String<char>(like.getN()) + cc_tokenizer::String<char>(" of the to be allocated memory block's capacity to store the generated values."));
            }

            cc_tokenizer::allocator<char> alloc_obj;
            cc_tokenizer::string_character_traits<char>::size_type i = 0;
            E current = start;
            OutType* ptr = reinterpret_cast<OutType*>(alloc_obj.allocate(sizeof(OutType)*like.getN()));                                   

            while (i < like.getN() && current  < stop)
            {                
                ptr[i] = current;

                current = current + step;
                
                i++;
            }    
                                                
            return (OutType*) ptr;            
        }
        
        template<typename E>
        static struct Collective<E> concatenate(struct Collective<E> a, struct Collective<E> b, AXIS axis=AXIS_COLUMN) throw (ala_exception)
        {  
            E *ptr = NULL;
            struct Collective<E> ret = {NULL, {0, 0, NULL, NULL}};

            //a.shape.getDimensionsOfArray().compare(b.shape.getDimensionsOfArray());
            /*
            std::cout<<"a = is.Columns = " << std::endl;
            std::cout<< a.getShape().getNumberOfColumns() << std::endl;
            std::cout<< a.getShape().getNumberOfRows().getNumberOfInnerArrays() << std::endl;
            std::cout<<"b = pe.Columns " << std::endl;
            std::cout<< b.getShape().getNumberOfColumns() << std::endl;
            std::cout<< b.getShape().getNumberOfRows().getNumberOfInnerArrays() << std::endl;
             */
                        
            switch(axis)
            {                
                /*
                    -------------------------
                    | Stacking horizontally |
                    -------------------------
                    a[3][256]
                    b[1][3] 
                    After this we've an array [3][257]
                    
                    We need to check that number of rows in first array (a) is same as number of columns in second array (b).
                 */
                case AXIS_COLUMN:

                    // Validation of Shapes
                    if (a.getShape().getDimensionsOfArray().getNumberOfInnerArrays() != b.getShape().getNumberOfColumns())
                    {                        
                        throw ala_exception("Error in Collective::concatenate(AXIS_COLUMNS): Unable to concatenate instances of Collective composite. The number of rows in the first instance does not match the number of columns in the second instance.");
                    }

                    // Memory Allocation
                    try 
                    {
                        ptr = reinterpret_cast<E*>(cc_tokenizer::allocator<float>().allocate( a.getShape().getN() + b.getShape().getN() ));
                    }
                    catch (std::bad_alloc e)
                    {
                        throw ala_exception("Error in Error in Collective::concatenate(AXIS_COLUMNS): Caught std::bad_alloc.");     
                    }

                    for (cc_tokenizer::string_character_traits<char>::size_type i = 0; i < (a.getShape().getN() + b.getShape().getN()); i++)
                    {
                        ptr[i] = 0;
                    }

                    // Concatenation Loop
                    for (int i = 0; i < a.getShape().getDimensionsOfArray().getNumberOfInnerArrays(); i++)                    
                    {                        
                        for (int j = 0; j < a.getShape().getNumberOfColumns(); j++)
                        {                            
                            ptr[i*a.getShape().getNumberOfColumns() + (i + j)] = a[i*a.getShape().getNumberOfColumns() + j]; 
                        }
                        
                        ptr[i*a.getShape().getNumberOfColumns() + a.getShape().getNumberOfColumns() + i] = b[i];                        
                    }

                    ret = Collective<E>{ptr, {a.getShape().getNumberOfColumns() + 1, a.getShape().getDimensionsOfArray().getNumberOfInnerArrays(), NULL, NULL}};

                break;
            }
            
            return ret;
        }

        template<typename E>
        static struct Collective<E> cos(Collective<E> x) 
        {
            cc_tokenizer::allocator<char> alloc_obj;

            struct Collective<E> ret = {reinterpret_cast<E*>(alloc_obj.allocate(sizeof(E)*x.shape.getN())), {x.shape.getNumberOfColumns(), x.shape.getNumberOfRows().getNumberOfInnerArrays(), NULL, NULL}};

            for (cc_tokenizer::string_character_traits<char>::size_type i = 0; i < x.shape.getN(); i++)
            {
                ret[i] = std::cos(x[i]);
            }

            return ret;
        }
                
        /*
            The product of two matrices can be computed by multiplying elements of the first row of the first matrix with the first column 
            of the second matrix then, add all the product of elements. Continue this process until each row of the first matrix is multiplied 
            with each column of the second matrix.

            [2.3 3.4 1.2 1.0] * [2.3  8.9]  = [(2.3*2.3 + 3.4*1.0 + 1.2*9.0 + 1.0*3.2) (2.3*8.9 + 3.4*1.0 + 1.2*2.3 + 1.0*8.9)] = [22.69  35.53]
                                [1.0  1.0]
                                [9.0  2.3]
                                [3.2  8.9]                                           
         */
        /**
         * Calculate the dot product of two matrices represented by Collective objects.
         *
         * @tparam E        The element type of the matrices.
         * @param a         The first input matrix.
         * @param b         The second input matrix.
         * @return          A new Collective object representing the result of the dot product.
         * @throws ala_exception If the matrices have incompatible shapes for dot product.
         *
         * This function computes the dot product of two matrices by multiplying elements
         * of the first matrix's rows with the corresponding columns of the second matrix
         * and summing the results. The resulting matrix is returned as a new Collective object.
         *
         * Example:
         * Matrix A:
         *   [2.3 3.4 1.2 1.0]
         *   [1.0 1.0 9.0 2.3]
         *   [3.2 8.9 2.3 8.9]
         *
         * Matrix B:
         *   [2.3 8.9]
         *   [1.0 1.0]
         *   [9.0 2.3]
         *   [3.2 8.9]
         *
         * Resulting Dot Product:
         *   [22.69 35.53]
         *   [12.6  27.56]
         *   [24.07 98.81]
         *
         * @note The input matrices must have compatible shapes for dot product, i.e., the
         *       number of columns in the first matrix must match the number of rows in the second matrix.
         */
        template<typename E>
        static /*E**/ struct Collective<E> dot(Collective<E> a, Collective<E> b) throw (ala_exception)
        {
            //std::cout<<a.shape.rows<<", "<<a.shape.columns<<std::endl;

            if (a.shape.getNumberOfColumns() != b.shape.getNumberOfRows().getNumberOfInnerArrays())
            {
                throw ala_exception("Numcy::dot(): Incompatible shapes for dot product: Number of columns in the first matrix does not match the number of rows in the second matrix.");
            }                        
            /*
                Immutable capture list...
                ----------------------------
                Originaly this was the lambda's ca[ture list [a, b]. 
                It captures the variables a and b by value.
                This means that func has access to the values of a and b as they were when the lambda was created.
                With the above capture list, I was getting compile time errors...
                - "error C2662: 'DIMENSIONSOFARRAY Dimensions::getNumberOfRows(void)': cannot convert 'this' pointer from 'const DIMENSIONS' to 'Dimensions'"
                - "error C2662: 'cc_tokenizer::string_character_traits<char>::size_type Dimensions::getNumberOfColumns(void)': cannot convert 'this' pointer from 'const DIMENSIONS' to 'Dimensions &'"

                Then I change it to [&a, &b]...
                ----------------------------------
                If you need to modify a and b within the lambda, you should capture them by reference using [&a, &b] in the capture list.
                However, this would require that a and b are mutable in the outer scope.
             */                        
            std::function</*E*()*/struct Collective<E>()> func = [&a, &b]/* That is called capture list */() -> /*E**/ Collective<E>
            {                  
                cc_tokenizer::allocator<char> alloc_obj;
                E* ptr = reinterpret_cast<E*>(alloc_obj.allocate(sizeof(E)*a.shape.getNumberOfRows().getNumberOfInnerArrays()*b.shape.getNumberOfColumns()));
                memset(ptr, 0, sizeof(E)*b.shape.getNumberOfColumns());

                struct Collective<E> ret = {ptr, {b.shape.getNumberOfColumns(), a.shape.getNumberOfRows().getNumberOfInnerArrays(), NULL, NULL}};

                for (cc_tokenizer::string_character_traits<char>::size_type i = 0; i < b.shape.getNumberOfColumns(); i++)
                {                    
                    for (cc_tokenizer::string_character_traits<char>::size_type j = 0; j < a.shape.getNumberOfColumns(); j++)
                    {
                        ptr[i] = ptr[i] + a[j]*b[b.shape.getNumberOfColumns()*j + i];
                    }   
                }

                //return ptr;
                return ret;
            };
           
           return func();
           //return NULL;
        }

        /*
            Output array, element-wise exponential of x. This is a scalar if @a is a scalar
         */
        template<typename E>
        static E* exp(E* a, cc_tokenizer::string_character_traits<char>::size_type n)
        { 
            cc_tokenizer::allocator<char> alloc_obj;

            E* ret = reinterpret_cast<E*>(alloc_obj.allocate(n*sizeof(E)));

            for (cc_tokenizer::string_character_traits<char>::size_type i = 0; i < n; i++)
            {
                ret[i] = ::exp(a[i]);
            }
            //cc_tokenizer::allocator<char>

            return ret;
        }

        /*
            Return a new array of given shape and type, filled with ones.
            @dim, shape of the array to return
            @order, whether to store multi-dimensional data in row-major (C-style) or column-major (Fortran-style) order in memory
         */
        template<typename E = float>
        
        static Collective<E> ones(DIMENSIONS dim, REPLIKA_ROW_MAJOR_OR_COLUMN_MAJOR order = REPLIKA_ROW_MAJOR) throw (ala_exception)
        {
            E* ptr = NULL;        
            cc_tokenizer::string_character_traits<char>::size_type n = dim.getN();
            
            if (n == 0)
            {
                throw ala_exception("Numcy::ones() Error: Dimensional data is empty. Unable to proceed.");
            }

            //ptr = reinterpret_cast<E*>(cc_tokenizer::allocator<E>().allocate(n));
            ptr = cc_tokenizer::allocator<E>().allocate(n);

            for (cc_tokenizer::string_character_traits<char>::size_type i = 0; i < n; i++)
            {
                ptr[i] = 1;
            }
                    
            return {ptr, {dim.getNumberOfColumns() , dim.getDimensionsOfArray().getNumberOfInnerArrays(), NULL, NULL}};
        }

        template<typename E>
        static struct Collective<E> sin(Collective<E> x) 
        {
            cc_tokenizer::allocator<char> alloc_obj;

            struct Collective<E> ret = {reinterpret_cast<E*>(alloc_obj.allocate(sizeof(E)*x.shape.getN())), {x.shape.getNumberOfColumns(), x.shape.getNumberOfRows().getNumberOfInnerArrays(), NULL, NULL}};
            
            for (cc_tokenizer::string_character_traits<char>::size_type i = 0; i < x.shape.getN(); i++)
            {                
                ret[i] = std::sin(x[i]);
            }

            return ret;
        }

        /*
            Extracts the upper triangular part of a 2-dimensional array.
    
            @m: Input matrix or array, must be at least 2-dimensional.
            @k: Optional integer parameter specifying the diagonal below which values are zeroed out.
                Default value is 0, zeroing out values below the main diagonal. 
                For a positive integer k, it zeros out values below the k-th diagonal (above the main diagonal).
                For a negative integer k, it zeros out values below the -k-th diagonal (below the main diagonal).

            Here's an example to illustrate how Numcy::triu() works:

            ```C++    
            Numcy::triu(Numcy::ones<t>({3, 3, NULL, NULL}), 0, true);
            ```
            Output:
            1 1 1
            0 1 1
            0 0 1

            In this example, Numcy::triu() zeroed out the values below the main diagonal, resulting in the upper triangular part of the original matrix. 
            You can also specify a different value of k to zero out values below or above a different diagonal.
         */
        template<typename E>
        static Collective<E> triu(Collective<E>& m, int k = 0, bool verbose = false) throw (ala_exception)
        {             
            if (!m.getShape().getN() || !m.getShape().getDimensionsOfArray().getNumberOfInnerArrays())
            {
                throw ala_exception("Numcy::triu() Error: Dimensional data is empty. Unable to proceed.");
            }
                        
            // For arrays with ndim exceeding 2, triu will apply to the final two axes.
            cc_tokenizer::string_character_traits<char>::size_type rows = NUMBER_OF_ROWS_IN_INNER_MOST_ARRAY(m), columns = NUMBER_OF_COLUMNS_IN_INNER_MOST_ARRAYS(m);
                                    
            E* ptr = cc_tokenizer::allocator<E>().allocate(m.getShape().getN());
            for (cc_tokenizer::string_character_traits<char>::size_type i = 0; i < m.getShape().getN(); i++)
            {
                ptr[i] = m[i];
            }

            // Iterate over innermost arrays.            
            for (cc_tokenizer::string_character_traits<char>::size_type i = 0; i < (m.getShape().getN() / (rows*columns));  i++)
            {   
                // Pointer to the start of the innermost array in the modified memory location
                E* innerMostArray = ptr + (i*rows*columns);

                // Iterate over rows of the innermost array
                for (cc_tokenizer::string_character_traits<char>::size_type q = (k < 0) ? k*-1 : 0; q < rows; q++) 
                {   
                    // Zero out elements below the specified diagonal                                                                                                                                                                              
                    for (cc_tokenizer::string_character_traits<char>::size_type i =  0; ((i < (k + q)) && (i < columns)); i++)                               
                    {
                        innerMostArray[q*columns + i] = 0;
                    }                                                
                }
            }

            /*
                Debugging Output (Verbose Mode):
                This block of code generates detailed output for debugging purposes when 'verbose' mode is enabled.
                It prints the modified array, row by row, in a human-readable format, facilitating inspection and analysis.
                Please note that this code is intended for debugging only and provides detailed information about
                the modified array's structure. In the final production code, it will be removed to optimize performance
                and reduce unnecessary output.
             */
            if (verbose) 
            {
                for (cc_tokenizer::string_character_traits<char>::size_type i = 0; /*(i < n)*/ i < (m.getShape().getN() / (rows*columns)); i++)
                {             
                    E* innerMostArray = ptr + (i*rows*columns); 
                    for (cc_tokenizer::string_character_traits<char>::size_type q = 0; q < rows; q++)
                    {
                        for (cc_tokenizer::string_character_traits<char>::size_type r = 0; r < columns; r++)
                        {
                            std::cout<<innerMostArray[q*columns + r]<<" ";                        
                        }
                        std::cout<<std::endl;
                    }                                 
                }
            }

            return Collective<E>{ptr, {m.getShape().getNumberOfColumns(), m.getShape().getDimensionsOfArray().getNumberOfInnerArrays(), NULL, NULL}};
        }
                                                                
        /*            
            Function: zeros
    
            Description:
            This static template function is used to create and initialize a one-dimensional array of a specified data type (default is float) with zeros.
    
            Parameters:
            - shape: An object representing the dimensions of the desired array.
            - columnsOrRowMajor: An optional parameter specifying whether the array should be arranged in row-major or column-major order (default is row-major).
    
            Returns:
            - E*: A pointer to the dynamically allocated array of type E (float by default), initialized with zeros.
    
            Throws:
            - ala_exception: If the shape provided has a size of zero, indicating a malformed array request.
    
            Working:
            1. It first determines the size 'n' of the desired array using the 'shape' object.
            2. It allocates memory for the array using the custom allocator 'alloc_obj'.
            3. It initializes the allocated memory with zero values.
            4. Finally, it returns a pointer to the created array.
    
            Note:
            - This function is typically used to create arrays filled with zeros for numerical computations.
            - The 'columnsOrRowMajor' parameter allows specifying the memory layout of the array (row-major or column-major) for advanced use cases.
         */
        template <typename E = float>
        static Collective<E> zeros(DIMENSIONS shape, REPLIKA_ROW_MAJOR_OR_COLUMN_MAJOR columnsOrRowMajor = REPLIKA_ROW_MAJOR) throw (ala_exception)
        {
            cc_tokenizer::string_character_traits<char>::size_type n = shape.getN();
            E* ptr = NULL;
            
            // n should not be zero
            if (n == 0)
            {
                throw ala_exception("Numcy::zeros(): Malformed shape of return array");
            }
            
            cc_tokenizer::allocator<E> alloc_obj;
            ptr = alloc_obj.allocate(n);

            for (cc_tokenizer::string_character_traits<char>::size_type i = 0; i < n; i++)
            {
                ptr[i] = 0x00;
            }
            
            return Collective<E>(ptr, shape.copy());
        }

        template <typename E = float>
        static E* zeros_old(DIMENSIONS shape, REPLIKA_ROW_MAJOR_OR_COLUMN_MAJOR columnsOrRowMajor = REPLIKA_ROW_MAJOR) throw (ala_exception)
        {
            cc_tokenizer::string_character_traits<char>::size_type n = shape.getN();
            E* ptr = NULL;

            // n should not be zero
            if (n == 0)
            {
                throw ala_exception("Numcy::zeros(): Malformed shape of return array");
            }
            
            cc_tokenizer::allocator<char> alloc_obj;
            ptr = reinterpret_cast<E*>(alloc_obj.allocate(sizeof(E)*n));

            for (cc_tokenizer::string_character_traits<char>::size_type i = 0; i < n; i++)
            {
                ptr[i] = 0x00;
            }

            return ptr;
        }        
};

#endif