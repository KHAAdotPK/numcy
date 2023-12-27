/*
    numcy/Dimensions.hh
    Q@khaa.pk
 */

//#include "../ala_exception/ala_exception.hh"
//#include "../String/src/allocator.hh"
//#include "../String/src/string_character_traits.hh"

//#include "DimensionsOfArray.hh"
#include "header.hh"

#ifndef KHAA_PK_DIMENSIONS_HEADER_HH
#define KHAA_PK_DIMENSIONS_HEADER_HH

/*
    To summarize, in a C-style array represented by a linked list of DIMENSIONS structures:
    - Each structure represents a specific dimension of the multi-dimensional array.
    - The next and prev pointers allow traversal between the dimensions.
    - The last DIMENSIONS structure holds the dimensions of the innermost dimension of the array, and the columns member stores the number of elements or columns in that dimension.
    - The columns member in the last link is non-zero to indicate the size of the innermost dimension.
    - The prev pointer of the first link and the next pointer of the last link are both set to NULL to indicate the start and end of the linked list, respectively.
 */
typedef struct Dimensions
{    
    //private:
        cc_tokenizer::string_character_traits<char>::size_type columns;
        cc_tokenizer::string_character_traits<char>::size_type rows;
        struct Dimensions* next;
        struct Dimensions* prev;

    public:

    Dimensions (DIMENSIONSOFARRAY dimensionsOfArray) : columns(0), rows(0), next(NULL), prev(NULL) 
    {        
        if (dimensionsOfArray.size())
        {
            cc_tokenizer::string_character_traits<char>::size_type i = 1;
            Dimensions* current = NULL;

            while (i < (dimensionsOfArray.size() - 1))
            {
                if (current == NULL)
                {
                    rows = dimensionsOfArray[0];
                    next = reinterpret_cast<Dimensions*>(cc_tokenizer::allocator<char>().allocate(sizeof(DIMENSIONS)));
                    current = next;
                    current->rows = dimensionsOfArray[i];
                    current->columns = 0;
                    current->next = NULL;
                    current->prev = this;                    
                }
                else 
                {
                    current->next = reinterpret_cast<Dimensions*>(cc_tokenizer::allocator<char>().allocate(sizeof(DIMENSIONS)));
                    current->next->next = NULL;
                    current->next->prev = current;
                    current = current->next;

                    current->rows = dimensionsOfArray[i];
                    current->columns = 0;
                }

                i = i + 1;
            }

            current->columns = dimensionsOfArray[i];
        }
    }

    Dimensions(Dimensions& ref) : columns(ref.columns), rows(ref.rows), next(ref.next), prev(ref.prev)
    {

    }
        
    Dimensions(cc_tokenizer::string_character_traits<char>::size_type c, cc_tokenizer::string_character_traits<char>::size_type r, struct Dimensions* n, struct Dimensions* p) : columns(c), rows(r), next(n), prev(p)
    {
    }

    ~Dimensions()
    {         
        Dimensions* current = next;
        
        if (next != NULL)
        {
            while (1)
            {
                if (current->next == NULL)
                {
                    break;
                }

                current = current->next;
            }

            while (1)
            {
                Dimensions* local = current->prev;

                cc_tokenizer::allocator<char>().deallocate(reinterpret_cast<char*>(current));

                if (local == NULL || local == this)
                {
                    break;
                }

                current = local;
            }
        }
                
        /*if (next != NULL)
        {
            Dimensions* current = next;

            while (1)
            {
                Dimensions* local = current;
                current = current->next;

                cc_tokenizer::allocator<char>().deallocate(reinterpret_cast<char*>(local));

                if (current == NULL)
                {
                    break;
                }    
            }                
        }*/

        //std::cout<<"In the destructor...."<<std::endl;        
    }

    /*
        Creates copy of "instance reference"/"self reference" of type DIMENSIONS(It goes through the whole linkedlist and creates a copy).
        Returns pointer to newly created copy of the context.
     */
    Dimensions* copy(void)
    {
        Dimensions *ret = NULL, *current;
        cc_tokenizer::string_character_traits<char>::size_type n = getN();
        cc_tokenizer::allocator<char> alloc_obj;

        //std::cout<< "Size = " << this->getDimensionsOfArray().size() << std::endl;

        for (size_t i = 0; i < getDimensionsOfArray().size() - 1; i++)
        {
            if (ret == NULL)
            {
                ret = reinterpret_cast<Dimensions*>(alloc_obj.allocate(sizeof(Dimensions)));
                *ret = {0, getDimensionsOfArray()[i], NULL, NULL};
                current = ret;
            }
            else
            {
                current->next = reinterpret_cast<Dimensions*>(alloc_obj.allocate(sizeof(Dimensions)));
                *(current->next) = {0, getDimensionsOfArray()[i], NULL, current}; 

                current = current->next;
            }
        }

        current->columns = getDimensionsOfArray()[getDimensionsOfArray().size() - 1];

        return ret;
    }

    /*                
        This method, named 'getN', is designed to traverse a linked list of 'Dimensions' structures, each representing a dimension of a multidimensional array.
        It calculates and returns the total number of indices an array can hold, which essentially indicates the total number of elements within the array.        
     */
    cc_tokenizer::string_character_traits<char>::size_type getN(void)
    {
        cc_tokenizer::string_character_traits<char>::size_type n = 1;
        struct Dimensions* current = this;

        while (1)
        {
            if (current == NULL)
            {
                break;
            }

            n = current->rows * n;
        
            if (current->next == NULL)
            {
                n = current->columns * n;
            }

            current = current->next;
        }

        return n;
    }

    /*
        Please note, before calling this method make sure that linked list is properly formed.
        It simply returns the size of the inner most array(a.k.a the number of columns)
        All arrays are two dimensional(atleast), for example... array[10] is an array of 1 line and 10 columns array.
     */
    cc_tokenizer::string_character_traits<char>::size_type getNumberOfColumns(void)
    {
        cc_tokenizer::string_character_traits<char>::size_type n = 0;
        struct Dimensions* current = this;

        while (1)
        {
            if (current == NULL)
            {
                break;
            }

            n = current->columns;

            current = current->next;
        }

        return n;
    }

    /*
        "In the case of a single dimentional array, it returns 1" PLEASE NOTE: this was old behaviour. I modified a for loop in method DIMENSIONSOFARRAY::getNumberOfInnerArrays() 
        In the case of multidimentional array it wll eturn the number of inner arrays in number of single dimension arrays e.g [2][3][5] is 6 single dimensions array with each row having 5 columns 
     */
    DIMENSIONSOFARRAY getNumberOfRows(void)
    {
        return getDimensionsOfArray();
    }

    /*         
        Just an example of creating a linked list of DIMENSIONS
       ---------------------------------------------------------
        DIMENSIONS dim3 = {10, 3, NULL, NULL};
        DIMENSIONS dim2 = {0, 10, &dim3, NULL};
        dim3.prev = &dim2;
        DIMENSIONS dim1 = {0, 78, &dim2, NULL};
        dim2.prev = &dim1;
        DIMENSIONS dim = {0, 9, &dim1, NULL};
        dim1.prev = &dim;
        Returns compoite DIMENIONSOFARRAY, a struct data type. It returns the number of dimensions a array has and 
        the size of each dimension of the array, including the size of innermost array(a.k.a the number of columns) 
     */
    DIMENSIONSOFARRAY getDimensionsOfArray(void) throw (ala_exception)
    {
        cc_tokenizer::string_character_traits<char>::size_type n = getNumberOfLinks();

        /*
            The last link of linked list has number of columns of the inner most array, that is why this method would return atleast 2 as the dimensions of the array and it implies having both rows and columns in the last link of either multilink or single link list.
         */
        if (n != 0)
        {
            n = n + 1;
        }
        else
        {
            throw ala_exception("Dimensions::gteDimensionsOfArray() Error: Malformed shape of array, link count is 0");
        }

        cc_tokenizer::string_character_traits<char>::size_type *ptr = reinterpret_cast<cc_tokenizer::string_character_traits<char>::size_type*>(cc_tokenizer::allocator<char>().allocate(n*sizeof(cc_tokenizer::string_character_traits<char>::size_type)));

        cc_tokenizer::string_character_traits<char>::size_type i = 0;
        Dimensions* current = this;

        while (1)
        {
            ptr[i] = current->rows;

            if (current->next == NULL)
            {
                ptr[i + 1] = current->columns;

                break;
            }
            
            current = current->next;

            i = i + 1;
        }

        return {ptr, n};
    }

    /*        
        Returns number of links in a linked list and this number would atleast be 1 because each linked list has a head where the previous pointer is NULL.
     */
    cc_tokenizer::string_character_traits<char>::size_type getNumberOfLinks(void)
    {
        cc_tokenizer::string_character_traits<char>::size_type n = 0;
        struct Dimensions* current = this;

        while (current != NULL)
        {
            n++;
            current = current->next;                 
        }

        return n;
    }
    
    /*
        Overloaded [] operator
        @index, when it is 0(NUMCY_DIMENSIONS_SHAPE_ROWS), value of DIMENSIONS::rows is returned
                when it is 1(NUMCY_DIMENSIONS_SHAPE_COLUMNS), value of DIMENSIONS::columns is returned
     */   
    cc_tokenizer::string_character_traits<char>::size_type operator[](cc_tokenizer::string_character_traits<char>::size_type index) const
    {   
        cc_tokenizer::string_character_traits<char>::size_type ret = 0;

        switch (index) 
        {
            case NUMCY_DIMENSIONS_SHAPE_ROWS:
                ret = rows;
            break;
            case NUMCY_DIMENSIONS_SHAPE_COLUMNS:
                ret = columns;
            break;
        }

        return ret;     
    }

} DIMENSIONS;

typedef Dimensions* DIMENSIONS_PTR;
typedef const Dimensions* CONST_DIMENSIONS_PTR;

#endif