/*
 * MorphOps.cpp
 *
 *	Source file containing implementations of Morphological operations to be used on images.
 *
 *  Created on: 28 Jan 2015
 *      Author: Spencer Newton
 */

#include "MorphOps.h"

using namespace cv;

// ================================= Variables ================================= //

int erosion_size = 2;
int dilation_size = 2;
int const max_elem = 2;
int const max_kernel_size = 21;

// ================================= End Variables ================================= //

/*
 * This function will 'erode' and image and get rid of any noise. This will eliminate small high-intensity pixels, make dark
 * areas larger and make bright areas smaller
 */
void erodeImage( Mat *src, Mat *dest, int kernelType, int kernelSize = 2 )
{
	int erosion_type;

	if( kernelType == 0 ){ erosion_type = MORPH_RECT; }
	else if( kernelType == 1 ){ erosion_type = MORPH_CROSS; }
	else if( kernelType == 2) { erosion_type = MORPH_ELLIPSE; }

	Mat element = getStructuringElement( erosion_type,
	                                       Size( 2*kernelSize + 1, 2*kernelSize+1 ),
	                                       Point( kernelSize, kernelSize ) );

	// Apply the erosion operation
	erode( *src, *dest, element );
}

/*
 * This function will 'dilate' an image and amplify any bright areas. This will make bright areas larger and dark areas smaller.
 */
void dilateImage( Mat *src, Mat *dest, int kernelType, int kernelSize = 2 )
{
	int dilation_type;
	if( kernelType == 0 ){ dilation_type = MORPH_RECT; }
	else if( kernelType == 1 ){ dilation_type = MORPH_CROSS; }
	else if( kernelType == 2) { dilation_type = MORPH_ELLIPSE; }

	Mat element = getStructuringElement( dilation_type,
	                                       Size( 2*kernelSize + 1, 2*kernelSize+1 ),
	                                       Point( kernelSize, kernelSize ) );
	// Apply the dilation operation
	dilate( *src, *dest, element );
}

/*
 * This function will blur and image by blending the value of each pixel based on the sum of each
 */
void blurImage( Mat *src, Mat *dest, int blurType, int kernelSize = 2 )
{
	/*
	 * 0 = Homogeneous
	 * 1 = Gaussian
	 * 2 = Median
	 */

	switch ( blurType )
	{
	default:
	case 0:
		blur( *src, *dest, Size( kernelSize, kernelSize ), Point(-1,-1) );
		break;
	case 1:
		GaussianBlur( *src, *dest, Size( kernelSize, kernelSize ), 0, 0 );
		break;
	case 2:
		medianBlur( *src, *dest, kernelSize );
		break;
	}
}




