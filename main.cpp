#include "json_reader.h"
#include "render.h"
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
	if (argc == 1) {
		cout << "Please enter a scenario file\n";
		return 1;
	}

	const char *scenario_file = argv[1];
	ifstream file(scenario_file);
	if (!file.is_open()) {
		cout << "Scenario file is not found or busy\n";
		return 1;
	}
	const string output_file = (argc == 3) ? argv[2] : "./motion.avi";

	const ParsedInput parsed_input = ParseJson(LoadJSON(file));
	file.close();

	const int frame_width = parsed_input.frame_settings.width;
	const int frame_width_m = parsed_input.frame_settings.width_m;
	const int frame_height = parsed_input.frame_settings.height;

	Render render(frame_height, frame_width, frame_width_m);

	if (!render.initWriter(output_file)) {
		exit(1);
	};

	const auto &movable_objects = parsed_input.movable_objects;
	const auto &static_objects = parsed_input.static_objects;
	constexpr int total_time_ms = 10000;
	for (int time_ms = 0; time_ms < total_time_ms; time_ms += 40) {
		render.drawStaticObjects(static_objects);
		for (const auto &object : movable_objects) {
			render.drawMovableObject(object, time_ms);
		}
		render.appendFrameToWriter();
		for (const auto &object : movable_objects) {
			render.clearObject(object, time_ms);
		}
	}

	return 0;
}
