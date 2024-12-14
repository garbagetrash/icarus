#include <algorithm>
#include <complex>
#include <vector>

namespace Icarus {

typedef std::complex<float> cf32;
typedef std::vector<cf32> vec_cf32;
const cf32 I(0.0, 1.0);

template <typename T>
class ShiftRegister {
public:
    ShiftRegister() : m_idx(0) {};
    ShiftRegister(size_t n) : m_idx(0), m_n(2*n) {
        m_buffer = std::vector<T>(2*n, 0);
    };
    ShiftRegister(std::vector<T>& v) : m_idx(0), m_buffer(v) {
        m_buffer.insert(m_buffer.end(), v.begin(), v.end());
        m_n = m_buffer.size();
    };

    void push(T x) {
        m_idx = (m_idx > 0) ? m_idx - 1 : m_n - 1;
        m_buffer[m_idx] = x;
        m_buffer[m_idx + m_n] = x;
    }

    T operator[](size_t idx) {
        return m_buffer[m_idx + idx];
    }

    size_t size() { return m_n; }

private:
    size_t m_n;
    size_t m_idx;
    std::vector<T> m_buffer;
};

class DifferentialCorrelator {
public:
  DifferentialCorrelator(const vec_cf32 &reference)
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

  float push(cf32& x) {
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

private:
  vec_cf32 m_reference;
  ShiftRegister<cf32> m_buffer;
  float m_last;
};

} // namespace Icarus
