#include <gtest/gtest.h>

#include <sstream>
#include <string>
#include <utility>

#include "Complex.h"
#include "ComplexSet.h"

namespace {

std::string toString(const Complex& value) {
    std::ostringstream out;
    out << value;
    return out.str();
}

std::string toString(const ComplexSet& value) {
    std::ostringstream out;
    out << value;
    return out.str();
}

}

TEST(ComplexTest, DefaultIsZero) {
    Complex z;
    EXPECT_EQ(z.re(), 0.0);
    EXPECT_EQ(z.im(), 0.0);
}

TEST(ComplexTest, Equality) {
    EXPECT_TRUE(Complex(1, 2) == Complex(1, 2));
    EXPECT_FALSE(Complex(1, 2) == Complex(2, 1));
    EXPECT_TRUE(Complex(1, 2) != Complex(1, 3));
}

TEST(ComplexTest, LexicographicLess) {
    EXPECT_TRUE(Complex(1, 5) < Complex(2, 0));
    EXPECT_TRUE(Complex(1, 2) < Complex(1, 3));
    EXPECT_FALSE(Complex(1, 3) < Complex(1, 2));
    EXPECT_FALSE(Complex(1, 2) < Complex(1, 2));
    EXPECT_TRUE(Complex(-3, 0) < Complex(0, -100));
}

TEST(ComplexTest, EqualityConsistentWithLess) {
    Complex a(2, -1);
    Complex b(2, -1);
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a < b);
    EXPECT_FALSE(b < a);
}

TEST(ComplexTest, Output) {
    EXPECT_EQ(toString(Complex(1, 2)), "(1, 2)");
    EXPECT_EQ(toString(Complex(-1.5, 0)), "(-1.5, 0)");
}

TEST(ComplexSetTest, EmptySet) {
    ComplexSet s;
    EXPECT_EQ(s.size(), 0u);
    EXPECT_TRUE(s.empty());
    EXPECT_FALSE(s.contains(Complex(0, 0)));
    EXPECT_FALSE(s(Complex(0, 0)));
}

TEST(ComplexSetTest, AddNewAndDuplicate) {
    ComplexSet s;
    EXPECT_TRUE(s.add(Complex(1, 2)));
    EXPECT_FALSE(s.add(Complex(1, 2)));
    EXPECT_EQ(s.size(), 1u);
    EXPECT_FALSE(s.empty());
}

TEST(ComplexSetTest, RemoveExistingAndMissing) {
    ComplexSet s;
    s.add(Complex(1, 2));
    s.add(Complex(3, 4));
    EXPECT_TRUE(s.remove(Complex(1, 2)));
    EXPECT_FALSE(s.remove(Complex(1, 2)));
    EXPECT_FALSE(s.remove(Complex(9, 9)));
    EXPECT_EQ(s.size(), 1u);
    EXPECT_FALSE(s.contains(Complex(1, 2)));
    EXPECT_TRUE(s.contains(Complex(3, 4)));
}

TEST(ComplexSetTest, ContainsMatchesCallOperator) {
    ComplexSet s;
    for (int i = 0; i < 10; i += 2) {
        s.add(Complex(i, -i));
    }
    for (int i = -2; i < 12; ++i) {
        Complex z(i, -i);
        EXPECT_EQ(s.contains(z), s(z)) << "i = " << i;
        EXPECT_EQ(s.contains(z), i >= 0 && i < 10 && i % 2 == 0);
    }
}

TEST(ComplexSetTest, CopyIsIndependent) {
    ComplexSet a;
    a.add(Complex(1, 1));
    a.add(Complex(2, 2));

    ComplexSet b(a);
    EXPECT_EQ(a, b);
    b.add(Complex(3, 3));
    b.remove(Complex(1, 1));
    EXPECT_EQ(a.size(), 2u);
    EXPECT_TRUE(a.contains(Complex(1, 1)));
    EXPECT_FALSE(a.contains(Complex(3, 3)));

    ComplexSet c;
    c.add(Complex(100, 0));
    c = a;
    EXPECT_EQ(c, a);
    c.add(Complex(5, 5));
    EXPECT_FALSE(a.contains(Complex(5, 5)));
}

