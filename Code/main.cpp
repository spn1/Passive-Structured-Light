/*
 * main.cpp
 *
 *	The main source file containing functions to control the flow of the program and the main functionality of the program - the
 *	tracking of light spots.
 *
 *  Created on: 5 Feb 2015
 *      Author: Spencer Newton
 *
 *  Algorithms and Code based on tutorials by Kyle Hounslow
 */

// Includes
#include "opencv2/video/tracking.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Globals.h"
#include "MorphOps.h"
#include "Geometry.h"
#include <stdio.h>
#include <vector>
using std::vector;

using namespace cv;
using namespace std;

// ================================= Variables ================================= //
// Image Names / List
const int IMAGE_WIDTH = 640;
const int IMAGE_HEIGHT = 480;
const int imageIndexMax = 6;
const int imageSetIndexMax = 7;
char* imageNames[imageSetIndexMax][imageIndexMax];
int imageIndex = 0;
int imageSetIndex = 0;

// Window Names
char mainWindowName[] = "Original Image";
char hsvWindowName[] = "HSV Morph";
char thresholdWindowName[] = "Thresholded Feed";
char trackbarWindowName[] = "Trackbars";
char differenceWindowName[] = "Frame Difference";
char differenceThresholdWindowName[] = "Difference Threshold";

// Parameter to store video feed
VideoCapture videoCapture;
const int VIDEO_WIDTH = 640;
const int VIDEO_HEIGHT = 480;

// Matrices to store frames
Mat frame, nextFrame; // Raw frames from camera / image; nextFrame for motion tracking comparison
Mat frameGray, nextFrameGray; // Gray frames for motion tracking
Mat hsvFrame; // Matrix to store HSV colour conversion
Mat thresholdFrame; // Matrix to store thresholded HSV image
Mat differenceFrame; // Matrix to store pixel differences between two frames
Mat differenceThresholdFrame; // Matrix to store thresholded difference image

// Motion Thresholding Parameters
int thresholdSensitivity = 40;
int thresholdSensitivityMax = 255;

// HSV Thresholding Parameters
int hMin = 0, sMin = 0, vMin = 0;
int hMax = 179, sMax = 255, vMax = 255;

// Morphological Operation Parameters
int erodeSize = 3, erodeMax = 9;
int dilateSize = 3, dilateMax = 9;
int blurStrength = 0, blurMax = 20;

// Tracking Parameters
int numberOfObjects;
int maxNumberOfObjects = 50;
int objectAreaMin = 10 * 10;
int objectAreaMax = (100 * 100);

// Control Parameters
bool blurFrame = true, erodeFrame = true, dilateFrame = true, trackFrame = true;
bool videoTrack = false, imageTrack = true;
bool colourTrack = false, differenceTrack = false;
bool printCoordinates = false, showHSV = true;
int mouseX, mouseY;

int input = 0;


// ================================= End Variables ================================= //



// ================================= Function Declarations ================================= //
void runTrackingAlgorithm();
void trackByColour();
void trackByDifference();
void setUpColourWindows();
void setUpMainWindow();
void setUpMotionWindows();
void setOdd( int, void *);
void trackThresholdPixels( Mat );
static void onMouse( int, int, int, int, void* );
void printHSV();

// ================================= End Function Declarations ================================= //

/*
 * Converts and integer into a string
 */
string intToString(int number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}

/*
 * Initializes the vector of image filenames.
 */
