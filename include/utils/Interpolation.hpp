#pragma once

#include <chrono>
#include <cmath>

enum class InterpolationType {
    linear,
    easeInOutElastic,
    easeOutCubic
};

inline float easeInOutElastic(float x) {
    const float c5 = (2 * 3.14159f) / 4.5f;
    if (x <= 0.0f)
        return 0.0f;
    else if (x >= 1.0f)
        return 1.0f;
    else if (x < 0.5f)
        return -0.5f * std::pow(2.0f, 20.0f * x - 10.0f) *
               std::sin((20.0f * x - 11.125f) * c5);
    else
        return 0.5f * std::pow(2.0f, -20.0f * x + 10.0f) *
               std::sin((20.0f * x - 11.125f) * c5) + 1.0f;
}

inline float easeOutCubic(float x) {
    return 1.0f - std::pow(1.0f - x, 3.0f);
}

inline float linear(float x) {
    return x;
}

template<typename T>
struct Interpolated {
    T start{};
    T end{};

    double startTime{};   // now double for high precision
    double speed{1.0};    // 1/duration

    InterpolationType interpolationType{InterpolationType::linear};

    // App start point (relative clock)
    inline static const auto appStart = std::chrono::steady_clock::now();

    explicit Interpolated(const T& initialValue = {})
        : start(initialValue), end(start) {}

    [[nodiscard]]
    static double GetCurrentTime() {
        return std::chrono::duration<double>(
            std::chrono::steady_clock::now() - appStart
        ).count(); // relative seconds, double precision
    }

    [[nodiscard]]
    float GetElapsedTime() const {
        double t = (GetCurrentTime() - startTime) * speed;
        if (t < 0.0) t = 0.0;
        if (t > 1.0) t = 1.0;

        switch (interpolationType) {
            case InterpolationType::easeInOutElastic: return easeInOutElastic(static_cast<float>(t));
            case InterpolationType::easeOutCubic:     return easeOutCubic(static_cast<float>(t));
            default:                                  return linear(static_cast<float>(t));
        }
    }

    void setValue(const T& value) {
        start = getValue();
        end = value;
        startTime = GetCurrentTime();
    }

    void setDuration(double duration) {
        if (duration <= 0.0) {
            speed = 1.0;
        } else {
            speed = 1.0 / duration;
        }
    }

    [[nodiscard]]
    T getValue() const {
        float elapsed = GetElapsedTime();
        return start + (end - start) * elapsed;
    }

    [[nodiscard]]
    bool is_done() const {
        double rawProgress = (GetCurrentTime() - startTime) * speed;
        return rawProgress >= 1.0;
    }

    [[nodiscard]]
    operator T() const {
        return getValue();
    }

    void operator=(const T& value) {
        setValue(value);
    }
};
