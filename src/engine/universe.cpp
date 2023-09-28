#include "universe.hpp"
#include "physconstants.hpp"

namespace phys {

Time Universe::getTime() const
{
    return m_time;
}

void Universe::setFluctoationPerion(const Time& newFluctoationPerion)
{
    m_fluctoationPerion = newFluctoationPerion;
}

Distance Universe::getMassCenter() const {
    MassMoment mass_moments;
    Mass mass_sum;
    
    for (auto* mp : m_mps) {
        mass_moments += mp->getPos() * mp->getMass();
        mass_sum += mp->getMass();
    }

    return mass_moments / mass_sum;
}

Velocity Universe::getVelocityCenter() const {
    Impulse sum_impulse{};
    Mass mass_sum{};

    for (auto* mp : m_mps) {
        sum_impulse += mp->getVelocity() * mp->getMass();
        mass_sum    += mp->getMass();
    }

    return sum_impulse / mass_sum;
}

void Universe::fluctuate(num_t degree)
{
    auto* mp = m_mps[rand() % m_mps.size()];
    Force f = phys::random<ForceVal>();
    f *= degree;
    mp->applyForce(f);
}

void Universe::recalcOptimalDt() {

#if 0
    VelocityVal max_speed{};

    for (auto* mp : m_mps) {
        max_speed = std::max(mp->getVelocity().Len(), max_speed);
    }

    const LengthVal normalizer = 2.5e3_m;
    m_dt = normalizer / max_speed;
#endif

    num_t koef = INFINITY;

    for(const auto* mp : m_mps) {
        for(const auto* oth : m_mps) {
            if(oth == mp) continue;
            koef = std::min(koef, *(mp->getPos()-oth->getPos()).Len() / *(mp->getVelocity() - oth->getVelocity()).Len());
        }
    }

    m_dt = (1_sec *= koef * config::DT_NORMALIZER);
    m_dt = std::min(max_dt, std::max(min_dt, m_dt));
//    std::cout << m_dt << '\n';
}

Time Universe::getOptimalDt() {
    return m_dt;
}

Length Universe::getMaxDist() const
{
    Length maxLen = 0_m;
    for(size_t i = 0; i < m_mps.size(); ++i) {
        maxLen = std::max(maxLen, m_mps[i]->getPos().Len());
    }
    return maxLen;
}

void Universe::shiftMassCenter()
{
    Distance new_center = getMassCenter();
    Velocity shift_velocity = getVelocityCenter();

    for (auto* mp : m_mps) {
        mp->setPos(mp->getPos() - new_center);
        mp->setVelocity(mp->getVelocity() - shift_velocity);
    }
}

void Universe::simulateStep(Time dt) {
    applyGravitation();
    
    if((m_fluctoationPerion <=> 0_sec) != std::partial_ordering::equivalent && (m_time - m_lastFluctoation) > m_fluctoationPerion) {
        fluctuate(config::FLUCTATION_DEGREE);
        m_lastFluctoation = m_time;
    }
    recalcOptimalDt();
    dt = m_dt;
    for (auto* mp: m_mps) {
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

ImpulseMoment Universe::getImpulseMoment() const
{
    ImpulseMoment p{};
    for(const auto* mp: m_mps) {
        p += mp->getImpulseMoment();
    }

    return p;
}

Impulse Universe::getImpulse() const
{
    Impulse p{};
    for(const auto* mp: m_mps) {
        p += mp->getImpulse();
    }

    return p;
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
