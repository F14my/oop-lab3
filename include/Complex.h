#pragma once

#include <iostream>


class Complex {
public:
    Complex();
    Complex(double re, double im);

    double re() const;
    double im() const;


    bool operator==(const Complex& other) const;
    bool operator!=(const Complex& other) const;

    bool operator<(const Complex& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Complex& c);

private:
    double re_;
    double im_;
};
