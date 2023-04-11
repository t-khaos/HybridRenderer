#pragma once

#include "Just/Common.h"

struct DiscretePDF {
private:
    std::vector<float> m_cdf;
    float m_sum{};
    float m_normalization{};
    bool m_normalized = false;
public:
    explicit DiscretePDF(size_t nEntries = 0) {
        Reserve(nEntries);
        clear();
    }

    void clear() {
        m_cdf.clear();
        m_cdf.push_back(0.0f);
        m_normalized = false;
    }

    void Reserve(size_t nEntries) {
        m_cdf.reserve(nEntries+1);
    }

    void Append(float pdfValue) {
        m_cdf.push_back(m_cdf[m_cdf.size()-1] + pdfValue);
    }

    size_t size() const {
        return m_cdf.size()-1;
    }

    float operator[](size_t entry) const {
        return m_cdf[entry+1] - m_cdf[entry];
    }

    bool isNormalized() const {
        return m_normalized;
    }

    float getSum() const {
        return m_sum;
    }

    float GetNormalization() const {
        return m_normalization;
    }

    float Normalize() {
        m_sum = m_cdf[m_cdf.size()-1];
        if (m_sum > 0) {
            m_normalization = 1.0f / m_sum;
            for (size_t i=1; i<m_cdf.size(); ++i)
                m_cdf[i] *= m_normalization;
            m_cdf[m_cdf.size()-1] = 1.0f;
            m_normalized = true;
        } else {
            m_normalization = 0.0f;
        }
        return m_sum;
    }

    size_t sample(float sampleValue) const {
        std::vector<float>::const_iterator entry =
                std::lower_bound(m_cdf.begin(), m_cdf.end(), sampleValue);
        size_t index = (size_t) std::max((ptrdiff_t) 0, entry - m_cdf.begin() - 1);
        return std::min(index, m_cdf.size()-2);
    }

    size_t sample(float sampleValue, float &pdf) const {
        size_t index = sample(sampleValue);
        pdf = operator[](index);
        return index;
    }

    size_t sampleReuse(float &sampleValue) const {
        size_t index = sample(sampleValue);
        sampleValue = (sampleValue - m_cdf[index])
                      / (m_cdf[index + 1] - m_cdf[index]);
        return index;
    }

    size_t sampleReuse(float &sampleValue, float &pdf) const {
        size_t index = sample(sampleValue, pdf);
        sampleValue = (sampleValue - m_cdf[index])
                      / (m_cdf[index + 1] - m_cdf[index]);
        return index;
    }
};
