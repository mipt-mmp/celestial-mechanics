#ifndef ENGINE_UNITS_HPP
#define ENGINE_UNITS_HPP
#include "geometry.hpp"
#include <compare>
#include <concepts>
#include <cstdint>

#define PHYS_UNITS_PROVIDE_LITERALS //FIXME: Compile option

#ifndef PHYS_UNITS_DEFAULT_TYPE
#define PHYS_UNITS_DEFAULT_TYPE long double
#endif

namespace phys {

const std::size_t UniverseDim = 2;

namespace detail {
    class UnitBase{}; // Needed for fast abstraction
}

template<class T>
concept SomeUnit = std::derived_from<T, detail::UnitBase>;


template<typename Num, int Meter = 0, int Sec = 0, int KG = 0, int K = 0, int Mole = 0, int Ampere = 0, int Candela = 0>
class Unit : public detail::UnitBase{
    Num num_;
public:
    constexpr Unit() : num_(0) {}
    constexpr explicit Unit(const Num &t) : num_(t) {}

    constexpr       Num &operator*()       { return num_; }
    constexpr const Num &operator*() const { return num_; }

    constexpr       Num *operator->()       { return &num_; }
    constexpr const Num *operator->() const { return &num_; }

    constexpr Unit &operator+=(const Unit &rhs) {
        num_ += rhs.num_;
        return *this;
    }

    constexpr Unit operator+(const Unit& rhs) const {
        return Unit{*this} += rhs;
    }

    constexpr Unit& operator-=(const Unit& rhs) {
        num_ -= rhs.num_;
        return *this;
    }

    constexpr Unit operator-(const Unit& rhs) const {
        return Unit{*this} -= rhs;
    }

    constexpr Unit operator-() {
        return Unit{-num_};
    }

    constexpr Unit& operator*=(const Num& num) {
        num_ *= num;
        return *this;
    }

    constexpr Unit& operator-=(const Num& num) {
        num_ -= num;
        return *this;
    }

