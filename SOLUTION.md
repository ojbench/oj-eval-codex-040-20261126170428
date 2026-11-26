# Implementation and verification

`solution.hpp` preserves the supplied template, replacing only its TODO blocks.
Forward links own nodes through `shared_ptr`; backward links and the tail use
`weak_ptr`, so there are no ownership cycles. Empty and singleton transitions
update both endpoints. Iterators retain shared ownership and postfix increment
returns the original position.

Each endpoint operation and iterator operation takes O(1) time, excluding the
cost of copying T. The list uses O(n) storage. Memory is managed entirely by
standard smart pointers, without explicit allocation or deallocation.

Build and run the local tests with:

```sh
cmake .
make
./code
```

Tests compare 100,000 deterministic random operations with `std::deque`, check
iteration and writable dereferencing, exercise empty/singleton reuse, and track
object lifetimes when iterators outlive the list. They also passed GCC's address
and undefined-behavior sanitizers. The advertised sample directory was absent.

The live problem API accepts only `cpp`, and the assignment supplies the judge
driver. Submit `solution.hpp` as C++ source, without a main function. The CMake
`code` target is the local test runner. The supplied client's missing
`submit_code` method has been restored to support its documented CLI.
