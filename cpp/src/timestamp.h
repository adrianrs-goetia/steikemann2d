#pragma once

#include <chrono>

class TimeStamp {
	std::chrono::steady_clock::time_point m_timestamp;

public:
	auto stamp() -> void {
		m_timestamp = std::chrono::steady_clock::now();
	}

	auto in_range(float t_time_range_in_seconds) -> bool {
		float duration_since_timestamp
			= std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - m_timestamp)
				  .count();
		float sec = (duration_since_timestamp / 1e9);
		return (duration_since_timestamp / 1e9) < t_time_range_in_seconds;
	}
};
