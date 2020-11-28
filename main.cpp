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

bool readFromCalib(string fileName, vector<vector<int>>& source_calib, vector<vector<int>>& dest_calib){
	string currLine;

	ifstream MyReadFile(fileName);

	while(true){
		getline(MyReadFile, currLine);
		if (currLine == "") return false;
		if (currLine.at(0) != '#')
			break;
	}

	vector<vector<int>> source_calib_tmp = {{0,0}, {0,0}, {0,0}, {0,0}};
	vector<vector<int>> dest_calib_tmp = {{0,0}, {0,0}, {0,0}, {0,0}};

	getline(MyReadFile, currLine);
	if (currLine == "") return false;

	vector<int> nums;
	int a = 0, b = 0;
	stringToVector(currLine, nums);
	for (int num : nums) {
		source_calib_tmp[a][b] = num;

		if (b == 1) { a++ ; b = 0; }
		else { b++; }
	}

	getline(MyReadFile, currLine);
	if (currLine == "") return false;

	a = 0; b = 0;
	stringToVector(currLine, nums);
	for (int num : nums) {
		dest_calib_tmp[a][b] = num;

		if (b == 1) { a++ ; b = 0; }
		else { b++; }
	}

	MyReadFile.close();

	source_calib = source_calib_tmp;
	dest_calib = dest_calib_tmp;

	return true;
}

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
		Point2f source_calib_curr = *source_calib;
		source_calib_curr.x = nums[2*i];
		source_calib_curr.y = nums[2*i+1];
		source_calib++;
	}

	for (int i = 0; i < LINE_LENGTH/NUM_CALIB_COLS; i++) {
		Point2f source_calib_curr = *source_calib;

		cout << "STORING VALUES IN SOURCE: " << source_calib_curr.x 
		<< " " << source_calib_curr.y << endl;


		source_calib++;
	}

	//Reading in the first line: dest_calib values
	getline(MyReadFile, currLine);
	if (currLine == "") return false;

	stringToVector(currLine, nums);
	assert (nums.size() == LINE_LENGTH);

	for (int i = 0; i < LINE_LENGTH/NUM_CALIB_COLS; i++) {
		Point2f dest_calib_curr = *dest_calib;
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
	vector<vector<int>> source_calib: (4,2) array containing source calibration
	vector<vector<int>> dest_calib: (4,2) array containing destination calibration
	Mat& transformation_mat: reference to transformation matrix which stores
		the resulting transformation matrix
*/
void obtainPerspectiveTransform(vector<vector<int>> source_calib, 
	vector<vector<int>> dest_calib, Mat& transformation_mat) {
	//transformation_mat = getPerspectiveTransform(source_calib, dest_calib);
	vector<vector<int>> s_c;
	vector<vector<int>> d_c;

	for (int i = 0; i < 1; i++) {
		vector<int> s_c_append = {1,1,1,1};
		vector<int> d_c_append = {1,1,1,1};

		s_c.push_back(s_c_append);
		d_c.push_back(d_c_append);
	}
	getPerspectiveTransform(s_c, d_c);
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

int main() {
	//vector<vector<int>> source_calib;
	//vector<vector<int>> dest_calib;

	Point2f source_calib_new[NUM_CALIB_ROWS];
	Point2f dest_calib_new[NUM_CALIB_ROWS];

	//readFromCalib("./sampleCalib.config", source_calib, dest_calib);
	readFromCalib("./sampleCalib.config", source_calib_new, dest_calib_new);
	
	for (int i = 0; i < NUM_CALIB_ROWS; i++) {
		Point2f curr = source_calib_new[i];
		cout << curr.x << " " << curr.y << endl;
	}

	cout << endl;

	for (int i = 0; i < NUM_CALIB_ROWS; i++) {
		Point2f curr = dest_calib_new[i];
		cout << curr.x << " " << curr.y << endl;
	}

	//Mat transformation_mat(2, 2 ,CV_32FC1);
	//obtainPerspectiveTransform(source_calib, dest_calib, transformation_mat);

	/*
	Mat image;

	int counter = 0;

	for (;;) {
		VideoCapture cap("/home/ravit/Videos/RealtimeAction.mp4");
		cap.read(image);

		if(!image.data) {
			return -1;
			cout << "NO IMAGE DATA" << endl;
		} else {
			counter++;
			cout << "FRAME #" << counter << endl;
		}
	}
	*/

	return 0;
}
