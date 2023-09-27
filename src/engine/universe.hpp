#ifndef ENGINE_UNIVERSE_HPP
#define ENGINE_UNIVERSE_HPP

#include "materialpoint.hpp"
namespace phys {

const Time defaultDeltaTime = 1e4_sec;

class Universe {
  std::vector<MaterialPoint *> m_mps;
  Time m_time;
  Time m_dt;

public:
  struct Metrics {
      Impulse impulse;
      Energy energy;
      ImpulseMoment impulsemoment;
      Time time;
  };

  Metrics getMetrics() const {
      return {
        getImpulse(),
        getEnergy(),
        getImpulseMoment(),
        m_time
      };
  }

  void addMaterialPoint(MaterialPoint *mp) { m_mps.push_back(mp); }

  Position getMassCenter() const;
  Velocity getVelocityCenter() const;

  void recalcOptimalDt();
  Time getOptimalDt();

  void shiftMassCenter();

  void simulateStep(Time dt = defaultDeltaTime);

  ImpulseMoment getImpulseMoment() const;
  Impulse getImpulse() const;
  Energy getEnergy() const;

  [[nodiscard]] Time getTime() const;

  private:
  Energy getPotentialGravitationEnergy() const;

  void applyGravitation();
};

}

#endif /* ENGINE_UNIVERSE_HPP */
