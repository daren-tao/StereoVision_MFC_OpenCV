#ifndef __T_3D_RECONSTRUCTION_H__
#define __T_3D_RECONSTRUCTION_H__

#include "elas.h"
#include "image.h"

#include "opencv2/opencv.hpp"


static void process(cv::Mat& frame1, cv::Mat& frame2, uchar* disparity1 )
{
	if(frame1.channels() > 1)
		cv::cvtColor(frame1, frame1, cv::COLOR_RGB2GRAY);
	if(frame2.channels() > 1)
		cv::cvtColor(frame2, frame2, cv::COLOR_RGB2GRAY);

	// get image width and height
	int32_t width  = frame1.cols;
	int32_t height = frame1.rows;

	// allocate memory for disparity images
	const int32_t dims[3] = {width, height, width}; // bytes per line = width
	float* D1_data = new float [width * height];
	float* D2_data = new float [width * height];

	// process
	Elas::parameters param;
	param.postprocess_only_left = true;
	Elas elas(param);
	elas.process(frame1.data, frame2.data, D1_data, D2_data, dims);

	// find maximum disparity for scaling output disparity images to [0..255]
	float disp_max = 0;
	for (int32_t i = 0; i < width * height; i++) {
		if (D1_data[i] > disp_max) disp_max = D1_data[i];
//		if (D2_data[i] > disp_max) disp_max = D2_data[i];
	}
	// copy float to uchar
	for (int32_t i = 0; i < width * height; i++) {
		disparity1[i] = (uint8_t)std::max(255.0 * D1_data[i] / disp_max, 0.0);
//		D2->data[i] = (uint8_t)std::max(255.0 * D2_data[i] / disp_max, 0.0);
	}
	// free memory
	delete D1_data;
	delete D2_data;
}

#endif