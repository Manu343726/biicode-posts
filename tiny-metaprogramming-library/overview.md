
# A Tiny Metaprogramming Library

It seems people like template metaprogramming. After three successful blog posts about tmp - with 5k views on average each one - I'm sure people like and even want to understand that obscure corner of C++. 

It's not a funny way to play with the compiler only, **template metaprogramming is a powerful tool for C++ developers and something that many of us must deal with everyday.**  

## Why?

C++ is a great language to write applications in, but it's even better to write libraries. You can write abstract, natural (readable) and performant generic APIs.  
To me the best example is the `vector` template (The algebraic vector, not the C++ `std::vector` one):

``` cpp
template<typename T>
struct vector3
{
    vector3() : vector3{0, 0, 0}
    {}
    
    vector3(T xx, T yy, T zz) : 
        x{xx},
        y{yy},
        z{zz}
    {}

    vector3& operator+=(const vector3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;

        return *this;
    }

    friend vector3 operator+(const vector3& lhs, const vector3& rhs)
    {
        //Want speed, pass by value? 
        //I prefer to have a clear interface, 
        //but that's another topic...
        vector3 cpy{lhs}; 
        
        return cpy += rhs;
    }

    friend bool operator==(const vector3& lhs, const vector3& rhs)
    {
        return std::tie(lhs.x,lhs.y,lhs.z) == std::tie(rhs.x,rhs.y,rhs.z);
    }
};
```

Using that `vector3` template is easy and it resembles algebraic notation:

``` cpp
vector3<int> a{1,2,3}, b{4,5,6};
vector3<int> c = a + b;
bool eq = c == vector3<int>{5,7,9};
```

The same for a hypothetical `matrix` template:

``` cpp
matrix<int,2,2> m{ {1,2}, {3,4} };

m.row(0) = m.row(0)*4 + m.row(1);
``` 

*Bjarne Stroupstrup does a great in-depth description of the design of this kind of matrix template in his book "The C++ Programming Language, 4th Edition"*

Compare that with the *pure OO* alternative of other languages:

``` java
Matrix<int> a = new Matrix<int>(2,2, 1,2,3,4);
Matrix<int> b = new Matrix<int>(2,2, 5,6,7,8);
Matrix<int> c = new Matrix<int>(2,2);

c = a.add(b);
a.row(0) = a.row(0).mul(4).add(a.row(0));
```

There are a few points that should be noted here:

#### Don't leave to runtime decisions based on properties you know at compile-time

In general that only serves to increase runtime overhead. 

