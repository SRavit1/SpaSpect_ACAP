// Copyright 2020 The Konect SpaSpect Authors. All Rights Reserved.
// This file is part of Konect SpaSpect technology.
//  ___            ___              _   
// / __|_ __  __ _/ __|_ __  ___ __| |_ 
// \__ \ '_ \/ _` \__ \ '_ \/ -_) _|  _|
// |___/ .__/\__,_|___/ .__/\___\__|\__|
//     |_|            |_|               

#include <iostream>

#include "main.h"

using namespace std;
using namespace cv;

/*
Description
	Runs tests related to obtaining frames from a video
Parameters
	int verbose = 0 - whether to produce verbose output
*/
int testReadVideo(int verbose = 0) {
	Mat image;

	int counter = 0;

	for (;;) {
		VideoCapture cap("/home/santript/ImportantProjects/Files/TimesSquare2.mp4");
		cap.read(image);

		if(!image.data) {
			return -1;
			if (verbose)
				cout << "NO IMAGE DATA" << endl;
		} else {
			counter++;
			if (verbose)
				cout << "FRAME #" << counter << endl;
		}
	}

	//TODO: Add in asserts for the test
}


/*
Description
	Runs tests related to obtaining the transformation matrix
Parameters
	int verbose = 0 - whether to produce verbose output
*/
int testTransformation(int verbose = 0) {
	Point2f source_calib[NUM_CALIB_ROWS];
	Point2f dest_calib[NUM_CALIB_ROWS];

	readFromCalib("./sampleCalib.config", source_calib, dest_calib);

	Mat transformation_mat(2, 2 ,CV_32FC1);
	obtainPerspectiveTransform(source_calib, dest_calib, transformation_mat);

	if (verbose)
		cout << "Traformation Mat Value: " << transformation_mat << endl;

	//TODO: Add in asserts for the test
}

int main() {
	//testReadVideo();
	testTransformation();
	cout << "All tests passed" << endl;
}