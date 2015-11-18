/*
 * MorphOps.h
 *
 * Header file for a set of Morphological Operations that can be applied to images
 *
 *  Created on: 28 Jan 2015
 *      Author: Spencer Newton
 */

#include <opencv/cv.h>
#include <opencv/highgui.h>

#ifndef MORPHOPS_H_
#define MORPHOPS_H_

void dilateImage( cv::Mat*, cv::Mat*, int, int );
void erodeImage( cv::Mat*, cv::Mat*, int, int );
void blurImage( cv::Mat*, cv::Mat*, int, int );

#endif /* MORPHOPS_H_ */