void setUpImageNames()
{
	imageNames[0][0] = "Images/new/d_ss_0.jpg";
	imageNames[0][1] = "Images/new/d_ss_1.jpg";
	imageNames[0][2] = "Images/new/d_ss_1.jpg";
	imageNames[0][3] = "Images/new/d_ss_1.jpg";
	imageNames[0][4] = "Images/new/d_ss_1.jpg";
	imageNames[0][5] = "Images/new/d_ss_1.jpg";

	imageNames[1][0] = "Images/new/l_ss_0.jpg";
	imageNames[1][1] = "Images/new/l_ss_1.jpg";
	imageNames[1][2] = "Images/new/l_ss_2.jpg";
	imageNames[1][3] = "Images/new/l_ss_3.jpg";
	imageNames[1][4] = "Images/new/l_ss_4.jpg";
	imageNames[1][5] = "Images/new/l_ss_5.jpg";

	imageNames[2][0] = "Images/new/l_9s_0.jpg";
	imageNames[2][1] = "Images/new/l_9s_1.jpg";
	imageNames[2][2] = "Images/new/l_9s_2.jpg";
	imageNames[2][3] = "Images/new/l_9s_3.jpg";
	imageNames[2][4] = "Images/new/l_9s_4.jpg";
	imageNames[2][5] = "Images/new/l_9s_5.jpg";

	imageNames[3][0] = "Images/new/lc_9s_0.jpg";
	imageNames[3][1] = "Images/new/lc_9s_1.jpg";
	imageNames[3][2] = "Images/new/lc_9s_2.jpg";
	imageNames[3][3] = "Images/new/lc_9s_3.jpg";
	imageNames[3][4] = "Images/new/lc_9s_4.jpg";
	imageNames[3][5] = "Images/new/lc_9s_4.jpg";

	imageNames[4][0] = "Images/new/dcube_9s_0.jpg";
	imageNames[4][1] = "Images/new/dcube_9s_1.jpg";
	imageNames[4][2] = "Images/new/dcube_9s_2.jpg";
	imageNames[4][3] = "Images/new/dcube_9s_3.jpg";
	imageNames[4][4] = "Images/new/dcube_9s_4.jpg";
	imageNames[4][5] = "Images/new/dcube_9s_5.jpg";

	imageNames[5][0] = "Images/new/l_36s_0.jpg";
	imageNames[5][1] = "Images/new/l_36s_1.jpg";
	imageNames[5][2] = "Images/new/l_36s_2.jpg";
	imageNames[5][3] = "Images/new/l_36s_3.jpg";
	imageNames[5][4] = "Images/new/l_36s_4.jpg";
	imageNames[5][5] = "Images/new/l_36s_4.jpg";

	imageNames[6][0] = "Images/new/lp_ss_0.jpg";
	imageNames[6][1] = "Images/new/lp_ss_1.jpg";
	imageNames[6][2] = "Images/new/lp_ss_2.jpg";
	imageNames[6][3] = "Images/new/lp_ss_3.jpg";
	imageNames[6][4] = "Images/new/lp_ss_4.jpg";
	imageNames[6][5] = "Images/new/lp_ss_5.jpg";

}

/*
 * The main method of the program.
 */
int main( int argc, char** argv)
{
	char choice;
	setUpImageNames();

	cout << "Track or Calculate: t or c\n";
	cin >> choice;

	switch (choice)
	{
	case 't':
		runTrackingAlgorithm();
		break;

	case 'c':
		runGeometryCalculations();
		break;

	default:
		break;
	}
}

/*
 * This function is the main loop of the program. It will continually load, modify, and display the images that are being used to try and find light spots.
 *
 *
 * WAITKEY KEYCODES:
 *  'a' = 97
 *  'b' = 98
 *  'c' = 99
 *  ...
 *  'k' = 107
 *  'l' = 108
 *  'm' = 109
 *  ...
 *  'z' = 122
 *
 */
void runTrackingAlgorithm()
{
	// Set up 'videoCapture' to take video from webcam
	if ( videoTrack )
	{
		videoCapture.open( 0 );
		videoCapture.set( CV_CAP_PROP_FRAME_WIDTH, VIDEO_WIDTH );
		videoCapture.set( CV_CAP_PROP_FRAME_HEIGHT, VIDEO_HEIGHT );
	}


	// Create main windows
	setUpMainWindow();
	imshow( mainWindowName, imread( "intro.png", CV_LOAD_IMAGE_COLOR ) );

	switch( waitKey( 0 ) )
	{
	case 99: // If letter 'c' is pressed
		colourTrack = true;
		setUpColourWindows();
		break;
	default:
	case 109: // If letter 'm' is pressed
		differenceTrack = true;
		setUpMotionWindows();
		break;
	}

	input = waitKey(10);

	// While escape key (code = 27) not pressed, wait 40ms each
	while ( input != 27 )
	{
		if ( colourTrack ) trackByColour();
		if ( differenceTrack ) trackByDifference();

		input = waitKey(10);

		// If you press p, print coordinates of spots
		if ( input == 112 ) printCoordinates = true;

		// Press w / s to cycle up / down through sets of images
		if ( input == 119 ) {
			imageSetIndex == imageSetIndexMax - 1 ? imageSetIndex = 0 : imageSetIndex++;
			imageIndex = 1;
		}
		if ( input == 115 )
		{
			imageSetIndex == 0 ? imageSetIndex = imageSetIndexMax - 1 : imageSetIndex--;
			imageIndex = 1;
		}

		// Press a / d to cycle left / right through set list of images
		if ( input == 97 )
			imageIndex == 0 ? imageIndex = imageIndexMax - 1 : imageIndex--;
		if ( input == 100 )
			imageIndex == imageIndexMax - 1 ? imageIndex = 0 : imageIndex++;

		// If r is pressed, toggle erode operations
		if ( input == 114 )
			erodeFrame = !erodeFrame;

		// If t is pressed, toggle dilate operations
		if ( input == 116 )
			dilateFrame = !dilateFrame;

		// If b is pressed, toggle blurring
		if ( input == 98 )
			blurFrame = !blurFrame;

		// If m is pressed, toggle hsv indication
		if ( input == 109 )
			showHSV = !showHSV;
	}
}

