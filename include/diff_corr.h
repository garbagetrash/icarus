#pragma once

#include <algorithm>
#include <complex>
#include <vector>

#include "shift_register.h"

namespace Icarus {

class DifferentialCorrelator {
public:
  DifferentialCorrelator(const vec_cf32 &reference);

  float push(cf32& x);

private:
  vec_cf32 m_reference;
  ShiftRegister<cf32> m_buffer;
  float m_last;
};

} // namespace Icarus
