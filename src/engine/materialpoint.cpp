#include "materialpoint.hpp"

namespace phys {

void MaterialPoint::move(Time dt) {
    v_ += cur_f_ * dt / m_;
    pos_ += v_ * dt;
    
    reset();
}

void MaterialPoint::applyForce(Force f) {
    cur_f_ += f;
}

void MaterialPoint::reset() {
    cur_f_ = {};
}

}
