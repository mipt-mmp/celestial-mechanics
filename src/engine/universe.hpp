#ifndef ENGINE_UNIVERSE_HPP
#define ENGINE_UNIVERSE_HPP

#include "materialpoint.hpp"
namespace phys {

const Time defaultDeltaTime = 1e4_sec;

class Universe {
    std::vector<MaterialPoint*> mps_;
    Time time;
    public:
        void addMaterialPoint(MaterialPoint* mp) {mps_.push_back(mp);}

        void simulateStep(Time dt = defaultDeltaTime);
        
        Energy getEnergy() const;
    private:
        Energy getPotentialGravitationEnergy() const;

        void applyGravitation();

};

}

#endif /* ENGINE_UNIVERSE_HPP */
