// -*- coding: utf-8 -*-
/******************************************************************************
@author: Wei Huajing
@company: KAMERAWERK
@e-mail: jerryweihuajing@126.com

@title: Source-Calculation on Contrast
******************************************************************************/

#include "..\Header\object_frame.h"

#include "..\Header\operation_vector.h"
#include "..\Header\calculation_contrast.h"
#include "..\Header\calculation_histogram.h"

/*
Calculate ROI matrix as an vector

Args:
	img_gray: input image(gray scale)
	center_ROI: center coordinate of ROI

Returns:
	ROI whose type is vector of 1-D
*/
vector<int> VectorROI(Mat& img_gray, int center_ROI[2]) {

	//size of matrix
	int height = img_gray.rows;
	int width = img_gray.cols;

	//5-Area ROI size
	int height_ROI = int(height / 9);
	int width_ROI = int(width / 9);

	//half size
	int half_height_ROI = int(height / 18);
	int half_width_ROI = int(width / 18);

	//amount of pixel in ROI
	int area_ROI = height_ROI * width_ROI;

	//ROI matrix
	vector<int> ROI_gray;

	//give value to ROI matrix object
	int i;
	int j;
	int i_start = center_ROI[0] - half_height_ROI;
	int j_start = center_ROI[1] - half_width_ROI;

	for (i = 0; i < height_ROI; i++) {

		for (j = 0; j < width_ROI; j++) {

			ROI_gray.push_back(img_gray.ptr<uchar>(i_start + i)[j_start + j]);

		}
	}
	//judge whether it is correct
	//cout << ROI.size() << endl;
	//cout << ROI[0] << endl;
	return ROI_gray;
}

/*
Calculate contrast of ROI vector

Args:
	vector_ROI: ROI whose type is vector of 1-D
	contrast_operator: contrast operator

Returns:
	contrast value of ROI
*/
double ContrastROI(vector<int>& vector_ROI, const string& contrast_operator) {

	cout << "-- Contrast ROI" << endl;

	//maximum and minimum of ROI vector
	int L_maximum = VectorMaximum(vector_ROI);
	int L_minimum = VectorMinimum(vector_ROI);

	//convert to double (+0.5 to avoid overflow)
	double L_max = static_cast<double>(L_maximum) + 0.5;
	double L_min = static_cast<double>(L_minimum) + 0.5;

	//final result
	double contrast=0.0;

	//Michelson (1927)
	if (contrast_operator == "Michelson") {

		contrast = (L_max - L_min) / (L_max + L_min);
	}
	//Whittle (1986)
	if (contrast_operator == "Whittle") {

		contrast = (L_max - L_min) / (L_min);
	}
	//Weber (1840)
	if (contrast_operator == "Weber") {

		contrast = L_max - L_min;
	}
	//W3C (2006)
	if (contrast_operator == "W3C") {

		contrast = (L_max + 0.05) / (L_min + 0.05);
	}
	//Boccignone (1996)
	if (contrast_operator == "Boccignone") {

		//need iteration

		//gray level and their frequency
		vector<int> gray_level = VectorGrayLevel(1);
		vector<double> gray_frequency = VectorGrayLevelFrequency(vector_ROI);

		//calculate avearge gray level
		double L_average = VectorMultiplication(gray_level, gray_frequency);

		//calculate index of gray value who is smaller or bigger than threshold
		vector<int> index_vector_below_average = VectorIndexBelowThreshold(gray_level, L_average);
		vector<int> index_vector_above_average = VectorIndexAboveThreshold(gray_level, L_average);

		//divide gray level
		vector<int> gray_level_below_average = VectorFromIndex(gray_level, index_vector_below_average);
		vector<int> gray_level_above_average = VectorFromIndex(gray_level, index_vector_above_average);

		//divide gray level frequency
		vector<double> gray_frequency_below_average = VectorFromIndex(gray_frequency, index_vector_below_average);
		vector<double> gray_frequency_above_average = VectorFromIndex(gray_frequency, index_vector_above_average);

		//cout << VectorSum(gray_frequency_below_average) << endl;
		//cout << VectorSum(gray_frequency_above_average) << endl;
		//cout << VectorSum(gray_frequency) << endl;

		//above average stands for foreground
		double L_above_average = VectorMultiplication(gray_level_above_average, gray_frequency_above_average) / VectorSum(gray_frequency_above_average);

		//below average stands for foreground
		double L_below_average = VectorMultiplication(gray_level_below_average, gray_frequency_below_average) / VectorSum(gray_frequency_below_average);

		//cout << L_above_average << endl;
		//cout << L_below_average << endl;

		//cout << VectorAverage(gray_level_above_average) << endl;
		//cout << VectorAverage(gray_level_below_average) << endl;

		contrast = 0.0;
	}
	//Moulden (1990): Standard Deviation
	//judge if 'S' is in the string
	if ((contrast_operator.find("S") != string::npos)) {

		cout << contrast_operator << endl;
	}

	//SD: standard deviation
	if (contrast_operator == "SD") {

		contrast = 0.0;
	}
	//SDLG: standard deviation of logarithm of luminance
	if (contrast_operator == "SDLG") {

		contrast = 0.0;
	}
	//SAW: space-average of Whittle contrast
	if (contrast_operator == "SAW") {

		contrast = L_max - L_min;
	}
	cout << "-> operator: " << contrast_operator << endl;
	cout << "-> value: " << contrast << endl;

	return contrast;
}

