#include <iostream>

#include "Complex.h"
#include "ComplexSet.h"

int main() {
    ComplexSet a;
    a += Complex(1, 2);
    a += Complex(-1, 0);
    a += Complex(1, -3);
    a += Complex(1, 2);

    ComplexSet b;
    b.add(Complex(1, 2));
    b.add(Complex(0, 5));

    std::cout << "A       = " << a << '\n';
    std::cout << "B       = " << b << '\n';
    std::cout << "A | B   = " << (a | b) << '\n';
    std::cout << "A & B   = " << (a & b) << '\n';
    std::cout << "A - B   = " << (a - b) << '\n';
    std::cout << "(1,2) in A: " << std::boolalpha << a(Complex(1, 2)) << '\n';
    std::cout << "A == B: " << (a == b) << '\n';
    return 0;
}
