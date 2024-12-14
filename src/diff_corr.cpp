#include <algorithm>
#include <complex>
#include <vector>

#include "diff_corr.h"
#include "shift_register.h"

namespace Icarus {

DifferentialCorrelator::DifferentialCorrelator(const vec_cf32 &reference)
{
  if (m_reference.size() < 2) {
    throw std::runtime_error("input must be at least length 2");
  }
  m_reference = reference;
  m_buffer = ShiftRegister<cf32>(2 * reference.size());
  std::transform(m_reference.begin(), m_reference.end(), m_reference.begin(),
                 [](auto x) { return std::exp(I * std::arg(x)); });
  for (size_t i = 1; i < m_reference.size(); i++) {
    m_reference[i - 1] = m_reference[i] - m_reference[i - 1];
  }
  m_reference.pop_back();
  m_last = 0.0f;
};

float DifferentialCorrelator::push(cf32& x) {
    float temp = std::arg(x);
    m_buffer.push(std::exp(I * (temp - m_last)));
    m_last = temp;

    cf32 output(0.0, 0.0);
    for (size_t i = 0; i < m_reference.size(); i++)
    {
      output += m_buffer[i] * m_reference[m_reference.size() - 1 - i];
    }
    return std::abs(output);
};

} // namespace Icarus