/*
Calculate 5-Area contrast of image

Args:
	img_gray: input image(gray scale)
	contrast_operator: contrast operator

Returns:
	contrast value of image based on 5-Area method
*/
double Contrast5Area(Mat& img_gray, const string& contrast_operator) {

	//size of matrix
	int height = img_gray.rows;
	int width = img_gray.cols;

	//5-Area ROI center
	int center_ROI_A[2] = { int(height / 2) ,int(height / 2) };
	int center_ROI_B[2] = { int(height / 4) ,int(height / 4) };
	int center_ROI_C[2] = { int(height / 4) ,int(3 * height / 4) };
	int center_ROI_D[2] = { int(3 * height / 4) ,int(height / 4) };
	int center_ROI_E[2] = { int(3 * height / 4) ,int(3 * height / 4) };

	//5-Area ROI vector
	vector<int> ROI_A = VectorROI(img_gray, center_ROI_A);
	vector<int> ROI_B = VectorROI(img_gray, center_ROI_B);
	vector<int> ROI_C = VectorROI(img_gray, center_ROI_C);
	vector<int> ROI_D = VectorROI(img_gray, center_ROI_D);
	vector<int> ROI_E = VectorROI(img_gray, center_ROI_E);

	//5-Area ROI contrast
	double contrast_A = ContrastROI(ROI_A, contrast_operator);
	double contrast_B = ContrastROI(ROI_B, contrast_operator);
	double contrast_C = ContrastROI(ROI_C, contrast_operator);
	double contrast_D = ContrastROI(ROI_D, contrast_operator);
	double contrast_E = ContrastROI(ROI_E, contrast_operator);

	//vector of 5-Area ROI contrast
	vector<double> vector_5_area_contrast = { contrast_A, contrast_B, contrast_C, contrast_D, contrast_E };

	//vector of 5-Area ROI weight
	vector<double> vector_5_area_weight = { 0.44,0.14,0.14,0.14,0.14 };

	return VectorMultiplication(vector_5_area_contrast, vector_5_area_weight);
}

/*
Calculate center contrast of image

Args:
	overloaded paramters:
		1 img_gray: input image(gray scale)
		2 which_frame: input frame object
	contrast_operator: contrast operator

Returns:
	contrast value of image based on center method
*/
//overloaded function 1:
double ContrastCenter(Mat& img_gray, const string& contrast_operator) {

	cout << "" << endl;
	cout << "-- Contrast Center" << endl;

	//size of matrix
	int height = img_gray.rows;
	int width = img_gray.cols;

	//5-Area ROI center
	int center_ROI[2] = { int(height / 2) ,int(height / 2) };

	//5-Area ROI vector
	vector<int> ROI = VectorROI(img_gray, center_ROI);

	return ContrastROI(ROI, contrast_operator);
}
//overloaded function 2:
double ContrastCenter(frame& which_frame, const string& contrast_operator) {

	return ContrastCenter(which_frame.img_gray, contrast_operator);
}