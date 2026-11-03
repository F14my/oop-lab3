#include "ComplexSet.h"

#include <utility>

static const std::size_t kInitialCapacity = 4;

ComplexSet::ComplexSet() : data_(nullptr), size_(0), capacity_(0) {}

ComplexSet::ComplexSet(const ComplexSet& other)
    : data_(nullptr), size_(0), capacity_(0) {
    if (other.size_ > 0) {
        data_ = new Complex[other.size_];
        for (std::size_t i = 0; i < other.size_; ++i) {
            data_[i] = other.data_[i];
        }
        size_ = other.size_;
        capacity_ = other.size_;
    }
}

ComplexSet::ComplexSet(ComplexSet&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

ComplexSet& ComplexSet::operator=(const ComplexSet& other) {
    if (this != &other) {
        ComplexSet tmp(other);
        swap(tmp);
    }
    return *this;
}

ComplexSet& ComplexSet::operator=(ComplexSet&& other) noexcept {
    if (this != &other) {
        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

ComplexSet::~ComplexSet() {
    delete[] data_;
}

void ComplexSet::swap(ComplexSet& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

std::size_t ComplexSet::lowerBound(const Complex& x) const {
    std::size_t lo = 0;
    std::size_t hi = size_;
    while (lo < hi) {
        std::size_t mid = lo + (hi - lo) / 2;
        if (data_[mid] < x) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

void ComplexSet::reserve(std::size_t minCapacity) {
    if (minCapacity <= capacity_) {
        return;
    }
    std::size_t newCapacity = (capacity_ == 0) ? kInitialCapacity : capacity_ * 2;
    while (newCapacity < minCapacity) {
        newCapacity *= 2;
    }

    Complex* newData = new Complex[newCapacity];
    for (std::size_t i = 0; i < size_; ++i) {
        newData[i] = std::move(data_[i]);
    }
    delete[] data_;
    data_ = newData;
    capacity_ = newCapacity;
}

void ComplexSet::pushBackUnchecked(const Complex& x) {
    reserve(size_ + 1);
    data_[size_] = x;
    ++size_;
}

bool ComplexSet::add(const Complex& x) {
    std::size_t pos = lowerBound(x);
    if (pos < size_ && data_[pos] == x) {
        return false;
    }
    reserve(size_ + 1);
    for (std::size_t i = size_; i > pos; --i) {
        data_[i] = data_[i - 1];
    }
    data_[pos] = x;
    ++size_;
    return true;
}

bool ComplexSet::remove(const Complex& x) {
    std::size_t pos = lowerBound(x);
    if (pos >= size_ || !(data_[pos] == x)) {
        return false;
    }
    for (std::size_t i = pos; i + 1 < size_; ++i) {
        data_[i] = data_[i + 1];
    }
    --size_;
    return true;
}

bool ComplexSet::contains(const Complex& x) const {
    std::size_t pos = lowerBound(x);
    return pos < size_ && data_[pos] == x;
}

bool ComplexSet::operator()(const Complex& x) const {
    return contains(x);
}

std::size_t ComplexSet::size() const { return size_; }
bool ComplexSet::empty() const { return size_ == 0; }
std::size_t ComplexSet::capacity() const { return capacity_; }

ComplexSet& ComplexSet::operator+=(const Complex& x) {
    add(x);
    return *this;
}

ComplexSet& ComplexSet::operator-=(const Complex& x) {
    remove(x);
    return *this;
}

ComplexSet ComplexSet::operator|(const ComplexSet& other) const {
    ComplexSet result;
    result.reserve(size_ + other.size_);
    std::size_t i = 0;
    std::size_t j = 0;
    while (i < size_ && j < other.size_) {
        if (data_[i] < other.data_[j]) {
            result.pushBackUnchecked(data_[i++]);
        } else if (other.data_[j] < data_[i]) {
            result.pushBackUnchecked(other.data_[j++]);
        } else {
            result.pushBackUnchecked(data_[i]);
            ++i;
            ++j;
        }
    }
    while (i < size_) {
        result.pushBackUnchecked(data_[i++]);
    }
    while (j < other.size_) {
        result.pushBackUnchecked(other.data_[j++]);
    }
    return result;
}

ComplexSet ComplexSet::operator&(const ComplexSet& other) const {
    ComplexSet result;
    result.reserve(size_ < other.size_ ? size_ : other.size_);
    std::size_t i = 0;
    std::size_t j = 0;
    while (i < size_ && j < other.size_) {
        if (data_[i] < other.data_[j]) {
            ++i;
        } else if (other.data_[j] < data_[i]) {
            ++j;
        } else {
            result.pushBackUnchecked(data_[i]);
            ++i;
            ++j;
        }
    }
    return result;
}

ComplexSet ComplexSet::operator-(const ComplexSet& other) const {
    ComplexSet result;
    result.reserve(size_);
    std::size_t i = 0;
    std::size_t j = 0;
    while (i < size_ && j < other.size_) {
        if (data_[i] < other.data_[j]) {
            result.pushBackUnchecked(data_[i++]);
        } else if (other.data_[j] < data_[i]) {
            ++j;
        } else {
            ++i;
            ++j;
        }
    }
    while (i < size_) {
        result.pushBackUnchecked(data_[i++]);
    }
    return result;
}

bool ComplexSet::operator==(const ComplexSet& other) const {
    if (size_ != other.size_) {
        return false;
    }
    for (std::size_t i = 0; i < size_; ++i) {
        if (!(data_[i] == other.data_[i])) {
            return false;
        }
    }
    return true;
}

bool ComplexSet::operator!=(const ComplexSet& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const ComplexSet& s) {
    os << "{ ";
    for (std::size_t i = 0; i < s.size_; ++i) {
        if (i > 0) {
            os << ", ";
        }
        os << s.data_[i];
    }
    if (s.size_ > 0) {
        os << ' ';
    }
    return os << '}';
}
