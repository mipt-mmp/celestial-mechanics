#ifndef ENGINE_GEOMETRY_HPP
#define ENGINE_GEOMETRY_HPP

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

namespace geom {
    
/**
 * @brief Vector class. 
 * 
 * @tparam T - element of vector.
 * @tparam dim - dimension of vector. 
 */

namespace traits {

    template <typename T>
    struct StdTraits {
        static auto sqrt(const T& t) {
            return std::sqrt(t);
        }
    };
}

template <typename T, std::size_t dim, template<typename> class traits = traits::StdTraits>
struct Vector {
    std::array<T, dim> coord_{};

    constexpr Vector() = default;
    constexpr Vector(const Vector&) = default;
    constexpr Vector& operator=(const Vector&) = default;
    constexpr Vector(std::initializer_list<T> l) {
        for (std::size_t i = 0; i < dim; ++i) {
            coord_[i] = *(l.begin() + i);
        }
    }

    template <class U>
    constexpr explicit Vector(const Vector<U, dim, traits>& oth) {
        *this = oth;
    }

    template <class U>                                          
    constexpr Vector& operator=(const Vector<U, dim, traits>& oth) { 
        for (std::size_t i = 0; i < dim; ++i) {                      
            coord_[i] = oth.coord_[i];                        
        }                                                       
        return *this;                                           
    }

    template <class U>                                          
    constexpr Vector& operator -=(const Vector<U, dim, traits>& oth) { 
        for (std::size_t i = 0; i < dim; ++i) {                      
            coord_[i] -= oth.coord_[i];                        
        }                                                       
        return *this;                                           
    }

    template <class U>                                          
    constexpr Vector& operator +=(const Vector<U, dim, traits>& oth) { 
        for (std::size_t i = 0; i < dim; ++i) {                      
            coord_[i] += oth.coord_[i];                        
        }                                                       
        return *this;                                           
    }



    template <class U>
    constexpr Vector& operator*=(U c) {
        for (std::size_t i = 0; i < dim; ++i) {
            coord_[i] *= c;
        }
        return *this;
    }

    template <class U>
    constexpr Vector& operator/=(U c) {
        for (std::size_t i = 0; i < dim; ++i) {
            coord_[i] /= c;
        }
        return *this;
    }

    constexpr const T& operator[](std::size_t i) const {
        assert(i < dim);
        return coord_[i];
    }

    constexpr T& operator[](std::size_t i) {
        assert(i < dim);
        return coord_[i];
    }

    constexpr bool operator<(const Vector& oth) const {
        for (std::size_t i = 0; i < dim; ++i) {
            if (coord_[i] != oth.coord_[i]) {
                return coord_[i] < oth.coord_[i];
            }
        }
        return false;
    }

    constexpr bool operator>(const Vector& oth) const {
        for (std::size_t i = 0; i < dim; ++i) {
            if (coord_[i] != oth.coord_[i]) {
                return coord_[i] > oth.coord_[i];
            }
        }
        return false;
    }

    constexpr bool operator<=(const Vector& oth) const {
        return !(*this > oth);
    }

    constexpr bool operator>=(const Vector& oth) const {
        return !(*this < oth);
    }

    constexpr bool operator==(const Vector& oth) const {
        for (std::size_t i = 0; i < dim; ++i) {
            if (coord_[i] != oth.coord_[i]) {
                return false;
            }
        }
        return true;
    }

    constexpr bool operator!=(const Vector& oth) const {
        return !(*this == oth);
    }

    using Len2_t = decltype(T{} * T{});
    using Len_t  = decltype(traits<Len2_t>::sqrt(Len2_t{}));

    constexpr Len2_t Len2() const {
        return (*this, *this);
    }
    

    constexpr Len_t Len() const {
        return traits<Len2_t>::sqrt(Len2());
    }

    constexpr bool operator||(const Vector& oth) const {
        T scalar = (*this, oth);
        return scalar * scalar == Len2() * oth.Len2();
    }

