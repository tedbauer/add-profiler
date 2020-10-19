# insert-add-profiling

Instruments C programs to count the number of dynamic add instructions run in each
function. Adapted from [llvm-pass-skeleton][skeleton].

Build:

    $ cd insert-add-profiling
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ cd ..

Find the number of dynamic `mul` instructions run in `test.c`:

    $ clang -Xclang -load -Xclang build/skeleton/libSkeletonPass.* test.c
    $ cc -c lib.o
    $ cc test.o lib.o
    $ ./a.out

For example, this program:
```
#include <stdio.h>

int func2(int h) {
	while (h < 100) {
		h = h * 2;
	}
	return h;
}

int main(int argc, const char** argv) {
	int n = 2;
	while (n < 20) {
		n = n * 5;
	}

	int h = 1;
	while (h < 100) {
		h = h * 2;
	}
	func2(1);
	return 0;
}
```

Outputs:
```
- function `func2`: 8 mul instructions
- function `main`: 10 mul instructions
```

[skeleton]: https://github.com/sampsyo/llvm-pass-skeleton
