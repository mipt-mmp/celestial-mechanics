#include "universe.hpp"
#include "physconstants.hpp"

namespace phys {

void Universe::simulateStep(Time dt) {
    applyGravitation();

    for(auto* mp: mps_) {
        mp->move(dt);
    }
}

void Universe::applyGravitation() {
    for(size_t i = 0; i < mps_.size(); ++i) {
        for(size_t j = 0; j < mps_.size(); ++j) {
            if(i == j) continue;
            auto direction = Normalize(mps_[i]->getPos() - mps_[j]->getPos());
            auto dist      =          (mps_[i]->getPos() - mps_[j]->getPos()).Len2();
            Force f = direction * phys::consts::G * mps_[i]->getMass() * mps_[j]->getMass() / dist;
            mps_[i]->applyForce(f);
        }
    }
}

}
