#ifndef UNIT_H
#define UNIT_H

#include <ostream>
#include <sstream>

template <int KG, int M, int S>
struct Unit {};

template <typename...>
struct MultUnits {};

template <int LKG, int LM, int LS, int RKG, int RM, int RS>
struct MultUnits<Unit<LKG, LM, LS>, Unit<RKG, RM, RS>> {
  using unit = Unit<LKG + RKG, LM + RM, LS + RS>;
};

template <typename...>
struct DivUnits {};
template <int LKG, int LM, int LS, int RKG, int RM, int RS>
struct DivUnits<Unit<LKG, LM, LS>, Unit<RKG, RM, RS>> {
  using unit = Unit<LKG - RKG, LM - RM, LS - RS>;
};

using One = Unit<0, 0, 0>;
using Kilogram = Unit<1, 0, 0>;
using Meter = Unit<0, 1, 0>;
using Second = Unit<0, 0, 1>;
using Hertz = DivUnits<One, Second>::unit;
using MeterPerSecond = DivUnits<Meter, Second>::unit;
using MeterPerSecondSquared = DivUnits<MeterPerSecond, Second>::unit;
using Newton = MultUnits<Kilogram, MeterPerSecondSquared>::unit;
using Joule = MultUnits<Newton, Meter>::unit;

template <typename UNIT>
struct ExtractBaseUnits {};

template <template<int, int, int> class UNIT, int KG, int M, int S>
struct ExtractBaseUnits<UNIT<KG, M, S>> {
  static const int kg = KG;
  static const int m = M;
  static const int s = S;
};

template <typename UNIT>
constexpr auto GetKg() {
  return ExtractBaseUnits<UNIT>::kg;
}
template <typename UNIT>
constexpr auto GetM() {
  return ExtractBaseUnits<UNIT>::m;
}
template <typename UNIT>
constexpr auto GetS() {
  return ExtractBaseUnits<UNIT>::s;
}

template <typename UNIT>
auto RepresentUnit() {
  std::stringstream ss;
  ss << "kg^" << GetKg<UNIT>() << " m^" << GetM<UNIT>() << " s^" << GetS<UNIT>();
  return ss.str();
}




template <typename T, typename UNIT>
class Quantity {
 public:
  using unit = UNIT;
  const T value;

  explicit Quantity(T v) : value{std::move(v)} {}
  
  template <typename RUNIT,
            typename RES_UNIT = typename MultUnits<UNIT, RUNIT>::unit>
  auto operator*(const Quantity<T, RUNIT>& rhs) {
    return Quantity<T, RES_UNIT>{value * rhs.value};
  }

  template <typename RUNIT,
            typename RES_UNIT = typename DivUnits<UNIT, RUNIT>::unit>
  auto operator/(const Quantity<T, RUNIT>& rhs) {
    return Quantity<T, RES_UNIT>{value / rhs.value};
  }

  Quantity<T, UNIT> operator+(const Quantity<T, UNIT>& rhs) {
    return {value + rhs.value};
  };

  Quantity<T, UNIT> operator-(const Quantity<T, UNIT>& rhs) {
    return {value - rhs.value};
  };

  Quantity<T, UNIT> operator-() {
    return {-value};
  }
 private:

  template <typename _T, typename _UNIT>
  friend std::ostream& operator<<(std::ostream&, const Quantity<_T, _UNIT>&);

};

template <typename T, typename UNIT>
std::ostream& operator<<(std::ostream& os, const Quantity<T, UNIT>& q) {
  os << q.value << " " << RepresentUnit<UNIT>();
  return os;
}

template <typename T> using Mass = Quantity<T, Kilogram>;
template <typename T> using Distance = Quantity<T, Meter>;
template <typename T> using Time = Quantity<T, Second>;
template <typename T> using Velocity = Quantity<T, MeterPerSecond>;
template <typename T> using Acceleration = Quantity<T, MeterPerSecondSquared>;
template <typename T> using Force = Quantity<T, Newton>;
template <typename T> using Energy = Quantity<T, Joule>;

#endif // UNIT_H
