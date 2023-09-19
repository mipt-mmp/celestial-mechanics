#ifndef ENGINE_MATERIALPOINT_HPP
#define ENGINE_MATERIALPOINT_HPP

#include "units.hpp"
#include "geometry.hpp"

namespace phys {

struct MaterialPoint {
    Position pos_;
    Velocity v_;
    Mass m_;
};

}

#endif /* ENGINE_MATERIALPOINT_HPP */
