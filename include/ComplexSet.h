#pragma once

#include <cstddef>
#include <iostream>

#include "Complex.h"


class ComplexSet {
public:
    ComplexSet();
    ComplexSet(const ComplexSet& other);
    ComplexSet(ComplexSet&& other) noexcept;
    ComplexSet& operator=(const ComplexSet& other);
    ComplexSet& operator=(ComplexSet&& other) noexcept;
    ~ComplexSet();

    bool add(const Complex& x);
    bool remove(const Complex& x);
    bool contains(const Complex& x) const;
    bool operator()(const Complex& x) const;

    std::size_t size() const;
    bool empty() const;
    std::size_t capacity() const;

    ComplexSet& operator+=(const Complex& x);
    ComplexSet& operator-=(const Complex& x);

    ComplexSet operator|(const ComplexSet& other) const;
    ComplexSet operator&(const ComplexSet& other) const;
    ComplexSet operator-(const ComplexSet& other) const;

    bool operator==(const ComplexSet& other) const;
    bool operator!=(const ComplexSet& other) const;

    void swap(ComplexSet& other) noexcept;

    friend std::ostream& operator<<(std::ostream& os, const ComplexSet& s);

private:
    Complex* data_;
    std::size_t size_;
    std::size_t capacity_;

    std::size_t lowerBound(const Complex& x) const;

    void reserve(std::size_t minCapacity);

    void pushBackUnchecked(const Complex& x);
};
