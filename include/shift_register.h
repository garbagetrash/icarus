#pragma once

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

} // namespace Icarus
