#pragma once

#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdint>

// Originially from "util.hpp" by Owen 'optorres' Torres from UB SEDS github 'flight-computer-firmware'
// https://github.com/University-at-Buffalo-SEDS/Flight-Computer-Firmware/blob/stm32/include/util.hpp

inline static uint32_t delta(uint32_t old, uint32_t current)
{
	if (old > current) {
		return (UINT32_MAX - old) + current + 1;
	}
	return current - old;
}