#ifndef ENGINE_UNIVERSE_HPP
#define ENGINE_UNIVERSE_HPP

#include "materialpoint.hpp"
namespace phys {

const Time defaultDeltaTime = 1e4_sec;

class Universe {
  std::vector<MaterialPoint *> m_mps;
  Time m_time;

  mutable Energy m_startEnergy{NAN};

public:
  void addMaterialPoint(MaterialPoint *mp) { m_mps.push_back(mp); }

  void simulateStep(Time dt = defaultDeltaTime);

  Energy getEnergy() const;

  [[nodiscard]] Time getTime() const;

  private:
  Energy getPotentialGravitationEnergy() const;

  void applyGravitation();
};

}

#endif /* ENGINE_UNIVERSE_HPP */
