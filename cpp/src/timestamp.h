#pragma once

#include <chrono>

class TimeStamp {
	std::chrono::steady_clock::time_point m_timestamp;

public:
	auto stamp() -> void {
		m_timestamp = std::chrono::steady_clock::now();
	}

	auto time_since_stamp() -> float {
		const auto duration_since_timestamp
			= std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - m_timestamp)
				  .count();
		return (static_cast<float>(duration_since_timestamp) / 1e9);
	}

	auto in_range(float t_time_range_in_seconds) -> bool {
		return time_since_stamp() < t_time_range_in_seconds;
	}
};
