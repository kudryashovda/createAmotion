#pragma once

#include <vector>
#include <array>
#include <string>

using std::vector;
using std::array;
using std::string;

using Point = array<int, 2>;

struct Line {
	string name;
	string type;
	Point start_pos{};
	Point end_pos{};
	int thickness{};
	int period{};
	double duty_cycle = 1.0;
};

struct Circle {
	string name;
	Point pos{};
	int radius{};
	int thickness{};
};

struct StaticObjects {
	vector<Line> lines;
	vector<Circle> circles;
};