    constexpr T& X() {
        static_assert(dim >= 1);
        return coord_[0];
    }
    constexpr const T& X() const {
        static_assert(dim >= 1);
        return coord_[0];
    }

    constexpr T& Y() {
        static_assert(dim >= 2);
        return coord_[1];
    }
    constexpr const T& Y() const {
        static_assert(dim >= 2);
        return coord_[1];
    }

    constexpr T& Z() {
        static_assert(dim >= 3);
        return coord_[2];
    }
    constexpr const T& Z() const {
        static_assert(dim >= 3);
        return coord_[2];
    }
};

template <typename T, std::size_t Dim, template<typename> typename traits = traits::StdTraits>
using Point = Vector<T, Dim, traits>;

template <class T, std::size_t Dim, template<typename> typename traits = traits::StdTraits>
constexpr auto operator,(const Vector<T, Dim, traits>& lhs, const Vector<T, Dim, traits>& rhs) {
    decltype(lhs[0] * rhs[0]) ans{};
    for (std::size_t i = 0; i < Dim; ++i) {
        ans += lhs[i] * rhs[i];
    }
    return ans;
}

template <class T, std::size_t Dim, template<typename> typename traits = traits::StdTraits>
Vector<T, Dim, traits> operator+(const Vector<T, Dim, traits>& lhs, const Vector<T, Dim, traits>& rhs) {
    return Vector<T, Dim, traits>(lhs) += rhs;
}

template <class T, std::size_t Dim, template<typename> typename traits = traits::StdTraits>
Vector<T, Dim, traits> operator-(const Vector<T, Dim, traits>& lhs, const Vector<T, Dim, traits>& rhs) {
    return Vector<T, Dim, traits>(lhs) -= rhs;
}

template <class T, std::size_t Dim, class U, template<typename> typename traits = traits::StdTraits>
Vector<T, Dim, traits> operator*(const Vector<T, Dim, traits>& lhs, U rhs) {
    return Vector<T, Dim, traits>(lhs) *= std::move(rhs);
}

template <class T, std::size_t Dim, class U, template<typename> typename traits = traits::StdTraits>
Vector<T, Dim, traits> operator/(const Vector<T, Dim, traits>& lhs, U rhs) {
    return Vector<T, Dim, traits>(lhs) /= std::move(rhs);
}

template <class T, std::size_t Dim, template<typename> typename traits = traits::StdTraits>
Vector<T, Dim, traits> operator-(const Vector<T, Dim, traits>& ths) {
    return Vector<T, Dim, traits>(ths) *= -1;
}

template <class T>
using Vector2 = Vector<T, 2>;

/**
 * @brief Cross product of two vectors.
 * 
 * @param lhs - left argument
 * @param rhs - right argument
 * @return constexpr T - cross product 
 */
template <class T>
constexpr T CrossProd(const Vector2<T>& lhs, const Vector2<T>& rhs) {
    return lhs.X() * rhs.Y() - lhs.Y() * rhs.X();
}

template <class T, std::size_t Dim, template<typename> typename traits = traits::StdTraits>
const Vector<T, Dim, traits> kZero = {};

template <class T, std::size_t Dim, std::size_t I, template<typename> typename traits = traits::StdTraits>
constexpr const Vector<T, Dim, traits> kBasis = [](std::size_t i) {
    Vector<T, Dim, traits> v{};
    v[i] = 1;
    return v;
}(I);

/**
 * @brief Class respresenting segment on space
 */
template <class T, std::size_t Dim, template<typename> typename traits = traits::StdTraits>
struct Segment {
    using Vec = Vector<T, Dim, traits>;
    Vec p1, p2;
};

/**
 * @brief Project Vector on base axis
 * 
 * @param vec - vector 
 * @param i - index of axis to project
 * @return constexpr Vector<T, 1> - projection
 */
template <class T, std::size_t Dim, template<typename> typename traits = traits::StdTraits>
constexpr Vector<T, 1> Project(const Vector<T, Dim, traits>& vec, std::size_t i) {
    return {vec[i]};
}

/**
 * @brief Project Segment on base axis
 * 
 * @param seg - segment 
 * @param i - index of axis to project
 * @return constexpr Segment<T, 1> - projection
 */
template <class T, std::size_t Dim,  template<typename> typename traits = traits::StdTraits>
constexpr Segment<T, 1> Project(const Segment<T, Dim>& seg, std::size_t i) {
    return {Project(seg.p1, i), Project(seg.p2, i)};
}

/**
 * @brief Ditance between two geomtric object
 * 
 * @param x - first object 
 * @param y - second object
 * @return auto - distance
 */
template <class T, class U>
auto Dist(T x, U y);

/**
 * @brief Check if two geomtric object crosses
 * 
 * @param x - first object 
 * @param y - second object
 * @return bool - if cross.
 */
template <class T, class U>
bool Cross(T x, U y);

template <class T, std::size_t Dim, template<typename> typename traits = traits::StdTraits>
auto Dist(const Vector<T, Dim, traits>& lhs, const Vector<T, Dim, traits>& rhs) -> decltype(Vector<T, Dim, traits>{}.Len()) {
    return (lhs - rhs).Len();
}

template <class T>
auto Dist(const Segment<T, 2>& lhs, const Vector<T, 2>& rhs) -> decltype(Vector<T, 2>{}.Len()) {
    if (lhs.p1 == lhs.p2) {
        return Dist(lhs.p1, rhs);
    }
    Vector<T, 2> a = rhs - lhs.p1;
    Vector<T, 2> b = lhs.p2 - lhs.p1;
    T dist = (a, b);
    if (dist <= 0) {
        return Dist(rhs, lhs.p1);
    }
    if (dist >= b.Len2()) {
        return Dist(rhs, lhs.p1);
    }
    return std::abs(CrossProd(a, b)) / b.Len();
}

template <class T>
auto Dist(const Segment<T, 2>& lhs, const Segment<T, 2>& rhs) -> decltype(Vector<T, 2>{}.Len()) {
    if (Cross(lhs, rhs)) {
        return 0;
    }
    return std::min(std::min(Dist(lhs, rhs.p1), Dist(lhs, rhs.p2)), std::min(Dist(rhs, lhs.p1), Dist(rhs, lhs.p2)));
}

template <class T>
bool Cross(Segment<T, 1> lhs, Segment<T, 1> rhs) {
    if (lhs.p1 > lhs.p2) {
        std::swap(lhs.p1, lhs.p2);
    }

    if (rhs.p1 > rhs.p2) {
        std::swap(rhs.p1, rhs.p2);
    }
    return std::max(lhs.p1, rhs.p1) <= std::min(lhs.p2, rhs.p2);
}

template <class T>
bool Cross(const Segment<T, 2>& lhs, const Segment<T, 2>& rhs) {
    return Cross(Project(lhs, 0), Project(rhs, 0)) && Cross(Project(lhs, 1), Project(rhs, 1)) &&
           CrossProd(lhs.p2 - lhs.p1, rhs.p1 - lhs.p1) * CrossProd(lhs.p2 - lhs.p1, rhs.p2 - lhs.p1) <= 0 &&
           CrossProd(rhs.p2 - rhs.p1, lhs.p1 - rhs.p1) * CrossProd(rhs.p2 - rhs.p1, lhs.p2 - rhs.p1) <= 0;
}

template <class T, std::size_t Dim,  template<typename> typename traits = traits::StdTraits>
std::ostream& operator<<(std::ostream& out, const Vector<T, Dim, traits>& vec) {
    for (std::size_t i = 0; i < Dim; ++i) {
        out << vec[i] << ' ';
    }
    return out;
}

template <class T, std::size_t Dim,  template<typename> typename traits = traits::StdTraits>
std::istream& operator>>(std::istream& in, Vector<T, Dim, traits>& vec) {
    for (std::size_t i = 0; i < Dim; ++i) {
        in >> vec[i];
    }
    return in;
}

template <class T, std::size_t Dim,  template<typename> typename traits = traits::StdTraits>
std::istream& operator>>(std::istream& in, Segment<T, Dim>& seg) {
    return in >> seg.p1 >> seg.p2;
}

/**
 * @brief Doubled oriented square of triange on surface
 * 
 * @param v1 - points of triangle
 * @param v2 - points of triangle
 * @param v3 - points of triangle
 * @return T - Doubled oriented square
 */
template <class T>
T OrientedSquare2(const Vector<T, 2>& v1, const Vector<T, 2>& v2, const Vector<T, 2>& v3) {
    return CrossProd(v2 - v1, v3 - v1);
}

/**
 * @brief If rotation from vector v2-v1 to v3-v1 is clockwise. 
 *  Alt: 3 has clockwise rotating chain line 
 * @tparam T 
 * @param v1 - first  point
 * @param v2 - second point
 * @param v3 - third  point
 * @return bool - is clockwise
 */
template <class T>
bool IsClockwise(const Vector<T, 2>& v1, const Vector<T, 2>& v2, const Vector<T, 2>& v3) {
    return CrossProd(v2 - v1, v3 - v1) < 0;
}

/**
 * @brief If rotation from vector v2-v1 to v3-v1 is counter-clockwise. 
 *  Alt: 3 has counter-clockwise rotating chain line 
 * @tparam T 
 * @param v1 - first  point
 * @param v2 - second point
 * @param v3 - third  point
 * @return bool - is counter-clockwise
 */
template <class T>
bool IsCClockwise(const Vector<T, 2>& v1, const Vector<T, 2>& v2, const Vector<T, 2>& v3) {
    return CrossProd(v2 - v1, v3 - v1) > 0;
}

/**
 * @brief Building convex hull from array of points.
 * 
 * @param points - points to make hull
 * @return std::vector<Vector2<T>> - convex hull.
 */
template <class T>
std::vector<Vector2<T>> ConvexHull(std::vector<Vector2<T>> points) {
    if (points.size() < 3) {
        return points;
    }
    std::sort(points.begin(), points.end());
    std::vector<Vector2<T>> up_convex;
    std::vector<Vector2<T>> dn_convex;
    up_convex.push_back(points.front());
    dn_convex.push_back(points.front());

    for (std::size_t i = 1; i < points.size() - 1; ++i) {
        if (IsClockwise(points.front(), points[i], points.back())) {
            while (up_convex.size() >= 2 &&
                   !IsClockwise(up_convex[up_convex.size() - 2], up_convex[up_convex.size() - 1], points[i])) {
                up_convex.pop_back();
            }
            up_convex.push_back(points[i]);
        }

        if (IsCClockwise(points.front(), points[i], points.back())) {
            while (dn_convex.size() >= 2 &&
                   !IsCClockwise(dn_convex[dn_convex.size() - 2], dn_convex[dn_convex.size() - 1], points[i])) {
                dn_convex.pop_back();
            }
            dn_convex.push_back(points[i]);
        }
    }

    while (up_convex.size() >= 2 &&
           !IsClockwise(up_convex[up_convex.size() - 2], up_convex[up_convex.size() - 1], points.back())) {
        up_convex.pop_back();
    }
    up_convex.push_back(points.back());

    while (dn_convex.size() >= 2 &&
           !IsCClockwise(dn_convex[dn_convex.size() - 2], dn_convex[dn_convex.size() - 1], points.back())) {
        dn_convex.pop_back();
    }

    for (std::size_t i = dn_convex.size() - 1; i > 0; --i) {
        up_convex.push_back(dn_convex[i]);
    }
    return up_convex;
}

/**
 * @brief Doubled square of convex,
 * 
 * @param convex - convex.
 * @return T - doubled square
 */
template <class T>
T ConvexSquare2(const std::vector<Vector2<T>>& convex) {
    T result = 0;
    for (std::size_t i = 1; i < convex.size() - 1; ++i) {
        result += std::abs(OrientedSquare2(convex.front(), convex[i], convex[i + 1]));
    }
    return result;
}

}

#endif /* ENGINE_GEOMETRY_HPP */
