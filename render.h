#pragma once

#include "movable_object.h"
#include "static_objects.h"
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

class Render
{
public:
	explicit Render(int frame_height, int image_height_m, int frame_width,
			int image_width_m):
			img_(frame_height, frame_width, CV_8UC3, cv::Scalar(0, 0, 0)),
			hor_pixel_per_meter_(frame_width / image_width_m),
			ver_pixel_per_meter_(frame_height / image_height_m) {}
	void drawStaticObjects(const StaticObjects &static_objects);
	void drawLine(cv::Point start, cv::Point end, int thickness = 1);
	void drawDashedLine(const Line &line);
	void drawMovableObject(const MovableObject &object, int msec,
			const cv::Scalar &color = cv::Scalar(255, 255, 255));
	void clearObject(const MovableObject &object, int msec);
	bool initWriter(const string &filename);
	void appendFrameToWriter();
private:
	cv::Mat img_;
	int hor_pixel_per_meter_{ 1 };
	int ver_pixel_per_meter_{ 1 };
	cv::VideoWriter writer_;
};