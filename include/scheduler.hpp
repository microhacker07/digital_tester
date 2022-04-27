#pragma once

#include <cstdint>

// Originially from "scheduler.hpp" by Owen 'optorres' Torres from UB SEDS github 'flight-computer-firmware'
// https://github.com/University-at-Buffalo-SEDS/Flight-Computer-Firmware/blob/stm32/include/scheduler.hpp

typedef void (*SchedCb)();

enum class TaskId : uint8_t {
	// The order of these entries represents a sort of priority system,
	// in that the first elements will always be checked first.
	Clock,
	Button,
	Blink,
	Count,
};

class Task {
public:
	Task(SchedCb cb, uint32_t period_us) :
		callback(cb),
		last_run_us(0),
		period_us(period_us)
	{}

	Task() :
		callback(nullptr),
		last_run_us(0),
		period_us(0)
	{}

	SchedCb callback;
	uint32_t last_run_us, period_us;
};

void scheduler_add(TaskId tid, const Task &task);

// Runs scheduled tasks.  Returns the micros time when the next task is scheduled.
uint32_t schedule();