TEST(ComplexSetTest, MoveLeavesSourceEmpty) {
    ComplexSet a;
    a.add(Complex(1, 1));
    a.add(Complex(2, 2));

    ComplexSet b(std::move(a));
    EXPECT_EQ(b.size(), 2u);
    EXPECT_EQ(a.size(), 0u);
    EXPECT_TRUE(a.empty());
    EXPECT_FALSE(a.contains(Complex(1, 1)));
    EXPECT_EQ(toString(a), "{ }");
    EXPECT_TRUE(a.add(Complex(7, 7)));
    EXPECT_EQ(a.size(), 1u);

    ComplexSet c;
    c.add(Complex(42, 0));
    c = std::move(b);
    EXPECT_EQ(c.size(), 2u);
    EXPECT_FALSE(c.contains(Complex(42, 0)));
    EXPECT_TRUE(b.empty());
    EXPECT_EQ(b.capacity(), 0u);
}

TEST(ComplexSetTest, SetOperations) {
    ComplexSet a;
    ComplexSet b;
    for (int x : {7, 1, 5, 3}) a.add(Complex(x, 0));
    for (int x : {6, 3, 2}) b.add(Complex(x, 0));

    EXPECT_EQ(toString(a | b), "{ (1, 0), (2, 0), (3, 0), (5, 0), (6, 0), (7, 0) }");
    EXPECT_EQ(toString(a & b), "{ (3, 0) }");
    EXPECT_EQ(toString(a - b), "{ (1, 0), (5, 0), (7, 0) }");
    EXPECT_EQ(toString(b - a), "{ (2, 0), (6, 0) }");

    EXPECT_EQ(a.size(), 4u);
    EXPECT_EQ(b.size(), 3u);
}

TEST(ComplexSetTest, EqualityOperators) {
    ComplexSet a;
    ComplexSet b;
    a.add(Complex(1, 2));
    a.add(Complex(3, 4));
    b.add(Complex(3, 4));
    b.add(Complex(1, 2));
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);

    b.add(Complex(5, 6));
    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);

    ComplexSet c;
    c.add(Complex(1, 2));
    c.add(Complex(3, 5));
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);

    EXPECT_TRUE(ComplexSet() == ComplexSet());
}

TEST(ComplexSetTest, OutputFormat) {
    ComplexSet s;
    EXPECT_EQ(toString(s), "{ }");
    s.add(Complex(1, 2));
    EXPECT_EQ(toString(s), "{ (1, 2) }");
    s.add(Complex(0, 0));
    EXPECT_EQ(toString(s), "{ (0, 0), (1, 2) }");
}

TEST(ComplexSetTest, StressThousandAdds) {
    ComplexSet s;
    int added = 0;
    for (int i = 0; i < 1000; ++i) {
        if (s.add(Complex(i % 250, (i * 7) % 3))) {
            ++added;
        }
    }
    EXPECT_EQ(s.size(), static_cast<std::size_t>(added));

    for (int i = 0; i < 1000; ++i) {
        EXPECT_FALSE(s.add(Complex(i % 250, (i * 7) % 3)));
    }
    EXPECT_EQ(s.size(), static_cast<std::size_t>(added));

    ComplexSet reversed;
    for (int i = 999; i >= 0; --i) {
        reversed.add(Complex(i % 250, (i * 7) % 3));
    }
    EXPECT_EQ(s, reversed);
    EXPECT_EQ(toString(s), toString(reversed));
}

TEST(ComplexSetTest, InsertAtBeginMiddleEnd) {
    ComplexSet s;
    s.add(Complex(5, 0));
    s.add(Complex(1, 0));
    s.add(Complex(9, 0));
    s.add(Complex(3, 0));
    s.add(Complex(7, 0));
    EXPECT_EQ(toString(s), "{ (1, 0), (3, 0), (5, 0), (7, 0), (9, 0) }");
}

TEST(ComplexSetTest, LexicographicOrderInSet) {
    ComplexSet s;
    s.add(Complex(1, 3));
    s.add(Complex(1, -1));
    s.add(Complex(0, 100));
    s.add(Complex(1, 0));
    s.add(Complex(-2, 5));
    EXPECT_EQ(toString(s), "{ (-2, 5), (0, 100), (1, -1), (1, 0), (1, 3) }");
    EXPECT_TRUE(s.contains(Complex(1, 0)));
    EXPECT_FALSE(s.contains(Complex(1, 1)));
}

TEST(ComplexSetTest, RemoveFirstLastOnly) {
    ComplexSet s;
    for (int x : {1, 2, 3, 4}) s.add(Complex(x, 0));

    EXPECT_TRUE(s.remove(Complex(1, 0)));
    EXPECT_EQ(toString(s), "{ (2, 0), (3, 0), (4, 0) }");
    EXPECT_TRUE(s.remove(Complex(4, 0)));
    EXPECT_EQ(toString(s), "{ (2, 0), (3, 0) }");
    EXPECT_TRUE(s.remove(Complex(2, 0)));
    EXPECT_TRUE(s.remove(Complex(3, 0)));
    EXPECT_TRUE(s.empty());
    EXPECT_EQ(toString(s), "{ }");
    EXPECT_FALSE(s.remove(Complex(3, 0)));
    EXPECT_TRUE(s.add(Complex(3, 0)));
}

