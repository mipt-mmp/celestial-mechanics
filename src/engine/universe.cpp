#include "universe.hpp"
#include "physconstants.hpp"

namespace phys {

Time Universe::getTime() const
{
    return m_time;
}

void Universe::simulateStep(Time dt) {
    applyGravitation();
    
    for(auto* mp: m_mps) {
        mp->move(dt);
    }

    m_time += dt;
}

void Universe::applyGravitation() {
    for(size_t i = 0; i < m_mps.size(); ++i) {
        for(size_t j = 0; j < m_mps.size(); ++j) {
            if(i == j) continue;
            auto direction = -Normalize(m_mps[i]->getPos() - m_mps[j]->getPos());
            assert(std::abs(*direction.Len2() - 1) < 1e-4l);
            auto dist      =          (m_mps[i]->getPos() - m_mps[j]->getPos()).Len2();
            assert(*dist > 0.l);
            Force f = direction * phys::consts::G * m_mps[i]->getMass() * m_mps[j]->getMass() / dist;
            m_mps[i]->applyForce(f);
        }
    }
}

Energy Universe::getEnergy() const {
    Energy e{};
    for(const auto* mp: m_mps) {
        e += mp->getKinetic();
    }
    e += getPotentialGravitationEnergy();

    return e;
}

Energy Universe::getPotentialGravitationEnergy() const {
    Energy e;
    for(size_t i = 0; i < m_mps.size(); ++i) {
        for(size_t j = i+1; j < m_mps.size(); ++j) {
            auto dist = (m_mps[i]->getPos() - m_mps[j]->getPos()).Len();
            e += -phys::consts::G * m_mps[i]->getMass() * m_mps[j]->getMass() / dist;
        }
    }
    return e;
}



}
