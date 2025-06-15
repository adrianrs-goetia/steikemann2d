#pragma once

#include <chrono>

class TimeStamp {
	std::chrono::steady_clock::time_point m_timestamp;

public:
	auto stamp() -> void {
		m_timestamp = std::chrono::steady_clock::now();
	}

	auto time_since_stamp() -> float {
		float duration_since_timestamp
			= std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - m_timestamp)
				  .count();
		return (duration_since_timestamp / 1e9);
	}

	auto in_range(float t_time_range_in_seconds) -> bool {
		auto duration_since_timestamp = time_since_stamp();
		return (duration_since_timestamp / 1e9) < t_time_range_in_seconds;
	}
};
