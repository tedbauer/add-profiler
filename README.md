# insert-add-profiling

Instruments C programs to count the number of dynamic add instructions run in each
function. Adapted from [llvm-pass-skeleton][skeleton].

Build:

    $ cd llvm-pass-skeleton
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ cd ..

Run:

    $ clang -Xclang -load -Xclang build/skeleton/libSkeletonPass.* something.c

[skeleton]: https://github.com/sampsyo/llvm-pass-skeleton
