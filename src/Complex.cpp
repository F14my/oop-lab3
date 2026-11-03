#include "Complex.h"

Complex::Complex() : re_(0.0), im_(0.0) {}

Complex::Complex(double re, double im) : re_(re), im_(im) {}

double Complex::re() const { return re_; }
double Complex::im() const { return im_; }

bool Complex::operator==(const Complex& other) const {
    return re_ == other.re_ && im_ == other.im_;
}

bool Complex::operator!=(const Complex& other) const {
    return !(*this == other);
}

bool Complex::operator<(const Complex& other) const {
    if (re_ != other.re_) {
        return re_ < other.re_;
    }
    return im_ < other.im_;
}

std::ostream& operator<<(std::ostream& os, const Complex& c) {
    return os << '(' << c.re_ << ", " << c.im_ << ')';
}
