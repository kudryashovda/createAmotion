#include "json_reader.h"

using namespace std::literals;
using std::move;

json::Document LoadJSON(std::istream &input)
{
	return json::Load(input);
}

int GetDictValueByNameAsInt(const json::Dict &json_frame_settings,
		const string &name, int def_value)
{
	if (const auto it = json_frame_settings.find(name);
			it != json_frame_settings.end()) {
		return it->second.AsInt();
	}
	return def_value;
}

double GetDictValueByNameAsDouble(const json::Dict &json_frame_settings,
		const string &name, double def_value)
{
	if (const auto it = json_frame_settings.find(name);
			it != json_frame_settings.end()) {
		return it->second.AsDouble();
	}
	return def_value;
}

string GetDictValueByNameAsString(const json::Dict &json_frame_settings,
		const string &name, string def_value)
{
	if (const auto it = json_frame_settings.find(name);
			it != json_frame_settings.end()) {
		return it->second.AsString();
	}
	return def_value;
}

Point GetDictValueByNameAsPoint(const json::Dict &json_frame_settings,
		const string &name, Point def_value)
{
	if (const auto it = json_frame_settings.find(name);
			it != json_frame_settings.end()) {

		const auto &json_array = it->second.AsArray();
		Point point;
		int idx = 0;
		for (const auto &coord : json_array) {
			point[idx] = coord.AsInt();
			++idx;
		}
		return point;
	}
	return def_value;
}

ParsedInput ParseJson(const json::Document &document)
{
	ParsedInput parsed;

	const auto root = document.GetRoot().AsDict();

	if (const auto json_frame_settings_iter = root.find("frame_settings"s);
			json_frame_settings_iter != root.end()) {
		const auto &json_frame_settings =
				json_frame_settings_iter->second.AsDict();
		parsed.frame_settings.width =
				GetDictValueByNameAsInt(json_frame_settings, "width", 0);
		parsed.frame_settings.height =
				GetDictValueByNameAsInt(json_frame_settings, "height", 0);
		parsed.frame_settings.width_m =
				GetDictValueByNameAsInt(json_frame_settings, "width_m", 0);
	}

	if (const auto json_objects_iter = root.find("movable_objects"s);
			json_objects_iter != root.end()) {
		const auto &objects = json_objects_iter->second.AsArray();
		for (const auto &json_object : objects) {
			const auto &json_object_dict = json_object.AsDict();
			MovableObject movable_object;
			movable_object.obj_name = GetDictValueByNameAsString(
					json_object_dict, "name", "undefined");
			movable_object.type = GetDictValueByNameAsString(
					json_object_dict, "type", "rectangle");
			movable_object.width_m = GetDictValueByNameAsDouble(
					json_object_dict, "width", 1.0);
			movable_object.height_m = GetDictValueByNameAsDouble(
					json_object_dict, "height", 1.0);
			movable_object.time_start_ms = GetDictValueByNameAsInt(
					json_object_dict, "time_start_ms", 0);

			if (const auto it = json_object_dict.find("route");
					it != json_object_dict.end()) {
				const auto &json_array = it->second.AsArray();
				for (const auto &json_path : json_array) {
					Path path;
					int idx = 0;
					for (const auto &item : json_path.AsArray()) {
						path[idx] = item.AsInt();
						++idx;
					}
					movable_object.route.push_back(path);
				}
			}

			parsed.movable_objects.push_back(move(movable_object));
		}
	}

	if (const auto json_objects_iter = root.find("static_objects"s);
			json_objects_iter != root.end()) {
		const auto &objects = json_objects_iter->second.AsArray();
		for (const auto &json_object : objects) {
			const auto &json_object_dict = json_object.AsDict();
			const auto it = json_object_dict.find("type");
			if (it == json_object_dict.end()) {
				continue;
			}
			const string object_type = it->second.AsString();
			if (object_type == "solid_line" || object_type == "dashed_line") {
				Line line;
				line.name = GetDictValueByNameAsString(
						json_object_dict, "name", "undefined");
				line.type = object_type;
				line.start_pos = GetDictValueByNameAsPoint(
						json_object_dict, "start_pos", Point{ 0, 0 });
				line.end_pos = GetDictValueByNameAsPoint(
						json_object_dict, "end_pos", Point{ 0, 0 });
				line.thickness = GetDictValueByNameAsInt(
						json_object_dict, "thickness", 0);
				line.period = GetDictValueByNameAsInt(
						json_object_dict, "period", 0);
				line.duty_cycle = GetDictValueByNameAsDouble(
						json_object_dict, "duty_cycle", 1.0);
				parsed.static_objects.lines.push_back(move(line));
			} else if (object_type == "circle") {
				Circle circle;
				circle.name = GetDictValueByNameAsString(
						json_object_dict, "name", "undefined");
				circle.pos = GetDictValueByNameAsPoint(
						json_object_dict, "pos", Point{ 0, 0 });
				circle.radius =
						GetDictValueByNameAsInt(json_object_dict, "radius", 0);
				circle.thickness = GetDictValueByNameAsInt(
						json_object_dict, "thickness", 0);
				parsed.static_objects.circles.push_back(move(circle));
			}
		}
	}
	return parsed;
}