In the matrix example, the matrix dimensions are known at compile time. Despite having some kind of [VLA](http://en.wikipedia.org/wiki/Variable-length_array) allocated on the stack, it hasn't got much sense to introduce dynamic memory allocation here, with all its cache misses, alloc/dealloc/memory-footprint overheads, etc.

Consider also the matrix's design. How you would implement this?  
What comes to my mind is a `matrix_slice` class with all the algebraic operations between matrix slices, where a slice is only a portion of a matrix (Maybe `matrix_view` is a common name for this thing with non-owning semantics).  
The key point here is that a matrix row can bee seen as a slice, a submatrix can be seen as a slice, and even a matrix can be seen as a slice. Implement matrix ops only in one site (the `slice`) then propagate them along your implementation via composition, inheritance, type aliasing, whatever you like.

Even if we decide to use the inheritance approach (`matrix` inherits from `slice`, `row` inherits from `slice`, etc), C++ has great (crazy) things to achieve this in a performant way, like [CRTP](http://en.wikipedia.org/wiki/Curiously_recurring_template_pattern); or going further enough, using the classic dynamic dispatch [the right way](http://bannalia.blogspot.com.es/2014/05/fast-polymorphic-collections.html).

**Performance matters. C++ is a language for performance.** If that was not the case, I would be using python or even ruby. But we are here because we need to squeeze down each CPU cicle. Nowadays that performance is only provided when understanding how hardware works and giving enough information to your compiler. C++ does both thing pretty well, or at least it gives you the opportunity to do them properly.   

Don't throw away those opportunities writing oh-my-runtime designs. Use your type system (literally); understand your compiler capabilities.

#### OO is not the 42 of programming

"*Everything must be an object, there are only classes, classes with their methods. Model your system with objects talking each other"* That's **The Big Lie Of Object Oriented Programming**.

You can't model every system with objects only. There are some actors in that system, abstracts things that cannot be represented easily as an "*object*". A function is a function, don't force it to be something else. I want a set of functions, not a class full of static methods. It's true, it's `std::cos()` vs `Math.cos()` only, but are the design concepts what points here.  

Take a look at the above OO example again:

``` java
c = a.add(b);
a.row(0) = a.row(0).mul(4).add(a.row(0));
```

`b` is added to `a`? Addition is a binary operation, that means that neither of their operands have more importance than the other, they just participate in the operation. `b` is not being added to `a`, it's the addition of `a` and `b` what produces a result, `c`.   

That's a problem. Modeling a binary operation as an object method is a very big mistake. There are really three things playing here: the operator, the first operand, and the second operand. **OO forgets about the first one, with one object playing two roles at the same time**.

Am I being pedant? Consider this:

``` java
c = b.add(a);
```

Is there any semantic difference with the previous example? 

As I said, there aren't objects only in a system. Thankfully, syntax apart, C++ takes this the right way, differentiating between functions and objects. In C++ addition there are three things: two object operands and the operator; where a C++ operator is just a fancy syntax for a non-member function. 

**There are different categories in a system, everything cannot be modeled directly as an object**. The power of C++ comes from its multiparadigm spirit, allowing us to decide what programming paradigm (OO, structured, functional, generic...) is better for each situation.

### Ok, a cool dissertation about C++ over Java, but why metaprogramming?

Because implementing these clear, performant, and almost-static interfaces is not that easy.

You want to automate the implementation of those libraries, or at least describe the most common cases in a generic way, where being generic does not mean parametrizing the element type only... Generics are only a simple toy for kids, to make them think that their libraries are generic, but it's really a casting party at Mr. Autoboxing house.   
Being generic means that if I manage a dynamic array, I don't know the specific policy for dynamic memory the user wants. I just parametrize it:

``` cpp
template<
    class T,
    class Allocator = std::allocator<T>
> class vector;
```

The problem is that that's only the cool interface. The implementation is full of template madness like policy classes, multiple inheritance, template specializations, etc. 

Template meta-programming helps to describe and implement that kind of generic designs, but its syntax makes your maintainer commit suicide just after approaching the codebase.

## What can we do?

TMP is an ugly metalanguage. But its usage can be improved a lot if you think of it as a bastard version of Haskell, what I coined as ["*The Haskell Metaphor*"](http://isocpp.org/blog/2014/11/metaprogramming-with-modern-c-the-haskell-metaphor).

Since a pure bottom-up approach [didn't work as expected](http://www.meetup.com/Madrid-C-Cpp/events/205900412/), let's try another approach: **Learn high-level template metaprogramming developing your own `Tiny Metaprogramming Library`**.

### The library

I'm a big fan of GitHub, so my version of the Tiny Metaprogramming Library [will be hosted on my github account](https://github.com/Manu343726/tiny-metaprogramming-library).

Hosting the library is one thing, but using it is a completely different beast.   
Deployment of C and C++ libraries is a so complex process, since each platform needs its own binary that should be compiled and linked with specific settings. Having a truly portable C++ library is a mess. Meanwhile most modern languages are shipped with their own dependency management system, where setting up a library just becomes using it via an `import`-like sentence and a `install dependencies` command.

[biicode](www.biicode.com) is a startup focused on giving the power of automatic dependency management to C and C++. It's CMake based, so making an existing project work with biicode is easy. Its even easier to manage a project on biicode from scratch.  
The tool works like a charm, resolving all the dependencies and generating pretty projects via CMake generators just `#include`ing what you need:

``` cmake
include(${CMAKE_HOME_DIRECTORY}/biicode.cmake)
include(boost/install/install) #Magic happens here

# Initializes block variables
INIT_BIICODE_BLOCK()
# Actually create targets: EXEcutables and libraries.
ADD_BIICODE_TARGETS()

BII_BOOST(1.57.0)

find_package(Boost COMPONENTS system coroutine REQUIRED)
target_link_libraries(${BII_BLOCK_TARGETS} PUBLIC ${Boost_LIBRARIES})

target_compile_options(${BII_BLOCK_TARGETS} PUBLIC -std=c++11)
```

``` cpp
#include <boost/coroutine/all.hpp>
#include <iostream>

using namespace boost::coroutines;

void cooperative(coroutine<void>::push_type &sink)
{
    std::cout << "Hello";
    sink();
    std::cout << "world";
}

int main()
{
    coroutine<void>::pull_type source{cooperative};
    std::cout << ", ";
    source();
    std::cout << "!\n";
}
```

``` shell
$ bii find
$ bii cpp:configure -G "Unix Makefiles"
$ bii cpp:build
$ ./bin/examples_boost-coroutine_main
Hello, world!
```

So I will develop and deploy my version of the Tiny Metaprogramming Library as a biicode block, [`manu343726/tiny`](https://www.biicode.com/manu343726/tiny), and all the examples provided in the blogposts will be using biicode for setup and building.  
I have developed a metaprogramming library before, the [Turbo Metaprogramming Library](https://github.com/Manu343726/Turbo). Many of my examples and guidelines may resemble the design of Turbo. Others may not, being fixes to bad design decisions.

The idea of this post series is that everybody following them has its own Tiny Metaprogramming Library, in a way that each one is implementing and trying the lessons learned.  
Of course you can ask me whatever questions you like, post comments in the blogposts about specific questions covered there, or questions about my reference implementation on github via the issues system.

### The blogposts

Each week we will learn and implement a little but interesting high-level feature, like expression evaluation, currying, lifting, lambda expressions, etc; and that feature will be added to our Tiny Metaprogramming Library.   
Of course as the blogposts are released, the library will be growing each week, starting from basic concepts to complex features based on those we have learnt and implemented before.

As the series and the libraries evolve my criteria may change, depending on your feedback, but this is the main set of bullets I'm thinking for the posts:

 - Templates, basic concepts (Just re-read [this](http://www.codeproject.com/Articles/826229/Template-Metaprogramming-with-Modern-Cplusplus-tem))
 - Type parameters vs non-type parameters. Value boxing and template boxing (i.e. `std::integral_constant` and `tml::lazy`).
 - Mastering the Haskell bastard: Values and expressions
 - Improving expression evaluation. 
 - Lists
 - Algorithms
 - Iterators
 - Lambda expressions
 - Monads

## Are you ready?

I hope you like this idea. It's not only me writing crazy meta-stuff, but everybody developing their own metaprogramming library, learning something new each week, and comparing the different approaches each one is taking. I'm the guy who writes this posts, but I can learn a lot with your Tiny Metaprogramming Libraries and your feedback. 

> Written with [StackEdit](https://stackedit.io/).
