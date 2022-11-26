#pragma once

#include <string>
#include <iostream>
#include <cmath>
#include <vector>
#include <array>
#include <algorithm>

using std::string;
using std::pair;
using std::vector;
using std::array;

using Path = array<int, 5>; // from x, y, to x, y, speed

struct MovableObject
{
	pair<double, double> getObjPosAtTimeInMeters(int time_ms) const
	{
		if (route.empty()) {
			return { 0, 0 };
		}

		const vector<int> time_offsets = calcTimeOffsets();

		const auto iter = std::upper_bound(
				time_offsets.begin(), time_offsets.end() - 1, time_ms);
		const auto last_time = time_offsets.back();

		const int pos = iter - time_offsets.begin();
		const int path_idx = pos - 1;
		const auto [from_x_m, from_y_m, to_x_m, to_y_m, speed_kmh] =
				route[path_idx];

		const auto partial_speeds = calcPartialSpeedInKmh(route[path_idx]);
		const auto [xspeed_kmh, yspeed_kmh] = partial_speeds;

		double x_m = from_x_m
				+ xspeed_kmh * (time_ms - time_offsets[path_idx]) / 3600.0;
		double y_m = from_y_m
				+ yspeed_kmh * (time_ms - time_offsets[path_idx]) / 3600.0;

		if (time_ms > last_time) {
			x_m = to_x_m;
			y_m = to_y_m;
		}
		return { x_m, y_m };
	}

	static pair<double, double> calcPartialSpeedInKmh(Path path)
	{
		const auto [from_x_m, from_y_m, to_x_m, to_y_m, speed_kmh] = path;
		const int dx = to_x_m - from_x_m;
		const int dy = to_y_m - from_y_m;
		if (dx == 0) {
			return { 0, (dy >= 0) ? speed_kmh : -speed_kmh };
		}
		if (dy == 0) {
			return { (dx >= 0) ? speed_kmh : -speed_kmh, 0 };
		}

		const double x = std::abs(dx / static_cast<double>(dy));
		double xspeed_kmh = speed_kmh * x * sqrt(1 + x * x) / (1 + x * x);
		double yspeed_kmh = speed_kmh * sqrt(1 + x * x) / (1 + x * x);

		if (dx < 0) {
			xspeed_kmh = -xspeed_kmh;
		}
		if (dy < 0) {
			yspeed_kmh = -yspeed_kmh;
		}
		return { xspeed_kmh, yspeed_kmh };
	}

	vector<int> calcTimeOffsets() const
	{
		vector<int> offset_times{ time_start_ms };
		for (const auto &path : route) {
			const auto [from_x_m, from_y_m, to_x_m, to_y_m, speed_kmh] = path;
			const int dx = abs(from_x_m - to_x_m);
			const int dy = abs(from_y_m - to_y_m);
			const double distance_m = hypot(dx, dy);
			const double speed_mps = speed_kmh / 3.6;
			const int time_ms = static_cast<int>(distance_m / speed_mps * 1000);
			const int total_time = offset_times.back() + time_ms;
			offset_times.push_back(total_time);
		}
		return offset_times;
	}

	string obj_name;
	string type;
	double width_m;
	double height_m;
	int time_start_ms{};
	vector<Path> route;
};