/*
 * This function will track light spots by looking for pixel differences between two images.
 */
void trackByDifference()
{
	// If focus is to track by images, load image with error catching.
	if ( imageTrack )
	{
		frame = imread( imageNames[imageSetIndex][0], CV_LOAD_IMAGE_COLOR );
		if (frame.cols == 0) {
		     cout << "Error reading file " << endl;
		}
		flip( frame, frame, 0 ); // Silly me took pictures upside down
	}
	else
	{
		videoCapture.read( frame );
	}

	// Read the next frame so we can compare the difference
	// This assumes that enough time has passed so that the next frame has already been captured by the camera
	if ( videoTrack )
	{
		videoCapture.read( nextFrame );
	}
	else if ( imageTrack )
	{
		nextFrame = imread( imageNames[imageSetIndex][imageIndex], CV_LOAD_IMAGE_COLOR );

		flip( nextFrame, nextFrame, 0 ); // Silly me took pictures upside down
	}

	// Resize windows to fit on laptop screen
	resize( frame, frame, Size(), 0.2f, 0.2f );
	resize( nextFrame, nextFrame, frame.size() );


	// Convert 'frame' to gray scale;
	cvtColor( frame, frameGray, CV_RGB2GRAY );
	// Convert the next frame to gray scale;
	cvtColor( nextFrame, nextFrameGray, CV_RGB2GRAY );

	// Get the absolute different between pixel values in the two images
	absdiff( frameGray, nextFrameGray, differenceFrame );

	// Threshold the difference image out to get clearer motion
	threshold( differenceFrame, differenceThresholdFrame, thresholdSensitivity, 255, THRESH_BINARY );

	// Blur image to get rid of noise
	if ( blurFrame && blurStrength != 0 )
		blurImage( &differenceThresholdFrame, &differenceThresholdFrame, 1, blurStrength );

	// Erode and Dilate 'thresholdFrame' to get rid of noise
	if ( erodeFrame && erodeSize != 0 )
			erodeImage( &differenceThresholdFrame, &differenceThresholdFrame, 0, erodeSize );

	if ( dilateFrame && dilateSize != 0 )
		dilateImage( &differenceThresholdFrame, &differenceThresholdFrame, 0, dilateSize );

	// Track object in real camera feed based on threshold pixels
	if ( trackFrame )
		trackThresholdPixels( differenceThresholdFrame );

	// Show result
	imshow( mainWindowName, nextFrame );
	imshow( differenceWindowName, differenceFrame );
	imshow( differenceThresholdWindowName, differenceThresholdFrame );
}

/*
 *	This function will track light spots by finding areas of a particular colour in an image.
 */
