#include "render.h"

void Render::drawLine(cv::Point start, cv::Point end, int thickness)
{
	const int lineType = cv::LINE_8;
	line(img_, start, end, cv::Scalar(255, 255, 255), thickness, lineType);
}

void Render::drawMovableObject(
		const MovableObject &object, int msec, const cv::Scalar &color)
{
	const auto [x_m, y_m] = object.getObjPosAtTimeInMeters(msec);
	const int x = static_cast<int>(x_m * hor_pixel_per_meter_);
	const int y = static_cast<int>(y_m * ver_pixel_per_meter_);

	const bool is_inside_frame = x <= img_.cols && y <= img_.rows && x >= 0
			&& y >= 0 && msec >= object.time_start_ms;

	if (!is_inside_frame) {
		return;
	}
	const int object_width =
			static_cast<int>(object.width_m * hor_pixel_per_meter_);
	const int object_height =
			static_cast<int>(object.height_m * ver_pixel_per_meter_);

	constexpr int thickness = 1;
	if (object.type == "rectangle") {
		const cv::Point p1(x - (object_width / 2), y - (object_height / 2));
		const cv::Point p2(x + (object_width / 2), y + (object_height / 2));
		rectangle(img_, p1, p2, color, thickness, cv::LINE_8);
	} else if (object.type == "circle") {
		cv::RotatedRect rRect(cv::Point2f(x, y),
				cv::Size2f(object_width, object_height), 0);
		cv::ellipse (img_, rRect, color, thickness);
	}

	const cv::Point text_position(x - (object_width / 2), y);
	constexpr double font_scale = 0.25;
	constexpr int font_weight = 1;
	putText(img_, object.obj_name, text_position, cv::FONT_HERSHEY_SIMPLEX,
			font_scale, color, font_weight);
}
void Render::clearObject(const MovableObject &object, int msec)
{
	drawMovableObject(object, msec, cv::Scalar(0, 0, 0));
}
bool Render::initWriter(const string &filename)
{
	const int codec = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
	constexpr double fps = 25.0;
	constexpr bool is_color = true;
	return writer_.open(filename, codec, fps, img_.size(), is_color);
}
void Render::appendFrameToWriter()
{
	writer_.write(img_);
}

cv::Point ToCvPoint(const Point &point)
{
	return { point[0], point[1] };
}

void Render::drawStaticObjects(const StaticObjects &static_objects) {
	for (const auto &line : static_objects.lines) {
		if (line.type == "solid_line") {
			drawLine(ToCvPoint(line.start_pos), ToCvPoint(line.end_pos),
					line.thickness);
		} else if (line.type == "dashed_line") {
			drawDashedLine(line);
		}
	}

	for (const auto &circle : static_objects.circles) {
		cv::circle(img_, ToCvPoint(circle.pos), circle.radius,
				cv::Scalar{ 255, 255, 255 }, circle.thickness);
	}
}

void Render::drawDashedLine(const Line &line)
{
	const int period = line.period;
	const int length = static_cast<int>(period * line.duty_cycle);

	cv::LineIterator it(
			img_, ToCvPoint(line.start_pos), ToCvPoint(line.end_pos), 8);
	cv::Point begin_pos = ToCvPoint(line.start_pos);
	for (int i = 0, j = 0; i < it.count; i++, it++) {
		if (i % period == 0) {
			begin_pos = it.pos();
			j = i;
		} else if (i == (j + length)) {
			cv::line(img_, begin_pos, it.pos(), cv::Scalar(255, 255, 255),
					line.thickness, cv::FILLED);
		}
	}
}