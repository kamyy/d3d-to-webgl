#pragma once
#ifndef Timer_h
#define Timer_h

namespace d3d11demo {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    class Timer {
    public:
        Timer(float sampleInterval = 0.5f);
        void reset(float sampleInterval = 0.5f);
        void begTiming();
        void endTiming();

        float getFrameTime() const {
            return m_frameTime;
        }

        float getAvgTimePerFrame() const {
            return m_avgTimePerFrame;
        }

        float getAvgFramesPerSec() const {
            return m_avgFramesPerSec;
        }

    private:
        double m_sampleInterval;

        int64_t m_begTick;
        int64_t m_endTick;
        int64_t m_frequency;
        int64_t m_tickTotal;
        int64_t m_loopTotal;

        float m_frameTime;
        float m_avgTimePerFrame;
        float m_avgFramesPerSec;
    };
}

#endif
