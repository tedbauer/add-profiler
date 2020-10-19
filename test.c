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
