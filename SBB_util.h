/*
 * SBB_util.h
 *
 *  Created on: Oct 9, 2011
 *      Authors: Chuka (N19440839), Deniz (N19223762) and Anagha (N18869540)
 */

#ifndef SBB_UTIL_H_
#define SBB_UTIL_H_

#include <stdexcept> // std::runtime_error
#include <sys/time.h>

#include <sys/times.h>


#define START_TIMER(obj) \
	printf("start_clock in file: %s line: %d\n", __FILE__, __LINE__); \
	obj.start_clock();

#define END_TIMER(obj) \
	printf("end_clock in file: %s line: %d\n", __FILE__, __LINE__); \
	obj.end_clock();


class SBB_util
{
	public:
		SBB_util();
		void start_clock();
		double* end_clock();

	private:
		clock_t _start_time;
		clock_t _end_time;
		struct tms _start_cpu;
		struct tms _end_cpu;
};

#endif /* SBB_UTIL_H_*/