TEST(ComplexSetTest, RemoveFromEmpty) {
    ComplexSet s;
    EXPECT_FALSE(s.remove(Complex(1, 1)));
    EXPECT_TRUE(s.empty());
}

TEST(ComplexSetTest, CapacityGrowsByDoubling) {
    ComplexSet s;
    EXPECT_EQ(s.capacity(), 0u);
    s.add(Complex(0, 0));
    EXPECT_EQ(s.capacity(), 4u);
    for (int i = 1; i < 5; ++i) s.add(Complex(i, 0));
    EXPECT_EQ(s.capacity(), 8u);
    for (int i = 5; i < 9; ++i) s.add(Complex(i, 0));
    EXPECT_EQ(s.capacity(), 16u);
}

TEST(ComplexSetTest, CompoundAssignmentChains) {
    ComplexSet s;
    (s += Complex(1, 1)) += Complex(2, 2);
    s += Complex(1, 1);
    EXPECT_EQ(s.size(), 2u);
    (s -= Complex(1, 1)) -= Complex(100, 100);
    EXPECT_EQ(toString(s), "{ (2, 2) }");
}

TEST(ComplexSetTest, SelfAssignment) {
    ComplexSet s;
    s.add(Complex(1, 2));
    s.add(Complex(3, 4));
    ComplexSet& ref = s;
    s = ref;
    EXPECT_EQ(toString(s), "{ (1, 2), (3, 4) }");
    s = std::move(ref);
    EXPECT_EQ(toString(s), "{ (1, 2), (3, 4) }");
}

TEST(ComplexSetTest, CopyAndMoveFromEmpty) {
    ComplexSet empty;
    ComplexSet copy(empty);
    EXPECT_TRUE(copy.empty());
    ComplexSet moved(std::move(empty));
    EXPECT_TRUE(moved.empty());
    EXPECT_TRUE(empty.empty());

    ComplexSet target;
    target.add(Complex(1, 1));
    target = ComplexSet();
    EXPECT_TRUE(target.empty());
}

TEST(ComplexSetTest, OperationsWithEmpty) {
    ComplexSet a;
    a.add(Complex(1, 0));
    a.add(Complex(2, 0));
    ComplexSet e;

    EXPECT_EQ(a | e, a);
    EXPECT_EQ(e | a, a);
    EXPECT_TRUE((a & e).empty());
    EXPECT_TRUE((e & a).empty());
    EXPECT_EQ(a - e, a);
    EXPECT_TRUE((e - a).empty());
    EXPECT_TRUE((e | e).empty());
}

TEST(ComplexSetTest, OperationsDisjointAndEqual) {
    ComplexSet a;
    ComplexSet b;
    a.add(Complex(1, 0));
    a.add(Complex(2, 0));
    b.add(Complex(3, 0));
    b.add(Complex(4, 0));

    EXPECT_EQ((a | b).size(), 4u);
    EXPECT_TRUE((a & b).empty());
    EXPECT_EQ(a - b, a);

    EXPECT_EQ(a | a, a);
    EXPECT_EQ(a & a, a);
    EXPECT_TRUE((a - a).empty());
}

TEST(ComplexSetTest, SetIdentities) {
    ComplexSet a;
    ComplexSet b;
    for (int i = 0; i < 20; ++i) a.add(Complex(i % 7, i % 3));
    for (int i = 0; i < 20; ++i) b.add(Complex(i % 5, i % 4));

    EXPECT_EQ(a | b, b | a);
    EXPECT_EQ(a & b, b & a);
    EXPECT_EQ((a - b) | (a & b), a);
    EXPECT_TRUE(((a - b) & b).empty());
    EXPECT_EQ((a | b).size(), a.size() + b.size() - (a & b).size());
}

TEST(ComplexSetTest, ResultOfOperationIsUsableSet) {
    ComplexSet a;
    ComplexSet b;
    a.add(Complex(1, 0));
    b.add(Complex(3, 0));
    ComplexSet u = a | b;
    EXPECT_TRUE(u.add(Complex(2, 0)));
    EXPECT_FALSE(u.add(Complex(3, 0)));
    EXPECT_EQ(toString(u), "{ (1, 0), (2, 0), (3, 0) }");
}
