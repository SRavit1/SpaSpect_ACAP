// Copyright 2020 The Konect SpaSpect Authors. All Rights Reserved.
// This file is part of Konect SpaSpect technology.
//  ___            ___              _   
// / __|_ __  __ _/ __|_ __  ___ __| |_ 
// \__ \ '_ \/ _` \__ \ '_ \/ -_) _|  _|
// |___/ .__/\__,_|___/ .__/\___\__|\__|
//     |_|            |_|               


#include <fstream>
#include <string>
#include <iostream>
#include <cassert>
#include <cctype>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp> // Perspective Transform

using namespace std;
using namespace cv;

const int NUM_CALIB_ROWS = 4;
const int NUM_CALIB_COLS = 2;
const int LINE_LENGTH = NUM_CALIB_ROWS * NUM_CALIB_COLS;

/*
Description
	Transforming string to vector to read from calib file
Params
	string input - the input string from calib file that will be converted to vector
	vector<int>& destination - the value of result is stored in this variable to prevent return (reference)
*/
void stringToVector(string input, vector<int>& destination) {
	vector<int> result;	

	int currVal = 0;
	for (int i = 0; i < input.length(); i++) {
		char curr = input.at(i);
		if (curr == ' ') {
			result.push_back(currVal);
			currVal = 0;
		} else {
			currVal *= 10;
			assert(isdigit(curr));
			currVal += (curr - '0');
		}
	}

	result.push_back(currVal);

	destination = result;
}

/*
Description
	Reads from the calibration file and stores the values in source_calib and dest_calib
Params
	string filename - the relative path of the file to read
	Point source_calib[] - the array of Point2f objects representing source calib information
	Point dest_calib[] - the array of Point2f objects representing desination calib information
*/
bool readFromCalib(string fileName, Point2f source_calib[], Point2f dest_calib[]){
	string currLine;

	ifstream MyReadFile(fileName);

	while(true){
		getline(MyReadFile, currLine);
		if (currLine == "") return false;
		if (currLine.at(0) != '#')
			break;
	}

	//Reading in the first line: source_calib values
	getline(MyReadFile, currLine);
	if (currLine == "") return false;

	vector<int> nums;
	stringToVector(currLine, nums);
	assert (nums.size() == LINE_LENGTH);

	for (int i = 0; i < LINE_LENGTH/NUM_CALIB_COLS; i++) {
		Point2f &source_calib_curr = *source_calib;
		source_calib_curr.x = nums[2*i];
		source_calib_curr.y = nums[2*i+1];
		source_calib++;
	}

	//Reading in the first line: dest_calib values
	getline(MyReadFile, currLine);
	if (currLine == "") return false;

	stringToVector(currLine, nums);
	assert (nums.size() == LINE_LENGTH);

	for (int i = 0; i < LINE_LENGTH/NUM_CALIB_COLS; i++) {
		Point2f &dest_calib_curr = *dest_calib;
		dest_calib_curr.x = nums[2*i];
		dest_calib_curr.y = nums[2*i+1];

		dest_calib++;
	}

	MyReadFile.close();

	return true;
}

/*
Description
	Converts calibration information from source and destination coordinates
		into a perspective transformation matrix
Params
	Point2f* source_calib: (4,2) array containing source calibration
	Point2f* dest_calib: (4,2) array containing destination calibration
	Mat& transformation_mat: reference to transformation matrix which stores
		the resulting transformation matrix
*/
void obtainPerspectiveTransform(Point2f* source_calib, 
	Point2f* dest_calib, Mat& transformation_mat) {
	transformation_mat = getPerspectiveTransform(source_calib, dest_calib);
}

/*
Description
	Applies specified transformation to source and stores it in dest
Params
	const Mat& transformation_mat: the transformation matrix
	const Mat& source: the source matrix on which to apply the transformation
	Mat& dest: the matrix in which the result will be stored
*/
void applyPerspectiveTransform(const Mat& transformation_mat, const Mat& source, Mat& dest) {
	dest = transformation_mat.dot(source);
}


void matrixToVector(const Mat& mat, vector<uint8_t>& finalVector){
	vector<uint8_t>* array = new vector<uint8_t>(mat.rows*mat.cols*mat.channels());
	if (mat.isContinuous())
   		array.insert(array.end(), mat.data, mat.data+(mat.rows*mat.cols*mat.channels()));
   	else
   		for (int i=0; i<mat.rows; ++i){
   		    uint8_t* row = mat.ptr<uint8_t>(i);
   		    array.insert(array.end(), row, row+(mat.cols*mat.channels()));
   		}

   	finalVector = *array;
}