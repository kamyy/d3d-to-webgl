#pragma once
#ifndef Timer_h
#define Timer_h

namespace eve {
	class Timer {
	public:
		Timer(f32 sampleInterval = 0.5f);
		void reset(f32 sampleInterval = 0.5f);
		void begTiming();
		void endTiming();

		f32 getFrameTime() const { 
			return m_frameTime; 
		}
		f32 getAvgTimePerFrame() const { 
			return m_avgTimePerFrame; 
		}
		f32 getAvgFramesPerSec() const { 
			return m_avgFramesPerSec; 
		}
	private:
		double m_sampleInterval;
		__int64 m_begTick;
		__int64 m_endTick;
		__int64 m_frequency;
		__int64 m_tickTotal;
		__int64 m_loopTotal;
		f32 m_frameTime;
		f32 m_avgTimePerFrame;
		f32 m_avgFramesPerSec;
	};
}

#endif