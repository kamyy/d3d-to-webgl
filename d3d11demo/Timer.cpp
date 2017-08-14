#include "StdAfx.h"
#include "Timer.h"

namespace d3d11demo {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    Timer::Timer(float interval) {
        reset(interval);
    }

    void Timer::reset(float interval) {
        QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
        m_sampleInterval = interval;
        m_begTick = 0;
        m_endTick = 0;
        m_tickTotal = 0;
        m_loopTotal = 0;
        m_frameTime = 0.0f;
        m_avgTimePerFrame = 0.0f;
        m_avgFramesPerSec = 0.0f;
    }

    void Timer::begTiming() {
        QueryPerformanceCounter((LARGE_INTEGER*)&m_begTick);
    }

    void Timer::endTiming() {
        QueryPerformanceCounter((LARGE_INTEGER*)&m_endTick);

        const __int64 ticks = m_endTick - m_begTick;
        m_loopTotal += 1;
        m_tickTotal += ticks;

        const double s = double(m_tickTotal / m_frequency);
        const double l = double(m_loopTotal);
        if (s > m_sampleInterval) {
            m_avgTimePerFrame = float(s / l);
            m_avgFramesPerSec = float(l / s);
            m_tickTotal = 0;
            m_loopTotal = 0;
        }

        m_frameTime = float(ticks / m_frequency);
    }

}