void trackByColour()
{
	// Read frame from 'videoCapture' and put into 'frame'
	if ( videoTrack )
	{
		videoCapture.read( frame );
	}
	else if ( imageTrack )
	{
		frame = imread( imageNames[imageSetIndex][imageIndex], CV_LOAD_IMAGE_COLOR );
		if (frame.cols == 0) {
			cout << "Error reading file " << endl;
		}

		flip( frame, frame, 0 ); // Silly me took pictures upside down
	}

	// Pictures too big for my laptop screen...
	resize( frame, frame, Size(), 0.2f, 0.2f );

	// Convert 'frame' to HSV colour scheme and put into new Matrix 'hsvFrame'
	cvtColor( frame, hsvFrame, CV_BGR2HSV );

	// Find pixels from a specific colour range from 'hsvFrame', set those to one and all others to
	// zero, and put result in new Matrix 'thresholdFrame'
	inRange( hsvFrame, Scalar(hMin, sMin, vMin), Scalar(hMax, sMax, vMax), thresholdFrame );

	// Blur image to get rid of noise
	if ( blurFrame && blurStrength != 0 )
		blurImage( &thresholdFrame, &thresholdFrame, 1, blurStrength );

	// Erode and Dilate 'thresholdFrame' to get rid of noise
	if ( erodeFrame && erodeSize != 0 )
			erodeImage( &thresholdFrame, &thresholdFrame, 0, erodeSize );

	if ( dilateFrame && dilateSize != 0 )
		dilateImage( &thresholdFrame, &thresholdFrame, 0, dilateSize );

	// Track object in real camera feed based on threshold pixels
	if ( trackFrame )
		trackThresholdPixels( thresholdFrame );

	// Show result
	imshow( mainWindowName, frame );
	imshow( hsvWindowName, hsvFrame );
	imshow( thresholdWindowName, thresholdFrame );

	if ( showHSV ) printHSV();
}

/*
 * This function sets up and initializes all the windows and trackbars used to alter parameters or show images in the program, specifically
 * those used in the colour finding algorithm.
 */
void setUpColourWindows()
{
	// Create Colour Windows
	namedWindow( hsvWindowName );
	namedWindow( thresholdWindowName );
	namedWindow( trackbarWindowName );

	// Add Trackbars
	createTrackbar( "Hue Min", trackbarWindowName, &hMin, hMax );
	createTrackbar( "Hue Max", trackbarWindowName, &hMax, hMax );
	createTrackbar( "Sat Min", trackbarWindowName, &sMin, sMax );
	createTrackbar( "Sat Max", trackbarWindowName, &sMax, sMax );
	createTrackbar( "Val Min", trackbarWindowName, &vMin, vMax );
	createTrackbar( "Val Max", trackbarWindowName, &vMax, vMax );

	createTrackbar( "Erode Size", trackbarWindowName, &erodeSize, erodeMax, setOdd );
	createTrackbar( "Dilate Size", trackbarWindowName, &dilateSize, dilateMax, setOdd );
	createTrackbar( "Blur Strength", trackbarWindowName, &blurStrength, blurMax, setOdd );

	setMouseCallback( mainWindowName, onMouse, 0 );
}

/*
 * This function sets up and initializes the main window that displays the unaltered image and the trackbars used to alter tracking parameters.
 */
void setUpMainWindow()
{
	// Create Windows
	namedWindow( mainWindowName );

	// Add tracking trackbars
	createTrackbar( "Max Number of Objects", mainWindowName, &maxNumberOfObjects, 200 );
	createTrackbar( "Min Object Area", mainWindowName, &objectAreaMin, objectAreaMax );
	createTrackbar( "Max Object Area", mainWindowName, &objectAreaMax, objectAreaMax );
}

/*
 * This function sets up and initializes all the windows and trackbars used to alter parameters or show images in the program, specifically
 * those used in the difference finding algorithm.
 */
void setUpMotionWindows()
{
	// Create Colour Windows
	namedWindow( trackbarWindowName );
	namedWindow( differenceWindowName );
	namedWindow( differenceThresholdWindowName );

	// Add Trackbars
	createTrackbar( "Erode Size", trackbarWindowName, &erodeSize, erodeMax, setOdd );
	createTrackbar( "Dilate Size", trackbarWindowName, &dilateSize, dilateMax, setOdd );
	createTrackbar( "Blur Strength", trackbarWindowName, &blurStrength, blurMax, setOdd );
	createTrackbar( "Sensitivity", trackbarWindowName, &thresholdSensitivity, thresholdSensitivityMax );
}

/*
 * Function to track the thresholded pixels ( all set to 1 ) in a given Matrix
 */
