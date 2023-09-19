#ifndef ENGINE_UNIVERSE_HPP
#define ENGINE_UNIVERSE_HPP

#include "materialpoint.hpp"
namespace phys {


class Universe {
    std::vector<MaterialPoint> objects_;
    public:
        void simulate();
};

}

#endif /* ENGINE_UNIVERSE_HPP */
