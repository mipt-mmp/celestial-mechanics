#include "materialpoint.hpp"

namespace phys {

void MaterialPoint::move(Time dt) {
    Acceleration a = cur_f_ / m_;
    auto j = (a - prev_a) / prev_dt;

    pos_ += v_ * dt + (a * dt * dt /= 2);// + (j * dt * dt * dt /= 6);

    v_ += a * dt;

    prev_a  = a;
    prev_dt = dt;

//    v_ += a * dt;
//    pos_ += v_ * dt + (a * dt * dt); //+ (j * dt * dt * dt /= 6);
    reset();
}

void MaterialPoint::applyForce(Force f) {
    cur_f_ += f;
}

void MaterialPoint::reset() {
    cur_f_ = {};
}

}
