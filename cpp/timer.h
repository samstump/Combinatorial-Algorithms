// timer.h

#ifndef _TIMER_H_INCLUDED_
#define _TIMER_H_INCLUDED_

#include <chrono>

//TODO: add an accumulate time feature to the timer to accumulate discontinuous measurements.

#ifdef Windows
// create the missing high_resolution_clock in Windows
#include <windows.h>

namespace
{
	const long long g_Frequency = []() -> long long 
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		return frequency.QuadPart;
	}();
}

struct HighResClock
{
	typedef long long                               rep;
	typedef std::nano                               period;
	typedef std::chrono::duration<rep, period>      duration;
	typedef std::chrono::time_point<HighResClock>   time_point;
	
	static const bool is_steady = true;
	
	static time_point now()
	{
		LARGE_INTEGER count;
		QueryPerformanceCounter(&count);
		return time_point(duration(count.QuadPart * static_cast<rep>(period::den) / g_Frequency));
	}	
};

typedef HighResClock clock_type;

#else
// all other platforms should be good
typedef std::chrono::high_resolution_clock clock_type;

#endif

class timer {
	public:
		typedef std::chrono::nanoseconds duration;
		
		timer(bool start_now = false) : t0(), t1()
		{
			running = start_now;
		}
		
		void reset()
		{
			t0 = clock_type::time_point();
			t1 = clock_type::time_point();
			running = false;
		}
		
		void start()
		{
			running = true;
			t0 = clock_type::now();
			t1 = t0;
		}
		
		duration stop()
		{
			running = false;
			t1 = clock_type::now();
			return (t1-t0);
		}
		
		duration elapsed()
		{
			if (running) {
				return (clock_type::now() - t0);
			} else {
				return (t1-t0);
			}
		}
		
		static double to_nanoseconds(const duration& d) { 
			return double(d.count());
		} 
		static double to_microseconds(const duration& d) {
			return std::chrono::duration_cast<std::chrono::duration<double, std::chrono::microseconds::period>>(d).count();
		}
		static double to_milliseconds(const duration& d) {
			return std::chrono::duration_cast<std::chrono::duration<double, std::chrono::milliseconds::period>>(d).count();
		}
		
	private:
		clock_type::time_point t0;
		clock_type::time_point t1;
		bool running;
};

#endif //_TIMER_H_INCLUDED

