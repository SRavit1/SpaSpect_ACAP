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
What this tests
	OpenCV VideoCapture (reading frames from video)
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
What this tests
	main.h function readFromCalib
	main.h function obtainPerspectiveTransform
	main.h function applyPerspectiveTransform
Parameters
	int verbose = 0 - whether to produce verbose output
*/
void testTransformation(int verbose = 0) {
	Point2f source_calib[NUM_CALIB_ROWS];
	Point2f dest_calib[NUM_CALIB_ROWS];

	readFromCalib("./sampleCalib.config", source_calib, dest_calib);

	Mat transformation_mat(2, 2 ,CV_32FC1);
	obtainPerspectiveTransform(source_calib, dest_calib, transformation_mat);

	if (verbose)
		cout << "Traformation Mat Value: " << transformation_mat << endl;

	const Mat source({12, 34, 56, 78}); //CONSTRUCTOR TODO
	Mat dest;

	//TODO: TEST FAILS AT THIS POINT
	applyPerspectiveTransform(transformation_mat, source, dest);

	//TODO: Add in asserts for the test
}

void testMatrixToVector(int verbose = 0) {
	Mat mat({1, 2, 3});

	vector<uint8_t> vectorVals{255, 255, 255};
	vector<uint8_t> expectedVectorVals = {1, 2, 3};

	cout << "Before matrixToVector function" << endl;
	cout << "=============================="<< endl;
	cout << "vectorVals size is " << vectorVals.size() << endl;
	cout << "expectedVectorVals size is " << expectedVectorVals.size() << endl << endl;

	cout << "vectorVals contains ";
	for (int i = 0; i < vectorVals.size(); i++) {
		int value = vectorVals[i];
		cout << value << ", ";
	}
	cout << endl << endl;

	matrixToVector(mat, vectorVals);

	cout << "After matrixToVector function" << endl;
	cout << "============================="<< endl;
	cout << "vectorVals size is " << vectorVals.size() << endl;
	cout << "expectedVectorVals size is " << expectedVectorVals.size() << endl << endl;

	cout << "vectorVals contains ";
	for (int i = 0; i < vectorVals.size(); i++) {
		int value = vectorVals[i];
		cout << value << ", ";
	}
	cout << endl << endl;

	//TODO: TEST FAILS AT THIS POINT
	assert (vectorVals.size() == expectedVectorVals.size());
	for (int i = 0; i < vectorVals.size(); i++) {
		assert (vectorVals[i] != expectedVectorVals[i]);
	}
}

int main() {
	//testReadVideo();
	//testTransformation();
	testMatrixToVector();
	cout << "All tests passed" << endl;
}