void trackThresholdPixels( Mat threshFrame )
{
	int x, y;
	int objectsWithinSize;
	Mat temp;
	vector< vector<Point> > contours;
	vector<Vec4i> contourHierarchy;

	threshFrame.copyTo( temp );

	// Get contours of pixels set to one in thresholded image.
	findContours( temp, contours, contourHierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

	numberOfObjects = contourHierarchy.size();
	objectsWithinSize = 0;

	if ( printCoordinates ) cout << "---- Spot Coordinates ----" << endl;

	// Assuming that the only objects left in 'thresholdFrame' are what we want, track them all.
	if ( numberOfObjects > 0 && numberOfObjects < maxNumberOfObjects )
	{
		for ( int index = 0; index >= 0; index = contourHierarchy[index][0] )
		{
			// Get area from contour object
			Moments moment = moments((cv::Mat)contours[index]);
			double area = moment.m00;

			if ( area > objectAreaMin )
			{
				// Circle spot on screen
				x = moment.m10/area;
				y = moment.m01/area;

				if ( colourTrack ) circle( frame, Point(x,y), 10, Scalar(0,255,0), 2);
				if ( differenceTrack ) circle( nextFrame, Point(x,y), 10, Scalar(0,255,0), 2);

				objectsWithinSize++;

				if ( printCoordinates ) cout << "\t" << intToString( x ) << ", " << intToString( y ) << "\n";
			}
		}

		// Display how many objects are being tracked
		if ( colourTrack ) putText( frame, "Spots Found: " + intToString( objectsWithinSize ), Point(10,20), 1, 1, Scalar(0,255,0), 2);
		if ( differenceTrack ) putText( nextFrame, "Spots Found: " + intToString( objectsWithinSize ), Point(10,20), 1, 1, Scalar(0,255,0), 2);

	}

	if ( printCoordinates ) printCoordinates = !printCoordinates;
}

/*
 * Function to ensure the erosion and dilation size is always odd
 */
void setOdd( int val, void *)
{
	if ( val != 0 )
	{
		if ( !(erodeSize % 2 == 1) )
		{
			erodeSize = ( erodeSize > 1 ? erodeSize - 1 : 1 );
		}

		if ( !(dilateSize % 2 == 1) )
		{
			dilateSize = ( dilateSize > 1 ? dilateSize - 1 : 1 );
		}

		if ( !(blurStrength % 2 == 1) )
		{
			blurStrength = ( blurStrength > 1 ? blurStrength - 1 : 1 );
		}
	}
}

/*
 *  Function that is called on mouse click that will print out the hsv value of the pixel at the mouse position.
 */
void printHSV()
{
	Mat image;

	if ( colourTrack ) image = frame.clone();
	if ( differenceTrack ) image = nextFrame.clone();

	// Get RGB Values
	Vec3b rgb = image.at<Vec3b>(mouseY, mouseX);
	int B=rgb.val[0];
	int G=rgb.val[1];
	int R=rgb.val[2];

	Mat HSV;
	Mat RGB = image( Rect(mouseX, mouseY, 1, 1) );
	cvtColor( RGB, HSV, CV_BGR2HSV );

	// Get HSV Values
	Vec3b hsv = HSV.at<Vec3b>(0,0);
	int H = hsv.val[0];
	int S = hsv.val[1];
	int V = hsv.val[2];

	// Put text on screen (on top of image)
	char name[30];
	sprintf(name,"B=%d",B);
	putText( image, name, Point(150,40), FONT_HERSHEY_SIMPLEX, .7, Scalar(0,255,0) );

	sprintf(name,"G=%d",G);
	putText( image,name, Point(150,80) , FONT_HERSHEY_SIMPLEX, .7, Scalar(0,255,0) );

	sprintf(name,"R=%d",R);
	putText( image,name, Point(150,120) , FONT_HERSHEY_SIMPLEX, .7, Scalar(0,255,0) );

	sprintf(name,"H=%d",H);
	putText( image,name, Point(25,40) , FONT_HERSHEY_SIMPLEX, .7, Scalar(0,255,0) );

	sprintf(name,"S=%d",S);
	putText( image,name, Point(25,80) , FONT_HERSHEY_SIMPLEX, .7, Scalar(0,255,0) );

	sprintf(name,"V=%d",V);
	putText( image,name, Point(25,120) , FONT_HERSHEY_SIMPLEX, .7, Scalar(0,255,0) );

	imshow( mainWindowName, image );
}

/*
 *  Function that defines what happens upon a mouse event.
 */
static void onMouse( int event, int x, int y, int f, void* )
{
	Mat image;
	if ( event == CV_EVENT_LBUTTONDOWN )
	{
		mouseX = x;
		mouseY = y;
	}
}