    constexpr auto operator<=>(const Unit& rhs) const -> decltype(num_ <=> rhs.num_) {
        return num_ <=> rhs.num_;
    }
};

template<typename Num, int Meter1, int Sec1, int KG1, int K1, int Mole1, int Ampere1, int Candela1,
                       int Meter2, int Sec2, int KG2, int K2, int Mole2, int Ampere2, int Candela2>
constexpr auto operator*(const Unit<Num, Meter1, Sec1, KG1, K1, Mole1, Ampere1, Candela1>& lhs, 
               const Unit<Num, Meter2, Sec2, KG2, K2, Mole2, Ampere2, Candela2>& rhs) -> 
               Unit<Num, Meter1 + Meter2, Sec1 + Sec2, KG1 + KG2, K1 + K2, Mole1 + Mole2, Ampere1 + Ampere2, Candela1 + Candela2>
{
    return Unit<Num, Meter1 + Meter2, Sec1 + Sec2, KG1 + KG2, K1 + K2, Mole1 + Mole2, Ampere1 + Ampere2, Candela1 + Candela2>{*lhs * *rhs};
}

template<typename Num, int Meter1, int Sec1, int KG1, int K1, int Mole1, int Ampere1, int Candela1,
                       int Meter2, int Sec2, int KG2, int K2, int Mole2, int Ampere2, int Candela2>
constexpr auto operator/(const Unit<Num, Meter1, Sec1, KG1, K1, Mole1, Ampere1, Candela1>& lhs, 
               const Unit<Num, Meter2, Sec2, KG2, K2, Mole2, Ampere2, Candela2>& rhs) -> 
               Unit<Num, Meter1 - Meter2, Sec1 - Sec2, KG1 - KG2, K1 - K2, Mole1 - Mole2, Ampere1 - Ampere2, Candela1 - Candela2>
{
    return Unit<Num, Meter1 - Meter2, Sec1 - Sec2, KG1 - KG2, K1 - K2, Mole1 - Mole2, Ampere1 - Ampere2, Candela1 - Candela2>{*lhs / *rhs};
}

template<typename Num, int Meter , int Sec, int KG, int K, int Mole, int Ampere, int Candela>
constexpr auto sqrt(const Unit<Num, Meter, Sec, KG, K, Mole, Ampere, Candela>& u) -> Unit<Num, Meter/2, Sec/2, KG/2, K/2, Mole/2, Ampere/2, Candela/2>
{
    static_assert(Meter   % 2 == 0, "Bad sqrt");
    static_assert(Sec     % 2 == 0, "Bad sqrt");
    static_assert(KG      % 2 == 0, "Bad sqrt");
    static_assert(K       % 2 == 0, "Bad sqrt");
    static_assert(Mole    % 2 == 0, "Bad sqrt");
    static_assert(Ampere  % 2 == 0, "Bad sqrt");
    static_assert(Candela % 2 == 0, "Bad sqrt");
    return Unit<Num, Meter/2, Sec/2, KG/2, K/2, Mole/2, Ampere/2, Candela/2>{std::sqrt(*u)};
}

template<SomeUnit T>
struct UnitTraits {
    static auto sqrt(const T& t) -> decltype(phys::sqrt(t)) {
        return phys::sqrt(t);
    }
};

using num_t = PHYS_UNITS_DEFAULT_TYPE;

using LengthVal       = Unit<num_t, 1>;         // m

using VelocityVal     = Unit<num_t, 1, -1>;     // m * s^-1

using AccelerationVal = Unit<num_t, 1, -2>;     // m * s^-2

using ForceVal        = Unit<num_t, 1, -2, 1>;  // kg * m * s^-2

using TimeVal         = Unit<num_t, 0, 1, 0>;   // s

using MassVal         = Unit<num_t, 0, 0, 1>;   // kg

using Length = LengthVal;

template<SomeUnit T>
using Vector = geom::Vector<T, UniverseDim, UnitTraits>;

using Position = Vector<LengthVal>;
using Distance = Vector<LengthVal>;

using Velocity     = Vector<VelocityVal>;
using Acceleration = Vector<AccelerationVal>;
using Force        = Vector<ForceVal>;

using Time = TimeVal;
using Mass = MassVal;

template<SomeUnit T, SomeUnit U>
auto operator*(const Vector<T>& lhs, const U& rhs) -> Vector<decltype(lhs[0] * rhs)>{
    Vector<decltype(lhs[0] * rhs)> ans{};
    for(size_t i = 0; i < UniverseDim; ++i) {
        ans[i] = lhs[i] * rhs;
    }
    return ans;
}

template<SomeUnit T, SomeUnit U>
auto operator/(const Vector<T>& lhs, const U& rhs) -> Vector<decltype(lhs[0] / rhs)>{
    Vector<decltype(lhs[0] / rhs)> ans{};
    for(size_t i = 0; i < UniverseDim; ++i) {
        ans[i] = lhs[i] / rhs;
    }
    return ans;
}

template<SomeUnit T>
auto Normalize(const Vector<T>& t) {
    assert(*t.Len() > 0.);
    return t / t.Len();
}

}

#if defined(PHYS_UNITS_PROVIDE_LITERALS) && defined (PHYS_UNITS_DEFAULT_TYPE)
#define PHYS_UNIT_LITERAL(U, Literal)\
constexpr phys::U operator""_##Literal(long double x) {return phys::U{x};} \
constexpr phys::U operator""_##Literal(unsigned long long x) {return phys::U{x};}

PHYS_UNIT_LITERAL(Length, m)
PHYS_UNIT_LITERAL(Time  , sec)
PHYS_UNIT_LITERAL(Mass  , kg)
PHYS_UNIT_LITERAL(ForceVal , H)
#endif

template<class OStream, typename Num, int Meter, int Sec, int KG, int K, int Mole, int Ampere, int Candela>
OStream& operator<<(OStream& out, const phys::Unit<Num, Meter, Sec, KG, K, Mole, Ampere, Candela>& u) {
    out << *u << ' ';

#define PRINT_UNIT(Unit, Name)              \
    do{                                     \
        if constexpr (Unit != 0) {          \
            out << Name;                    \
            if constexpr (Unit != 1) {      \
                out << "^" << Unit << ' ';  \
            }                               \
        }                                   \
    }while(0)

    PRINT_UNIT(Meter,   "m"  );
    PRINT_UNIT(Sec,     "s"  );
    PRINT_UNIT(KG,      "kg" );
    PRINT_UNIT(K,       "K"  );
    PRINT_UNIT(Mole,    "mol");
    PRINT_UNIT(Ampere,  "A"  );
    PRINT_UNIT(Candela, "cd" );
#undef PRINT_UNIT
    return out;
}


#endif /* ENGINE_UNITS_HPP */
