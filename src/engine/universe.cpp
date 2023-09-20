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
            auto direction = -Normalize(mps_[i]->getPos() - mps_[j]->getPos());
            assert(std::abs(*direction.Len2() - 1) < 1e-4l);
            auto dist      =          (mps_[i]->getPos() - mps_[j]->getPos()).Len2();
            assert(*dist > 0.l);
            Force f = direction * phys::consts::G * mps_[i]->getMass() * mps_[j]->getMass() / dist;
            mps_[i]->applyForce(f);
        }
    }
}

Energy Universe::getEnergy() const {
    Energy e{};
    for(const auto* mp: mps_) {
        e += mp->getKinetic();
    }
    e += getPotentialGravitationEnergy();
    return e;
}

Energy Universe::getPotentialGravitationEnergy() const {
    Energy e;
    for(size_t i = 0; i < mps_.size(); ++i) {
        for(size_t j = i+1; j < mps_.size(); ++j) {
            auto dist = (mps_[i]->getPos() - mps_[j]->getPos()).Len();
            e += -phys::consts::G * mps_[i]->getMass() * mps_[j]->getMass() / dist;
        }
    }
    return e;
}



}
