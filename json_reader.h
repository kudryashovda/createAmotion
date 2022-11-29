#pragma once

#include "json.h"
#include "movable_object.h"
#include "static_objects.h"

using std::istream;
using std::vector;

struct FrameSettings
{
	int width{};
	int height{};
	int width_m{};
	int height_m{};
};

struct ParsedInput
{
	FrameSettings frame_settings;
	vector<MovableObject> movable_objects;
	StaticObjects static_objects;
};

json::Document LoadJSON(istream &input);

ParsedInput ParseJson(const json::Document &document);
int GetDictValueByNameAsInt(const json::Dict &json_frame_settings,
		const string &name, int def_value);
string GetDictValueByNameAsString(const json::Dict &json_frame_settings,
		const string &name, string def_value);
double GetDictValueByNameAsDouble(const json::Dict &json_frame_settings,
		const string &name, double def_value);
Point GetDictValueByNameAsPoint(const json::Dict &json_frame_settings,
		const string &name, Point def_value);